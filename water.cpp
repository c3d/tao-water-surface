// ****************************************************************************
//  Water.cpp                                                     Tao project
// ****************************************************************************
//
//   File Description:
//
//   Water implementation.
//
//
//
//
//
//
//
// ****************************************************************************
// This software is property of Taodyne SAS - Confidential
// Ce logiciel est la propriété de Taodyne SAS - Confidentiel
//  (C) 2011 Baptiste Soulisse <baptiste.soulisse@taodyne.com>
//  (C) 2011 Taodyne SAS
// ****************************************************************************
#include "water.h"
#include "water_factory.h"

// ============================================================================
//
//   Water
//
// ============================================================================

// Shaders setting
bool                  Water::failed = false;
QGLShaderProgram*     Water::dropShader = NULL;
QGLShaderProgram*     Water::updateShader = NULL;
std::map<text, GLint> Water::uniforms;
const QGLContext*     Water::context = NULL;

Water::Water(int w, int h)
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
    : Basis(&context), textureId(0), ratio(0.95)
{
    checkGLContext();
    width  = w;
    height = h;

    frame  = WaterFactory::instance()->tao->newFrameBufferObjectWithFormat(w, h, GL_RGBA16F_ARB);
    WaterFactory::instance()->tao->releaseFrameBufferObject(frame);

    randomDrops(20);
}


Water::~Water()
// ----------------------------------------------------------------------------
//   Destruction
// ----------------------------------------------------------------------------
{
}

void Water::Draw()
// ----------------------------------------------------------------------------
//   Draw : Do nothing
// ----------------------------------------------------------------------------
{
    if (!tested)
    {
        licensed = WaterFactory::instance()->tao->checkImpressOrLicense("WaterSurface 1.0");
        tested = true;
    }
    if (!licensed && !WaterFactory::instance()->tao->blink(1.0, 0.2, 300.0))
        return;
}


void Water::extenuation(float r)
// ----------------------------------------------------------------------------
//   Set extenuation of the water
// ----------------------------------------------------------------------------
{
    ratio = r;

    // Clamp to avoid diverging
    if(ratio > 1.0)
        ratio = 1.0;
}


void Water::drop(double x, double y, double radius, double strength)
// ----------------------------------------------------------------------------
//   Add a drop to the water
// ----------------------------------------------------------------------------
{
    checkGLContext();

    // Save current settings
    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_VIEWPORT_BIT);

    // Save current framebuffer, if any
    GLint fbname = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbname);

    // Make sure output buffer has the right size (resolution may have changed)
    // and prepare to draw into it
    WaterFactory::instance()->tao->bindFrameBufferObject(frame);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, width, height);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glUseProgram(dropShader->programId());

    GLfloat center[2] = {x, y};
    glUniform2fv(uniforms["dropCenter"], 1, center);
    glUniform1f(uniforms["dropRadius"], radius);
    glUniform1f(uniforms["dropStrength"], strength);

    glBegin(GL_QUADS);
    glTexCoord2i( 0 , 0);
    glVertex2i  (-width/2, -height/2);
    glTexCoord2i( 1 , 0);
    glVertex2i  ( width/2, -height/2);
    glTexCoord2i( 1,  1);
    glVertex2i  ( width/2,  height/2);
    glTexCoord2i( 0,  1);
    glVertex2i  (-width/2,  height/2);
    glEnd();

    glUseProgram(0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    // Done with drawing.
    // Make buffer available to the shader as a texture.

    WaterFactory::instance()->tao->releaseFrameBufferObject(frame);
    textureId = WaterFactory::instance()->tao->frameBufferObjectToTexture(frame);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    // Select draw buffer
    if (fbname)
        glBindFramebuffer(GL_FRAMEBUFFER, fbname);
    else
        glDrawBuffer(GL_BACK);

    // Restore settings
    glPopAttrib();
}


void Water::randomDrops(int n)
// ----------------------------------------------------------------------------
//   Add some random drops
// ----------------------------------------------------------------------------
{
    for(int i = 0; i < n; i++)
    {
        double x = XL::xl_random(0.0, 1.0) * 2 - 1;
        double y = XL::xl_random(0.0, 1.0) * 2 - 1;
        drop(x, y, 1.0, (i & 1) ? 1.0 : -1.0);
    }
}

