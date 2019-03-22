// *****************************************************************************
// water.cpp                                                       Tao3D project
// *****************************************************************************
//
// File description:
//
//   Water implementation.
//
//
//
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2012-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
// (C) 2012-2013, Catherine Burvelle <catherine@taodyne.com>
// (C) 2012-2015,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2012-2013, Jérôme Forissier <jerome@taodyne.com>
// (C) 2012-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
// *****************************************************************************
// This file is part of Tao3D
//
// Tao3D is free software: you can r redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tao3D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tao3D, in a file named COPYING.
// If not, see <https://www.gnu.org/licenses/>.
// *****************************************************************************
#include "water.h"
#include "water_factory.h"
#include "tao/graphic_state.h"

DLL_PUBLIC Tao::GraphicState * graphic_state = NULL;
#define tao WaterFactory::instance()->tao



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

Water::Water(int w, int h)
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
    : pcontext(NULL), ping(0), pong(0),
      width(w), height(h), ratio(0.95), strength(1.0), frame(0), pass(0)
{
    checkGLContext();

    IFTRACE(water_surface)
            debug() << "Creation successfull" << "\n";

    // Check that we can use texture lookups in vertex shaders.
    // If not, then disabling the use of water strength. Refs #3305.
    int MaxVertexTextureImageUnits;
    GL.Get(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &MaxVertexTextureImageUnits);
    if(MaxVertexTextureImageUnits == 0)
        strength = 0.0;
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
    // Use GL state to transfer textures in Tao
    GL.Enable(GL_TEXTURE_2D);
    switch(pass)
    {
    case 0: break;
    case 1: GL.BindTexture(GL_TEXTURE_2D, pong); break;
    case 2: GL.BindTexture(GL_TEXTURE_2D, ping); break;
    default:
        XL_ASSERT(!"Invalid value");
    }

    // We don't want to use Tao filter settings (notably mipmap settings)
    // So we force textures to GL_LINEAR.
    GL.TexParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    GL.TexParameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
    if(failed)
        return;

    IFTRACE(water_surface)
            debug() << "Add drop" << "\n";

    checkGLContext();

    // Assure we have a correct state before make changes
    GL.Sync();

    // Save current settings
    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                 GL_TEXTURE_BIT | GL_VIEWPORT_BIT);

    // Prepare to draw into buffer
    GL.BindFramebuffer(GL_FRAMEBUFFER, frame);

    // Switch to correct buffer and bind
    // the other as a texture
    switch(pass)
    {
    case 0:
        GL.DrawBuffer(GL_COLOR_ATTACHMENT0);
        break;
    case 1:
        GL.DrawBuffer(GL_COLOR_ATTACHMENT0);
        GL.Enable(GL_TEXTURE_2D);
        GL.BindTexture(GL_TEXTURE_2D, pong);
        break;
    case 2:
        GL.DrawBuffer(GL_COLOR_ATTACHMENT1);
        GL.Enable(GL_TEXTURE_2D);
        GL.BindTexture(GL_TEXTURE_2D, ping);
        break;
    default:
        XL_ASSERT(!"Invalid value");
    }

    // Clear color
    GL.ClearColor(0.0, 0.0, 0.0, 1.0);
    GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GL.Viewport(0, 0, width, height);

    // Bind drop shader
    GL.UseProgram(dropShader->programId());

    // Set uniforms
    GLfloat center[2] = {(float) x, (float) y};
    GL.Uniform2fv(uniforms["dropCenter"], 1, center);
    GL.Uniform(uniforms["dropRadius"], (float) radius);
    GL.Uniform(uniforms["dropStrength"], (float) strength);

    GL.Begin(GL_QUADS);
    GL.TexCoord( 0 , 0);
    GL.Vertex  (-width/2, -height/2);
    GL.TexCoord( 1 , 0);
    GL.Vertex  ( width/2, -height/2);
    GL.TexCoord( 1,  1);
    GL.Vertex  ( width/2,  height/2);
    GL.TexCoord( 0,  1);
    GL.Vertex  (-width/2,  height/2);
    GL.End();

    GL.UseProgram(0);

    GL.BindTexture(GL_TEXTURE_2D, 0);
    GL.Disable(GL_TEXTURE_2D);

    GL.BindFramebuffer(GL_FRAMEBUFFER, 0);

    // Ping pong technique
    switch(pass)
    {
    case 0:
    case 1:
        pass = 2;
        break;
    case 2:
        pass = 1;
        break;
    default:
        XL_ASSERT(!"Invalid value");
    }

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
    if(failed)
        return;

    IFTRACE(water_surface)
            debug() << "Update water" << "\n";

    checkGLContext();

    // Assure we have a correct state before make changes
    GL.Sync();

    // Save current settings
    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_TEXTURE_BIT | GL_VIEWPORT_BIT);

    // Prepare to draw into buffer
    GL.BindFramebuffer(GL_FRAMEBUFFER, frame);

    // Switch to correct buffer and bind
    // the other as a texture
    switch(pass)
    {
    case 0:
        GL.DrawBuffer(GL_COLOR_ATTACHMENT0);
        break;
    case 1:
        GL.DrawBuffer(GL_COLOR_ATTACHMENT0);
        GL.Enable(GL_TEXTURE_2D);
        GL.BindTexture(GL_TEXTURE_2D, pong);
        break;
    case 2:
        GL.DrawBuffer(GL_COLOR_ATTACHMENT1);
        GL.Enable(GL_TEXTURE_2D);
        GL.BindTexture(GL_TEXTURE_2D, ping);
        break;
    default:
        XL_ASSERT(!"Invalid value");
    }

    // Clear color
    GL.ClearColor(0.0, 0.0, 0.0, 1.0);
    GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GL.Viewport(0, 0, width, height);

    // Bind update shader
    GL.UseProgram(updateShader->programId());

    // Set uniforms
    GLfloat delta[2] = { 1.0f / width, 1.0f / height};
    GL.Uniform2fv(uniforms["updateDelta"], 1, delta);
    GL.Uniform(uniforms["updateRatio"], ratio);

    GL.Begin(GL_QUADS);
    GL.TexCoord( 0 , 0);
    GL.Vertex  (-width/2, -height/2);
    GL.TexCoord( 1 , 0);
    GL.Vertex  ( width/2, -height/2);
    GL.TexCoord( 1,  1);
    GL.Vertex  ( width/2,  height/2);
    GL.TexCoord( 0,  1);
    GL.Vertex  (-width/2,  height/2);
    GL.End();

    GL.UseProgram(0);

    GL.BindTexture(GL_TEXTURE_2D, 0);
    GL.Disable(GL_TEXTURE_2D);

    GL.BindFramebuffer(GL_FRAMEBUFFER, 0);

    // Ping pong technique
    switch(pass)
    {
    case 0:
    case 1:
        pass = 2;
        break;
    case 2:
        pass = 1;
        break;
    default:
        XL_ASSERT(!"Invalid value");
    }

    // Restore settings
    glPopAttrib();

}


