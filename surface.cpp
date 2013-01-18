// ****************************************************************************
//  surface.cpp                                                     Tao project
// ****************************************************************************
//
//   File Description:
//
//   Surface implementation.
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
//  (C) 1992-2010 Christophe de Dinechin <christophe@taodyne.com>
//  (C) 2010 Jerome Forissier <jerome@taodyne.com>
//  (C) 2010 Taodyne SAS
// ****************************************************************************
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "surface.h"


SurfaceMesh::SurfaceMesh(int lines, int columns)
// ----------------------------------------------------------------------------
//   Initialize surface parameters
// ----------------------------------------------------------------------------
{
    // Subdivision steps
    float stepX = 1.0 / lines;
    float stepY = 1.0 / columns;

    // Compute vertices and textures coordinates
    for(int j = 0; j <= columns; j++)
    {
        for(int i = 0; i <= lines; i++)
        {
            vertices.push_back(Vector3(stepX * i - 0.5, stepY * j - 0.5, 0));
            textures.push_back(Vector((double) i / lines, (double) j / columns));
        }
    }

    // Compute indexes
    for(int j = 0; j < columns; j++)
    {
        for(int i = 0; i < lines; i++)
        {
            indices.push_back(j * (lines + 1) + i);
            indices.push_back(j * (lines + 1) + i + 1);
            indices.push_back((j + 1) * (lines + 1) + i + 1);
            indices.push_back((j + 1) * (lines + 1) + i);
        }
    }
}

// ============================================================================
//
//    Surface
//
// ============================================================================

Surface::SurfaceCache Surface::cache;


Surface::Surface(float x, float y, float w, float h, int slices, int stacks)
// ----------------------------------------------------------------------------
//   Construction
// ----------------------------------------------------------------------------
    : center(x, y, 0), width(w), height(h), slices(slices), stacks(stacks)
{}

Surface::~Surface()
// ----------------------------------------------------------------------------
//   Destruction
// ----------------------------------------------------------------------------
{
}

void Surface::render_callback(void *arg)
// ----------------------------------------------------------------------------
//   Rendering callback: call the render function for the object
// ----------------------------------------------------------------------------
{
    ((Surface *)arg)->Draw();
}

void Surface::identify_callback(void *)
// ----------------------------------------------------------------------------
//   Identify callback: don't do anything
// ----------------------------------------------------------------------------
{
}

void Surface::delete_callback(void *arg)
// ----------------------------------------------------------------------------
//   Delete callback: destroy object
// ----------------------------------------------------------------------------
{
    delete (Surface *)arg;
}

void Surface::Draw()
// ----------------------------------------------------------------------------
//   Draw subdivided surface
// ----------------------------------------------------------------------------
{
    // Check module license
    if(! WaterFactory::checkLicense())
        return;

    SurfaceMesh * surface = NULL;
    Key key(slices, stacks);
    SurfaceCache::iterator found = cache.find(key);
    if (found == cache.end())
    {
        // Prune the map if it gets too big
        while (cache.size() > MAX_SURFACES)
        {
            SurfaceCache::iterator first = cache.begin();
            delete (*first).second;
            cache.erase(first);
        }
        surface = new SurfaceMesh(slices, stacks);
        cache[key] = surface;
    }
    else
    {
        surface = (*found).second;
    }

    Draw(surface);
}

void Surface::Draw(SurfaceMesh* surface)
// ----------------------------------------------------------------------------
//   Draw the specified surface
// ----------------------------------------------------------------------------
{
    glPushMatrix();
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_NORMALIZE);
    glTranslatef(center.x, center.y, 0.0);
    glScalef(width, height, 1.0);
    glNormal3f(0., 0., 1.);

    // Set vertex coordinates
    glVertexPointer(3, GL_DOUBLE, 0, &surface->vertices[0].x);
    glEnableClientState(GL_VERTEX_ARRAY);

    const ModuleApi *tao = WaterFactory::instance()->tao;
    tao->EnableTexCoords(&surface->textures[0].x, ~0UL);
    tao->SetTextures();

    GLuint size = stacks * slices * 4;

    // Set fill color defined in Tao
    if(tao->SetFillColor())
        glDrawElements(GL_QUADS, size, GL_UNSIGNED_INT, &surface->indices[0]);

    tao->DisableTexCoords(~0UL);

    glDisableClientState(GL_VERTEX_ARRAY);

    glPopAttrib();
    glPopMatrix();
}


Tree_p Surface::surface(Real_p x, Real_p y, Real_p w,
                        Real_p h, Integer_p lines_nb, Integer_p columns_nb)
// ----------------------------------------------------------------------------
//   Define a subdivded surface
// ----------------------------------------------------------------------------
{
    Surface* s = new Surface(x, y, w, h, lines_nb, columns_nb);
    WaterFactory::instance()->tao->addToLayout(Surface::render_callback,
                                               s, Surface::delete_callback);
    return XL::xl_true;
}

