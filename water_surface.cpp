// *****************************************************************************
// water_surface.cpp                                               Tao3D project
// *****************************************************************************
//
// File description:
//
//     Implementation of the XL primitives for the "water_surface" module.
//
//
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2014,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2012, Soulisse Baptiste <baptiste.soulisse@taodyne.com>
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

#include "water_surface.h"
#include <iostream>
#include "coords3d.h"

XL_DEFINE_TRACES

Water *water;

Tree_p create_water(Tree_p)
// ----------------------------------------------------------------------------
//   Create the texture used to simulate water's behavior
// ----------------------------------------------------------------------------
{
    water = new Water(256, 256);
    water->update();
    Basis::tao->AddToLayout2(Basis::render_callback,
                             Basis::identify_callback,
                             water, Basis::delete_callback);
    return xl_true;
}


Tree_p surface(Tree_p, Real_p x, Real_p y, Real_p w,
               Real_p h, Integer_p lines_nb, Integer_p columns_nb)
// ----------------------------------------------------------------------------
//   Define a subdivded surface
// ----------------------------------------------------------------------------
{
    Surface* s = new Surface(x, y, w, h, lines_nb, columns_nb);
    Surface::tao->addToLayout(Surface::render_callback,
                            s, Surface::delete_callback);

    return xl_true;
}


Tree_p add_drop(Tree_p, Real_p x, Real_p y, Real_p radius, Real_p strength)
// ----------------------------------------------------------------------------
//   Add a drop to the water surface
// ----------------------------------------------------------------------------
{
    if(water)
    {
        water->drop(x, y, radius, strength);
        return xl_true;
    }
    return xl_false;
}


Tree_p add_random_drops(Tree_p, Integer_p number)
// ----------------------------------------------------------------------------
//   Add some random drops to the water surface
// ----------------------------------------------------------------------------
{
    if(water)
    {
        water->randomDrops(number);
        return xl_true;
    }
    return xl_false;
}


Tree_p water_extenuation(Tree_p, Real_p ratio)
// ----------------------------------------------------------------------------
//   Set extenuation of the water surface
// ----------------------------------------------------------------------------
{
    if(water)
    {
        water->extenuation(ratio);
        return xl_true;
    }
    return xl_false;
}


int module_init(const Tao::ModuleApi *api, const Tao::ModuleInfo *)
// ----------------------------------------------------------------------------
//   Initialize the Tao module
// ----------------------------------------------------------------------------
{
    XL_INIT_TRACES();
    Basis::tao = api;
    return 0;
}


int module_exit()
// ----------------------------------------------------------------------------
//   Uninitialize the Tao module
// ----------------------------------------------------------------------------
{
    return 0;
}