void Water::checkGLContext()
// ----------------------------------------------------------------------------
//   Re-create context-dependent resources if GL context has changed
// ----------------------------------------------------------------------------
{
    tao->makeGLContextCurrent();
    if (pcontext != QGLContext::currentContext())
    {
        IFTRACE(water_surface)
                debug() << "Context has changed" << "\n";

        pcontext = QGLContext::currentContext();

        // Synchronise state
        GL.Sync();

        createShaders();     // Create all shaders
        createTexture(ping); // Create ping texture
        createTexture(pong); // Create pong texture
        createBuffer();      // Create fbo

        // Reset pass
        pass = 0;
    }
}


void Water::createTexture(uint& texId)
// ----------------------------------------------------------------------------
//   Create a texture to attach to the fbo
// ----------------------------------------------------------------------------
{
    // Don't need to create texture if already failed
    if(failed)
        return;

    // Create texture for ping pong technic
    GL.GenTextures(1, &texId);
    GL.BindTexture(GL_TEXTURE_2D, texId);

    GL.TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    GL.TexParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    GL.TexParameter(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    GL.TexParameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GL.TexParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    GL.BindTexture(GL_TEXTURE_2D, 0);

    tao->showGlErrors();

    IFTRACE(water_surface)
                debug() << "Create texture: " << texId << "\n";
}


void Water::createBuffer()
// ----------------------------------------------------------------------------
//   Create frame buffer
// ----------------------------------------------------------------------------
{
    if(frame)
        GL.DeleteFramebuffers(1, &frame);

    // Check if graphic card has really two color attachments
    // because we need it to make ping-pong technics
    GLint max_color_attachments = 0;
    GL.Get(GL_MAX_COLOR_ATTACHMENTS, &max_color_attachments);
    if(max_color_attachments < 2)
    {
        failed = true;
        IFTRACE(water_surface)
                    debug() << "No enough color attachments available for this module. " << "\n";
    }

    // Don't need to create fbo if already failed
    if(failed)
        return;

    // Create fbo and attach textures to it
    GL.GenFramebuffers(1, &frame); // Generate one frame buffer and store the ID in frame
    GL.BindFramebuffer(GL_FRAMEBUFFER, frame); // Bind our frame buffer
    GL.FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                            GL_TEXTURE_2D, ping, 0);
    GL.FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
                            GL_TEXTURE_2D, pong, 0);
    checkFramebufferStatus();
    GL.BindFramebuffer(GL_FRAMEBUFFER, 0);

    tao->showGlErrors();

    IFTRACE(water_surface)
                debug() << "Create frame buffer: " << frame << "\n";
}


