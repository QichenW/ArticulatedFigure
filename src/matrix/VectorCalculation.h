//
// Created by Qichen Wang on 4/7/17.
//

#ifndef ANIMATIONS_VECTORCALCULATION_H
#define ANIMATIONS_VECTORCALCULATION_H

#if defined(__APPLE__)
#include <GLUT/glut.h>
#include <string>
#include <math.h>
#include <matrix/CoefficientMatrices.h>

#else
#include <GL/glut.h>
#endif

class VectorCalculation {
public:
    static GLfloat getDistance(GLfloat *pos1, GLfloat *pos2);

    static void getUnitDirection(GLfloat *des, GLfloat *src);

    static GLfloat getMagnitude(GLfloat *v);

    static bool isZeroVector(GLfloat *v);
};


#endif //ANIMATIONS_VECTORCALCULATION_H
