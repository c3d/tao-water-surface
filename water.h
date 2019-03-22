#ifndef WATER_H
#define WATER_H
// *****************************************************************************
// water.h                                                         Tao3D project
// *****************************************************************************
//
// File description:
//
//      Define water.
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
// (C) 2012-2014,2019, Christophe de Dinechin <christophe@dinechin.org>
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

#include "tao/coords3d.h"
#include "tao/module_api.h"
#include "tao/tao_gl.h"
#include "basics.h" // XLR
#include <QGLContext>
#include <QGLShaderProgram>

using namespace std;
using namespace Tao;

struct Water
{
    Water(int w = 256, int h = 256);
    virtual ~Water();

    virtual void    Draw();

    void            drop(double x, double y, double radius, double strength);
    void            randomDrops(int n);
    void            update();

    void            extenuation(float r);

private:
    // Re-create shaders if GL context has changed
    void            checkGLContext();

    void            createShaders();
    void            createDropShader();
    void            createUpdateShader();

    void            createTexture(uint& texId);
    void            createBuffer();

    void checkFramebufferStatus();

    std::ostream &  debug();

public:
    const QGLContext    *pcontext;

    uint     ping, pong;
    int      width, height;

    // Water settings
    float    ratio;
    float    strength;

private:
   // FBO settings
   uint frame;

   uint pass;

   // Shaders settings
   static bool  failed;
   static QGLShaderProgram *dropShader, *updateShader;
   static std::map<text, GLint> uniforms;
};


#endif
