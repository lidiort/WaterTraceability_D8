/***************************************************************************
 *   Copyright (C) 2007 by Lidia Ortega   *
 *   lidia@ujaen.es   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "mates.h"


float AreaTriangulo2(float ax, float ay, float bx, float by, float cx, float cy) {
    return (ax * by - ay * bx +
        bx * cy - by * cx +
        cx * ay - cy * ax);
}

bool Izquierda(float ax, float ay, float bx, float by, float cx, float cy) {
    return (AreaTriangulo2(ax, ay, bx, by, cx, cy) > 0);
}

bool esInfinito(float a) {
    if (a > 8e9) return true;
    return  false;
}