#ifndef WATER_SURFACE_H
#define WATER_SURFACE_H
// *****************************************************************************
// water_surface.h                                                 Tao3D project
// *****************************************************************************
//
// File description:
//
//    Prototype of the function used by water_surface.tbl to
//    implement the new XL primitive
//
//
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2012-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
// (C) 2013, Catherine Burvelle <catherine@taodyne.com>
// (C) 2013-2014,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2013, Jérôme Forissier <jerome@taodyne.com>
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
