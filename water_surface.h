#ifndef WATER_SURFACE_H
#define WATER_SURFACE_H
// ****************************************************************************
//  water_surface.h                                                 Tao project
// ****************************************************************************
//
//   File Description:
//
//    Prototype of the function used by water_surface.tbl to
//    implement the new XL primitive
//
//
//
//
//
//
// ****************************************************************************
// This software is property of Taodyne SAS - Confidential
// Ce logiciel est la propriété de Taodyne SAS - Confidentiel
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2011 Baptiste Soulisse <baptiste.soulisse@taodyne.com>
//  (C) 2011 Taodyne SAS
// ****************************************************************************
#include "tree.h"
#include "main.h"
#include "runtime.h"
#include "base.h"
#include "tao/module_api.h"
#include "water.h"

using namespace XL;

Tree_p create_water(Tree_p);
Tree_p water_extenuation(Tree_p, Real_p ratio);

Tree_p add_drop(Tree_p, Real_p x, Real_p y,
                Real_p radius, Real_p strength);
Tree_p add_random_drops(Tree_p, Integer_p number);

Tree_p surface(Tree_p tree, Real_p x, Real_p y, Real_p w,
               Real_p h, Integer_p lines_nb, Integer_p columns_nb);

#endif
