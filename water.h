#ifndef WATER_H
#define WATER_H
// ****************************************************************************
//  water.h                                                        Tao project
// ****************************************************************************
//
//   File Description:
//
//      Define water.
//
//
//
//
//
// ****************************************************************************
// This software is property of Taodyne SAS - Confidential
// Ce logiciel est la propriÃ©tÃ© de Taodyne SAS - Confidentiel
//  (C) 2011 Baptiste Soulisse <baptiste.soulisse@taodyne.com>
//  (C) 2011 Taodyne SAS
// ****************************************************************************

#include "tao/coords3d.h"
#include "tao/module_api.h"
#include "tao/tao_gl.h"
#include "basis.h"
#include "basics.h" // XLR

using namespace std;
using namespace Tao;

struct Water : public Basis
{
    Water(int w = 256, int h = 256);
    virtual ~Water();

    virtual void    Draw();

    void            drop(double x, double y, double radius, double strength);
    void            randomDrops(int n);
    void            update();

    void            resize(int w, int h);
    void            extenuation(float r);
protected:
    virtual void    createShaders();

private:
    void            createDropShader();
    void            createUpdateShader();
    std::ostream &  debug();


public:
    uint     textureId;
    int      width, height;

private:
   // Water settings
   float    ratio;

   // FBO settings
   Tao::ModuleApi::fbo     *frame;

   // Shaders settings
   static bool  failed;
   static QGLShaderProgram *dropShader, *updateShader;
   static std::map<text, GLint> uniforms;
   static const QGLContext* context;

};


#endif