void Water::update()
// ----------------------------------------------------------------------------
//   Update the water
// ----------------------------------------------------------------------------
{
    checkGLContext();

    // Save current settings
    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_VIEWPORT_BIT);

    // Save current framebuffer, if any
    GLint fbname = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbname);

    // Make sure output buffer has the right size (resolution may have changed)
    // and prepare to draw into it
    WaterFactory::instance()->tao->bindFrameBufferObject(frame);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, width, height);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glUseProgram(updateShader->programId());

    GLfloat delta[2] = {1.0 / width, 1.0 / height};
    glUniform2fv(uniforms["updateDelta"], 1, delta);
    glUniform1f(uniforms["updateRatio"], ratio);

    glBegin(GL_QUADS);
    glTexCoord2i( 0 , 0);
    glVertex2i  (-width/2, -height/2);
    glTexCoord2i( 1 , 0);
    glVertex2i  ( width/2, -height/2);
    glTexCoord2i( 1,  1);
    glVertex2i  ( width/2,  height/2);
    glTexCoord2i( 0,  1);
    glVertex2i  (-width/2,  height/2);
    glEnd();

    glUseProgram(0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    // Done with drawing.
    // Make buffer available to the shader as a texture.
    WaterFactory::instance()->tao->releaseFrameBufferObject(frame);

    // Select draw buffer
    if (fbname)
        glBindFramebuffer(GL_FRAMEBUFFER, fbname);
    else
        glDrawBuffer(GL_BACK);

    // Restore settings
    glPopAttrib();
}


void Water::createShaders()
// ----------------------------------------------------------------------------
//   Create all shaders
// ----------------------------------------------------------------------------
{
    if(!failed)
    {
        createDropShader();
        createUpdateShader();
    }
}


void Water::createDropShader()
// ----------------------------------------------------------------------------
//   Create shader used to add drops
// ----------------------------------------------------------------------------
{
    bool ok = false;
    if(! dropShader)
    {
        dropShader = new QGLShaderProgram();
        // Basic vertex shader
        static string vSrc =
                "/********************************************************************************\n"
                "**                                                                               \n"
                "** Copyright (C) 2011 Taodyne.                                                   \n"
                "** All rights reserved.                                                          \n"
                "** Contact: Taodyne (contact@taodyne.com)                                        \n"
                "**                                                                               \n"
                "** This file is part of the Tao Presentations application, developped by Taodyne.\n"
                "** It can be only used in the software and these modules.                        \n"
                "**                                                                               \n"
                "** If you have questions regarding the use of this file, please contact          \n"
                "** Taodyne at contact@taodyne.com.                                               \n"
                "**                                                                               \n"
                "********************************************************************************/\n"
                "varying vec2 coord;"
                "void main()"
                "{"
                "   coord = gl_Vertex.xy * 0.5 + 0.5;"
                "   gl_Position = vec4(gl_Vertex.xyz, 1.0);"
                "}";

        static string fSrc =
                "/********************************************************************************\n"
                "**                                                                               \n"
                "** Copyright (C) 2011 Taodyne.                                                   \n"
                "** All rights reserved.                                                          \n"
                "** Contact: Taodyne (contact@taodyne.com)                                        \n"
                "**                                                                               \n"
                "** This file is part of the Tao Presentations application, developped by Taodyne.\n"
                "** It can be only used in the software and these modules.                        \n"
                "**                                                                               \n"
                "** If you have questions regarding the use of this file, please contact          \n"
                "** Taodyne at contact@taodyne.com.                                               \n"
                "**                                                                               \n"
                "********************************************************************************/\n"
                "const float PI = 3.141592653589793;"
                "uniform sampler2D texture;"
                "uniform vec2 center;"
                "uniform float radius;"
                "uniform float strength;"
                "uniform int   textureId;"
                "varying vec2 coord;"
                "void main() {"
                "   vec4 info = texture2D(texture, coord);"

                "   float drop = max(0.0, 1.0 - length(center * 0.5 + 0.5 - coord) / (radius / 100.0));"
                "   drop = 0.5 - cos(drop * PI) * 0.5;"
                "   info.r += drop * (strength / 1000.0);"
                "   gl_FragColor = vec4(info.rgb, 1.0);"
                "}";


        if (dropShader->addShaderFromSourceCode(QGLShader::Vertex, vSrc.c_str()))
        {
            if (dropShader->addShaderFromSourceCode(QGLShader::Fragment, fSrc.c_str()))
            {
                ok = true;
            }
            else
            {
                std::cerr << "Drop shader" << "\n";
                std::cerr << "Error loading fragment shader code: " << "\n";
                std::cerr << dropShader->log().toStdString();
            }
        }
        else
        {
            std::cerr << "Drop shader" << "\n";
            std::cerr << "Error loading vertex shader code: " << "\n";
            std::cerr << dropShader->log().toStdString();
        }

        if (!ok)
        {
            delete dropShader;
            dropShader = NULL;
            failed = true;
        }
        else
        {
            dropShader->link();

            // Save uniform locations
            uint id = dropShader->programId();
            uniforms["dropCenter"]    = glGetUniformLocation(id, "center");
            uniforms["dropRadius"]    = glGetUniformLocation(id, "radius");
            uniforms["dropStrength"]  = glGetUniformLocation(id, "strength");
        }
    }
}

