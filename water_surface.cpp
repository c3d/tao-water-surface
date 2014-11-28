// ****************************************************************************
//  water_surface.cpp                                               Tao project
// ****************************************************************************
//
//   File Description:
//
//     Implementation of the XL primitives for the "water_surface" module.
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
