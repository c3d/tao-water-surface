// ****************************************************************************
//  water_factory.cpp                                               Tao project
// ****************************************************************************
//
//   File Description:
//
//     Create and manipulate a water surface.
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
#include "water_factory.h"
#include <iostream>


const Tao::ModuleApi *WaterFactory::tao = NULL;

WaterFactory* WaterFactory::factory = NULL;

// License
bool          WaterFactory::tested = false;
bool          WaterFactory::licensed = false;

WaterFactory::WaterFactory()
{
    glewInit();
}


Water* WaterFactory::water(text name)
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


bool WaterFactory::checkLicense()
// ----------------------------------------------------------------------------
//   Check module license
// ----------------------------------------------------------------------------
{
    if (!tested)
    {
        licensed = instance()->tao->checkImpressOrLicense("WaterSurface 1.001");
        tested = true;
    }

    if (!licensed && !instance()->tao->blink(1.0, 0.2, 300.0))
        return false;

    return true;
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


void WaterFactory::delete_callback(void *arg)
// ----------------------------------------------------------------------------
//   Delete water name
// ----------------------------------------------------------------------------
{
    free(arg);
}


Name_p WaterFactory::water_show(text name)
// ----------------------------------------------------------------------------
//   Show water
// ----------------------------------------------------------------------------
{
    Water* water = instance()->water(name);
    water->update();
    instance()->tao->addToLayout(WaterFactory::render_callback,
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