void Water::createShaders()
// ----------------------------------------------------------------------------
//   Create shader programs
// ----------------------------------------------------------------------------
{
    IFTRACE(water_surface)
                debug() << "Create shaders" << "\n";

    createDropShader();
    createUpdateShader();
}


void Water::createDropShader()
// ----------------------------------------------------------------------------
//   Create shader used to add drops
// ----------------------------------------------------------------------------
{
    if(!failed)
    {
        IFTRACE(water_surface)
                debug() << "Create drop shader" << "\n";

        delete dropShader;

        dropShader = new QGLShaderProgram(pcontext);
        bool ok = false;

        // Basic vertex shader
        static string vSrc =
                "/********************************************************************************\n"
                "**                                                                               \n"
                "** Copyright (C) 2011 Taodyne.                                                   \n"
                "** All rights reserved.                                                          \n"
                "** Contact: Taodyne (contact@taodyne.com)                                        \n"
                "**                                                                               \n"
                "** This file is part of the Tao3D application, developped by Taodyne.\n"
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
                "** This file is part of the Tao3D application, developped by Taodyne.\n"
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
            uniforms["dropCenter"]    = GL.GetUniformLocation(id, "center");
            uniforms["dropRadius"]    = GL.GetUniformLocation(id, "radius");
            uniforms["dropStrength"]  = GL.GetUniformLocation(id, "strength");
        }
    }
}


void Water::createUpdateShader()
// ----------------------------------------------------------------------------
//   Create shader used to update water
// ----------------------------------------------------------------------------
{
    if(!failed)
    {
        IFTRACE(water_surface)
                debug() << "Create update shader" << "\n";

        delete updateShader;

        updateShader = new QGLShaderProgram(pcontext);
        bool ok = false;

        // Basic vertex shader
        static string vSrc =
                "/********************************************************************************\n"
                "**                                                                               \n"
                "** Copyright (C) 2011 Taodyne.                                                   \n"
                "** All rights reserved.                                                          \n"
                "** Contact: Taodyne (contact@taodyne.com)                                        \n"
                "**                                                                               \n"
                "** This file is part of the Tao3D application, developped by Taodyne.\n"
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
                "** This file is part of the Tao3D application, developped by Taodyne.\n"
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
            uniforms["updateDelta"] = GL.GetUniformLocation(id, "delta");
            uniforms["updateRatio"] = GL.GetUniformLocation(id, "ratio");
        }
    }
}


void Water::checkFramebufferStatus()
// ----------------------------------------------------------------------------
//   Check the status of current frame buffer
// ----------------------------------------------------------------------------
{
    GLenum status;
    status= (GLenum) GL.CheckFramebufferStatus(GL_FRAMEBUFFER);

    text error = "";
    switch(status) {
        case GL_FRAMEBUFFER_COMPLETE_EXT: return ;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
            error = "Framebuffer incomplete,incomplete attachment\n"; break;
        case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
            error = "Unsupported framebuffer format\n"; break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
            error = "Framebuffer incomplete,missing attachment\n"; break;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            error = "Framebuffer incomplete,attached images must have same dimensions\n"; break;
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
            error = "Framebuffer incomplete,attached images must have same format\n"; break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
            error = "Framebuffer incomplete,missing draw buffer\n"; break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
            error = "Framebuffer incomplete,missing read buffer\n"; break;
        default:
            error = "Framebuffer incomplete"; break;
    }

    IFTRACE(water_surface)
                debug() << error;

    failed = true;
}


std::ostream & Water::debug()
// ----------------------------------------------------------------------------
//   Convenience method to log with a common prefix
// ----------------------------------------------------------------------------
{
    std::cerr << "[Water] " << (void*)this << " ";
    return std::cerr;
}

