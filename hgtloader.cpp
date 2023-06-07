/***************************************************************************
 *   Copyright (C) 2008 by Antonio Rueda   *
 *   ajrueda@ujaen.es   *
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
#include <fstream>

#include "hgtloader.h"

void HGTLoader::load(string file, vector<vector<int> >& elev) throw (FileNotFoundException)
{
    ifstream ifs(file);
    if (!ifs) throw FileNotFoundException();

    elev.clear();
    elev.reserve(1201);

    for (int f = 0; f < 1201; f++) {
        elev.push_back(vector<int>());
        elev[f].reserve(1201);

        for (int c = 0; c < 1201; c++) {
            elev[f].push_back((ifs.get() << 8) | ifs.get());
        }
    }
}

void HGTLoader::save(string file, vector<vector<int> >& elev) throw (FileOutputException)
{
    ofstream ofs(file);
    if (!ofs) throw FileOutputException();

    for (int f = 0; f < 1201; f++) {
        elev.push_back(vector<int>());
        elev[f].reserve(1201);

        for (int c = 0; c < 1201; c++) {
            ofs.put(elev[f][c] >> 8);
            ofs.put(elev[f][c] & 0xff);
        }
    }
}
