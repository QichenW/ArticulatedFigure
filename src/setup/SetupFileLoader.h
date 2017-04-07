//
// Created by Qichen on 9/25/16.
//

#ifndef GLUTPROJECT_SETUPFILELOADER_H
#define GLUTPROJECT_SETUPFILELOADER_H


#include <string>
#include <articulated/ArticulatedMan.h>

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class SetupFileLoader {
public:
    static void loadPreferencesFromTextFile(char *path, ArticulatedMan *pPreferences);
};


#endif //GLUTPROJECT_SETUPFILELOADER_H
