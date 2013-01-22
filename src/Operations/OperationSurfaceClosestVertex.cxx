/*LICENSE_START*/
/*
 *  Copyright 1995-2002 Washington University School of Medicine
 *
 *  http://brainmap.wustl.edu
 *
 *  This file is part of CARET.
 *
 *  CARET is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  CARET is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with CARET; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "OperationSurfaceClosestVertex.h"
#include "OperationException.h"

#include "SurfaceFile.h"

#include <fstream>

using namespace caret;
using namespace std;

AString OperationSurfaceClosestVertex::getCommandSwitch()
{
    return "-surface-closest-vertex";
}

AString OperationSurfaceClosestVertex::getShortDescription()
{
    return "FIND CLOSEST SURFACE VERTEX TO COORDINATES";
}

OperationParameters* OperationSurfaceClosestVertex::getParameters()
{
    OperationParameters* ret = new OperationParameters();
    ret->addSurfaceParameter(1, "surface", "the surface to use");
    ret->addStringParameter(2, "coord-list-file", "text file with coordinates");
    ret->addStringParameter(3, "vertex-list-out", "output - the output text file with vertex numbers");//HACK: we don't currently have an "output text file" parameter type, fake the formatting
    ret->setHelpText(
        AString("For each coordinate XYZ triple, find the closest vertex in the surface, and output its vertex number into a text file.")
    );
    return ret;
}

void OperationSurfaceClosestVertex::useParameters(OperationParameters* myParams, ProgressObject* myProgObj)
{
    LevelProgress myProgress(myProgObj);
    SurfaceFile* mySurf = myParams->getSurface(1);
    AString coordFileName = myParams->getString(2);
    fstream coordFile(coordFileName.toLocal8Bit().constData(), fstream::in);
    if (!coordFile.good())
    {
        throw OperationException("error opening coordinate list file for reading");
    }
    AString nodeFileName = myParams->getString(3);
    fstream nodeFile(nodeFileName.toLocal8Bit().constData(), fstream::out);
    if (!nodeFile.good())
    {
        throw OperationException("error opening output file for writing");
    }
    vector<float> coords;
    float x, y, z;
    while (coordFile >> x >> y >> z)//yes, really, thats how they intended it to be used, more or less
    {
        coords.push_back(x);
        coords.push_back(y);
        coords.push_back(z);
    }
    for (int i = 0; i < (int)coords.size(); i += 3)
    {
        int node = mySurf->closestNode(coords.data() + i);
        nodeFile << node << endl;
    }
}
