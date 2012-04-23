#ifndef SURFACE_H
#define SURFACE_H
// ****************************************************************************
// surface.h                                                        Tao project
// ****************************************************************************
//
//   File Description:
//
//      Define a subdivided srface.
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
//  (C) 2010 Taodyne SAS
// ****************************************************************************
#include "water_factory.h"
#include "basis.h"
#include <vector>

struct SurfaceMesh
// ----------------------------------------------------------------------------
//   Common drawing code for mesh-based shapes
// ----------------------------------------------------------------------------
{
    SurfaceMesh(int lines, int columns);

    std::vector<Point3> vertices;
    std::vector<GLuint> indices;
    std::vector<Point>  textures;
};

struct Surface
// ----------------------------------------------------------------------------
//   Define a subdivided surface
// ----------------------------------------------------------------------------
{
    // Constructor and destructor
    Surface(float x, float y, float w, float h, int lines, int columns);
    ~Surface();

    // Draw Surface
    virtual void Draw();
    void Draw(SurfaceMesh* surface);

    static void     render_callback(void *arg);
    static void     identify_callback(void *arg);
    static void     delete_callback(void *arg);

    static Tree_p   surface(Real_p x, Real_p y,
                            Real_p w, Real_p h, Integer_p lines_nb,
                            Integer_p columns_nb);

private:
    // Surface parameters
    Tao::Vector3 center;
    float width, height;
    int   slices, stacks;

    enum { MAX_SURFACES = 10 };
    typedef std::pair<uint, uint> Key;
    typedef std::map<Key, SurfaceMesh *> SurfaceCache;

    static SurfaceCache cache;
};

#endif
