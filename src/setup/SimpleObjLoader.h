//
// Created by Qichen on 9/21/16.
//

#ifndef GLUTPROJECT_SIMPLEOBJLOADER_H
#define GLUTPROJECT_SIMPLEOBJLOADER_H

// glut
#if defined(__APPLE__)

#include <GLUT/glut.h>
#include <string>
#include <vector>

#else
#include <GL/glut.h>
#endif

using namespace std;

class SimpleObjLoader
{
public:
    static GLuint loadObj(char *fileName, int objNo, float scale);

    static void recordObjectAsTrianglesWithNoVtNoVn();

    static void randomizeTheColor(GLfloat *des, int mode);
};

#endif //GLUTPROJECT_SIMPLEOBJHEADER_H
