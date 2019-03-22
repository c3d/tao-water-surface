#ifndef WATER_FACTORY_H
#define WATER_FACTORY_H
// *****************************************************************************
// water_factory.h                                                 Tao3D project
// *****************************************************************************
//
// File description:
//
//    Create and manipulate water surfaces.
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


class WaterFactory
{
public:
    WaterFactory();
    virtual ~WaterFactory() {}

    Water*  water(text name);

public:
    static WaterFactory* instance();
    static void          destroy();
    static bool          checkLicense();

    static void          render_callback(void *arg);
    static void          identify_callback(void *arg);
    static void          delete_callback(void *arg);

    // XL interface
    static Real_p        water_strength(text);
    static Name_p        water_show(text name);
    static Name_p        water_only(text name);
    static Name_p        water_remove(text name);
    static Name_p        water_extenuation(text name, Real_p ratio);
    static Name_p        add_drop(text name, Real_p x, Real_p y,
                                  Real_p radius, Real_p strength);
    static Name_p        add_random_drops(text name, Integer_p number);

public:
    // Pointer to Tao functions
    static const Tao::ModuleApi *tao;

protected:
    typedef std::map<text, Water *>  water_map;
    water_map    waters;

protected:
    static WaterFactory * factory;
};

#endif
