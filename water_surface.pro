# ******************************************************************************
#  water_surface.pro                                                 Tao project
# ******************************************************************************
# File Description:
# Qt build file for the Water surface module
# ******************************************************************************
# This software is property of Taodyne SAS - Confidential
# Ce logiciel est la propriété de Taodyne SAS - Confidentiel
# (C) 2010 Jerome Forissier <jerome@taodyne.com>
# (C) 2010 Taodyne SAS
# ******************************************************************************

MODINSTDIR = water_surface

include(../modules.pri)

INCLUDEPATH += $${TAOTOPSRC}/tao/include/tao/
HEADERS = \
          water.h \
          surface.h \
    water_factory.h

SOURCES = water.cpp \
          surface.cpp \
    water_factory.cpp

win32 {
  DEFINES     += GLEW_STATIC
  SOURCES     += $${TAOTOPSRC}/tao/include/tao/GL/glew.c
}

TBL_SOURCES  = water_surface.tbl

OTHER_FILES = traces.tbl \
    water_surface2.xl \
    water_surface.xl \
    water_surface.tbl
QT          += core \
               gui \
               opengl

# Icon from http://www.iconarchive.com/show/simple-green-icons-by-simplefly/water-sea-icon.html
INSTALLS    += thismod_icon

CRYPT_XL_SOURCES = water_surface2.xl
include(../crypt_xl.pri)

QMAKE_SUBSTITUTES = doc/Doxyfile.in
DOXYFILE = doc/Doxyfile
DOXYLANG = en,fr
include(../modules_doc.pri)


















