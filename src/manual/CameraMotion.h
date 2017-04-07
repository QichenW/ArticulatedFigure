//
// Created by Qichen on 4/6/17.
//

#ifndef ANIMATIONS_CAMERAMOTION_H
#define ANIMATIONS_CAMERAMOTION_H

#if defined(__APPLE__)
#include <GLUT/glut.h>
#include <string>
#include <math.h>
#include <matrix/CoefficientMatrices.h>

#else
#include <GL/glut.h>
#endif

class CameraMotion {
public:
    static GLfloat cameraPos[3], cameraLookingAt[3], cameraUp[3];

    static void zoom(bool isZoomIn);

    static void move(bool isClockWise);

    static void prepare();

    static void pitch(bool pitchUp);

private:
    static GLfloat theta, disToLookPoint;
    static const GLfloat ANGLE_INCREMENT, PITCH_INCREMENT;
    static const GLfloat ZOOM_INCREMENT;
};


#endif //ANIMATIONS_CAMERAMOTION_H
