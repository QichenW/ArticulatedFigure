//
// Created by Qichen on 9/26/16.
//

#ifndef GLUTPROJECT_INTERPOLATIONHELPER_H
#define GLUTPROJECT_INTERPOLATIONHELPER_H

#if defined(__APPLE__)
#include <GLUT/glut.h>
#include <string>
#include <vector>

#else
#include <GL/glut.h>
#endif
using namespace std;
class InterpolationHelper {

public:
    //TODO implement solutions for more than 4 points, now hard coded to allow only 4 key frames
    const static int NUMBER_OF_CONTROL_POINTS = 4;
    const static int QUATERNION_DIMENSION = 4;
    const static int POSITION_OR_EULER_DIMENSION = 3;
    static void calculate3dCoefficientMatrix(GLfloat (*dest)[3], int interpolationMode, float **controlPoints);

    static void prepareTimeVector(GLfloat *tVector, GLfloat t);

    static void prepareTranslationOrEulerAngleVector(GLfloat *transOrEuler, GLfloat *tVector, GLfloat coefficientMatrix[4][3]);

    static void prepareQuaternionVector(GLfloat quaternion[4], GLfloat tVector[4], GLfloat coefficientMatrix[4][4]);

    static void calculate4dCoefficientMatrix(GLfloat dest[4][4], int interpolationMode, GLfloat quaternionList[4][4]);
};


#endif //GLUTPROJECT_INTERPOLATIONHELPER_H
