//
// Created by Qichen on 11/2/16.
//

#ifndef ANIMATIONS_DRAWLINKS_H
#define ANIMATIONS_DRAWLINKS_H

#include "SimpleObjLoader.h"
#include "setup/Preferences.h"
#include "articulation/Part.h"

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class DrawLinks {

public:
    static void drawLinks(Part **links);

};


#endif //ANIMATIONS_DRAWLINKS_H
