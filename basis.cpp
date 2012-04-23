// ****************************************************************************
//  Basis.cpp                                                   Tao project
// ****************************************************************************
//
//   File Description:
//
//   Basis implementation.
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
#include "basis.h"

bool Basis::tested = false;
bool Basis::licensed = false;

// ============================================================================
//
//    Basis
//
// ============================================================================

Basis::Basis(const QGLContext **pcontext)
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
    : pcontext(pcontext)
{
}

Basis::~Basis()
// ----------------------------------------------------------------------------
//   Destruction
// ----------------------------------------------------------------------------
{
}

void Basis::checkGLContext()
// ----------------------------------------------------------------------------
//   Re-create context-dependent resources if GL context has changed
// ----------------------------------------------------------------------------
{
    if (*pcontext != QGLContext::currentContext())
    {
        createShaders();
        *pcontext = QGLContext::currentContext();
    }
}


void Basis::createShaders()
// ----------------------------------------------------------------------------
//   Create shader programs
// ----------------------------------------------------------------------------
{
}