void Water::createUpdateShader()
// ----------------------------------------------------------------------------
//   Create shader used to update water
// ----------------------------------------------------------------------------
{
    bool ok = false;
    if(! updateShader)
    {
        updateShader = new QGLShaderProgram();
        // Basic vertex shader
        static string vSrc =
                "/********************************************************************************\n"
                "**                                                                               \n"
                "** Copyright (C) 2011 Taodyne.                                                   \n"
                "** All rights reserved.                                                          \n"
                "** Contact: Taodyne (contact@taodyne.com)                                        \n"
                "**                                                                               \n"
                "** This file is part of the Tao Presentations application, developped by Taodyne.\n"
                "** It can be only used in the software and these modules.                        \n"
                "**                                                                               \n"
                "** If you have questions regarding the use of this file, please contact          \n"
                "** Taodyne at contact@taodyne.com.                                               \n"
                "**                                                                               \n"
                "********************************************************************************/\n"
                "varying vec2 coord;"
                "void main()"
                "{"
                "   coord = gl_Vertex.xy * 0.5 + 0.5;"
                "   gl_Position = vec4(gl_Vertex.xyz, 1.0);"
                "}";

        static string fSrc =
                "/********************************************************************************\n"
                "**                                                                               \n"
                "** Copyright (C) 2011 Taodyne.                                                   \n"
                "** All rights reserved.                                                          \n"
                "** Contact: Taodyne (contact@taodyne.com)                                        \n"
                "**                                                                               \n"
                "** This file is part of the Tao Presentations application, developped by Taodyne.\n"
                "** It can be only used in the software and these modules.                        \n"
                "**                                                                               \n"
                "** If you have questions regarding the use of this file, please contact          \n"
                "** Taodyne at contact@taodyne.com.                                               \n"
                "**                                                                               \n"
                "********************************************************************************/\n"
                "uniform sampler2D texture;"
                "uniform float ratio;"
                "uniform vec2 delta;"
                ""
                "varying vec2 coord;"
                "void main() {"
                "  /* get vertex info */"
                "  vec4 info = texture2D(texture, coord);"
                "  "
                "  /* calculate average neighbor height */"
                "  vec2 dx = vec2(delta.x, 0.0);"
                "  vec2 dy = vec2(0.0, delta.y);"
                "  float average = ("
                "    texture2D(texture, coord - dx).r +"
                "    texture2D(texture, coord - dy).r +"
                "    texture2D(texture, coord + dx).r +"
                "    texture2D(texture, coord + dy).r"
                "  ) * 0.25;"
                "  "
                "  /* change the velocity to move toward the average */"
                "  info.g += (average - info.r) * 2.0;"
                "  "
                "  /* attenuate the velocity a little so waves do not last forever */"
                "  info.g *= ratio;"
                "  "
                "  /* move the vertex along the velocity */"
                "  info.r += info.g;"
                ""
                "  gl_FragColor = vec4(info.rgb, 1.0);"
                "}";


        if (updateShader->addShaderFromSourceCode(QGLShader::Vertex, vSrc.c_str()))
        {
            if (updateShader->addShaderFromSourceCode(QGLShader::Fragment, fSrc.c_str()))
            {
                ok = true;
            }
            else
            {
                std::cerr << "Update shader" << "\n";
                std::cerr << "Error loading fragment shader code: " << "\n";
                std::cerr << updateShader->log().toStdString();
            }
        }
        else
        {
            std::cerr << "Update shader" << "\n";
            std::cerr << "Error loading vertex shader code: " << "\n";
            std::cerr << updateShader->log().toStdString();
        }

        if (!ok)
        {
            delete updateShader;
            updateShader = NULL;
            failed = true;
        }
        else
        {
            updateShader->link();

            // Save uniform locations
            uint id = updateShader->programId();
            uniforms["updateDelta"] = glGetUniformLocation(id, "delta");
            uniforms["updateRatio"] = glGetUniformLocation(id, "ratio");
        }
    }
}


