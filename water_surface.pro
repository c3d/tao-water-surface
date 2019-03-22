# ******************************************************************************
# water_surface.pro                                                Tao3D project
# ******************************************************************************
#
# File description:
# Qt build file for the Water surface module
#
#
#
#
#
#
#
# ******************************************************************************
# This software is licensed under the GNU General Public License v3
# (C) 2012-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
# (C) 2013, Catherine Burvelle <catherine@taodyne.com>
# (C) 2012-2015,2019, Christophe de Dinechin <christophe@dinechin.org>
# (C) 2012-2013, Jérôme Forissier <jerome@taodyne.com>
# (C) 2012-2013, Baptiste Soulisse <baptiste.soulisse@taodyne.com>
# ******************************************************************************
# This file is part of Tao3D
#
# Tao3D is free software: you can r redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Tao3D is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Tao3D, in a file named COPYING.
# If not, see <https://www.gnu.org/licenses/>.
# ******************************************************************************

MODINSTDIR = water_surface

include(../modules.pri)

INCLUDEPATH += $${TAOTOPSRC}/tao/include/tao/
HEADERS = \
          water.h \
    water_factory.h

SOURCES = water.cpp \
    water_factory.cpp

TBL_SOURCES  = water_surface.tbl

OTHER_FILES = traces.tbl \
    water_surface.xl \
    water_surface.tbl
QT          += core \
               gui \
               opengl

# Icon from http://www.iconarchive.com/show/simple-green-icons-by-simplefly/water-sea-icon.html
INSTALLS    += thismod_icon

QMAKE_SUBSTITUTES = doc/Doxyfile.in
DOXYFILE = doc/Doxyfile
DOXYLANG = en,fr
include(../modules_doc.pri)


















