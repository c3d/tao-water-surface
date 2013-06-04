#ifndef WATER_FACTORY_H
#define WATER_FACTORY_H
// ****************************************************************************
//  water_factory.h                                                 Tao project
// ****************************************************************************
//
//   File Description:
//
//    Create and manipulate water surfaces.
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
    static void          delete_callback(void *arg);

    // XL interface
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
