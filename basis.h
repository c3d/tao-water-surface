#ifndef BASIS_H
#define BASIS_H
// ****************************************************************************
//  Basis.h                                                     Tao project
// ****************************************************************************
//
//   File Description:
//
//      Define Basis class.
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
#include <QObject>

using namespace std;
using namespace Tao;

struct Basis : public QObject
{
    Basis(const QGLContext **pcontext = NULL);
    virtual ~Basis();

    // Re-create shaders if GL context has changed
    void            checkGLContext();
    virtual void    createShaders();
public:
    const QGLContext    **pcontext;

public:
    static bool tested, licensed;
};


#endif
