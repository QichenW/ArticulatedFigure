//
// Created by Qichen on 4/6/17.
//

#include <matrix/VectorCalculation.h>
#include "CameraMotion.h"
GLfloat CameraMotion::cameraPos[3] = {0, 0, 100};
GLfloat CameraMotion::cameraLookingAt[3] = {0, 0, -100};
GLfloat CameraMotion::cameraUp[3] = {0, 1, 0};
GLfloat CameraMotion::theta = 0;
GLfloat CameraMotion::disToLookPoint;
const GLfloat CameraMotion::ANGLE_INCREMENT = (const GLfloat) (M_PI / 36.0);
const GLfloat CameraMotion::ZOOM_INCREMENT = 10;
const GLfloat CameraMotion::PITCH_INCREMENT= 10;


/**
 * zoom in/out
 * @param isZoomIn - true for zoom in; false for zoom out
 */
void CameraMotion::zoom(bool isZoomIn) {
    GLfloat tempV[3] = {};
    for (int i = 0; i < 3; ++i) {
        tempV[i] = cameraLookingAt[i] - cameraPos[i];
    }
    VectorCalculation::getUnitDirection(tempV, tempV);
    for (int i = 0; i < 3; ++i) {
        if (isZoomIn) {
            cameraPos[i] += tempV[i] * ZOOM_INCREMENT;
        } else {
            cameraPos[i] -= tempV[i] * ZOOM_INCREMENT;
        }
    }
    if(isZoomIn){
        disToLookPoint -= ZOOM_INCREMENT;
    } else {
        disToLookPoint += ZOOM_INCREMENT;
    }
}

/**
 * move the camera, without changing the point of focus
 * @param isClockWise - true if move in the clockwise direction about the point of focus
 */
void CameraMotion::move(bool isClockWise) {
    if(isClockWise) {
        theta -= ANGLE_INCREMENT;
        if (theta <= -1 * M_PI) {
            theta += 2 * M_PI;
        }
    } else {
        theta += ANGLE_INCREMENT;
        if(theta >= M_PI){
            theta -= 2 * M_PI;
        }
    }
    cameraPos[0] = cameraLookingAt[0] + disToLookPoint * sinf(theta);
    cameraPos[2] = cameraLookingAt[2] + disToLookPoint * cosf(theta);

}

/**
 * prepare for camera motion
 */
void CameraMotion::prepare() {
    disToLookPoint = VectorCalculation::getDistance(cameraLookingAt, cameraPos);
}

/**
 * change the z direction of the camera
 * @param pitchUp - true if pitch up, false if pitch down
 */
void CameraMotion::pitch(bool pitchUp) {
    if(pitchUp){
        cameraLookingAt[1] += PITCH_INCREMENT;
    } else {
        cameraLookingAt[1] -= PITCH_INCREMENT;
    }
    disToLookPoint = VectorCalculation::getDistance(cameraLookingAt, cameraPos);
}
