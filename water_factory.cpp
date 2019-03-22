// *****************************************************************************
// water_factory.cpp                                               Tao3D project
// *****************************************************************************
//
// File description:
//
//     Create and manipulate a water surface.
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
#include "water_factory.h"
#include <iostream>


const Tao::ModuleApi *WaterFactory::tao = NULL;

WaterFactory* WaterFactory::factory = NULL;

WaterFactory::WaterFactory()
// ----------------------------------------------------------------------------
//   Create water factory
// ----------------------------------------------------------------------------
{
}


Water* WaterFactory::water(text name)
// ----------------------------------------------------------------------------
//   Return water instance according to its name
// ----------------------------------------------------------------------------
{
    Water* water = NULL;
    water_map::iterator found = waters.find(name);
    if(found != waters.end())
    {
        water = (*found).second;
    }
    else
    {
        water = new Water();
        waters[name] = water;
    }
    return water;
}


WaterFactory* WaterFactory::instance()
// ----------------------------------------------------------------------------
//   Return factory instance (singleton)
// ----------------------------------------------------------------------------
{
    if (!factory)
        factory = new WaterFactory;
    return factory;
}


void WaterFactory::destroy()
// ----------------------------------------------------------------------------
//   Destroy factory instance
// ----------------------------------------------------------------------------
{
    if (!factory)
        return;
    delete factory;
    factory = NULL;
}


void WaterFactory::render_callback(void *arg)
// ----------------------------------------------------------------------------
//   Find water by name and draw it
// ----------------------------------------------------------------------------
{
    text name = text((const char *)arg);
    Water * water = WaterFactory::instance()->water(name);
    if (water)
        water->Draw();
}


void WaterFactory::identify_callback(void *arg)
// ----------------------------------------------------------------------------
//   Identify callback: don't do anything
// ----------------------------------------------------------------------------
{
    (void) arg;
}


void WaterFactory::delete_callback(void *arg)
// ----------------------------------------------------------------------------
//   Delete water name
// ----------------------------------------------------------------------------
{
    free(arg);
}


Real_p WaterFactory::water_strength(text name)
// ----------------------------------------------------------------------------
//   Strength of water
// ----------------------------------------------------------------------------
{
    Water* water = instance()->water(name);
    return new Real(water->strength);
}


Name_p WaterFactory::water_show(text name)
// ----------------------------------------------------------------------------
//   Show water
// ----------------------------------------------------------------------------
{
    Water* water = instance()->water(name);
    water->update();
    instance()->tao->AddToLayout2(WaterFactory::render_callback,
                                 WaterFactory::identify_callback,
                                 strdup(name.c_str()),
                                 WaterFactory::delete_callback);
    return XL::xl_true;
}


Name_p WaterFactory::water_only(text name)
// ----------------------------------------------------------------------------
//   Purge all other waters from memory
// ----------------------------------------------------------------------------
{
    WaterFactory * f = WaterFactory::instance();
    water_map::iterator n = f->waters.begin();
    for (water_map::iterator v = f->waters.begin();
         v != f->waters.end();
         v = n)
    {
        if (name != (*v).first)
        {
            Water *w = (*v).second;
            f->waters.erase(v);
            delete w;
            n = f->waters.begin();
        }
        else
        {
            n = ++v;
        }
    }
    return xl_false;
}


Name_p WaterFactory::water_remove(text name)
// ----------------------------------------------------------------------------
//   Purge the given water from memory
// ----------------------------------------------------------------------------
{
    WaterFactory * f = WaterFactory::instance();
    water_map::iterator found = f->waters.find(name);
    if (found != f->waters.end())
    {
        Water *w = (*found).second;
        f->waters.erase(found);
        delete w;
        return XL::xl_true;
    }
    return XL::xl_false;
}


Name_p WaterFactory::water_extenuation(text name, Real_p ratio)
// ----------------------------------------------------------------------------
//   Set extenuation of the water surface
// ----------------------------------------------------------------------------
{
    Water* water = instance()->water(name);
    if(water)
    {
        water->extenuation(ratio);
        return xl_true;
    }
    return xl_false;
}


Name_p WaterFactory::add_drop(text name, Real_p x, Real_p y, Real_p radius, Real_p strength)
// ----------------------------------------------------------------------------
//   Add a drop to a current water
// ----------------------------------------------------------------------------
{
    Water* water = instance()->water(name);
    if(water)
    {
        water->drop(x, y, radius, strength);
        return xl_true;
    }
    return xl_false;
}


Name_p WaterFactory::add_random_drops(text name, Integer_p number)
// ----------------------------------------------------------------------------
//   Add some random drops to a water
// ----------------------------------------------------------------------------
{
    Water* water = instance()->water(name);
    if(water)
    {
        water->randomDrops(number);
        return xl_true;
    }
    return xl_false;
}


XL_DEFINE_TRACES

int module_init(const Tao::ModuleApi *api, const Tao::ModuleInfo *)
// ----------------------------------------------------------------------------
//   Initialize the Tao module
// ----------------------------------------------------------------------------
{
    XL_INIT_TRACES();
    WaterFactory::instance()->tao = api;

    // Check if we support floating textures to use correctly this module.
    // If not, do not create the water surface to avoid GL errors. Refs #2690.
    if (!api->isGLExtensionAvailable("GL_ARB_texture_float"))
    {
        XL::Ooops("Water: GL_ARB_texture_float extension not available");
        return -1;
    }
    return 0;
}


int module_exit()
// ----------------------------------------------------------------------------
//   Uninitialize the Tao module
// ----------------------------------------------------------------------------
{
    WaterFactory::water_only("");
    WaterFactory::destroy();
    return 0;
}
