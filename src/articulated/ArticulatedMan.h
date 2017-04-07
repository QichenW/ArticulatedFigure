//
// Created by Qichen Wang on 4/7/17.
//

#ifndef ANIMATIONS_ARTICULATEDMAN_H
#define ANIMATIONS_ARTICULATEDMAN_H

#include <iostream>
#include <string>
#include "InterpolationHelper.h"
#include "QuaternionConverter.h"
#include "Part.h"
#include <matrix/CoefficientMatrices.h>

#if defined(__APPLE__)

#include <GLUT/glut.h>

#else
#include <GL/glut.h>
#endif
using namespace std;

class ArticulatedMan {
public:
    ArticulatedMan();

    bool isPlaying;
    static GLfloat tVector[4];
    Part *parts[15];
    GLfloat quaternion[4] = {}, eulerAngle[3] = {}, translation[3] = {};
    int curveSegmentAmount, currentSegment;

    void updateTimeVector();

    bool isInterpolatingSubPath();

    bool finishedWhole();

    void stopPlaying();
    /****
 * mode:
 * // orientationMode    -1: not chosen, 0 : euler angle, 1 : quaternion
 * // interpolationMode  -1: not chosen, 0 : Catmull-Rom, 1 : B-spline
 */
private:
    static const GLfloat TIME_INCREMENT;

    int interpolationMode, orientationMode, keyFrameAmount;
    GLfloat **listOfPositions, **listOfEulerAngle, listOfQuaternion[4][4];

public:
    CoefficientMatrices *pCoefficientMatrices;
    CoefficientMatrices *currentCoefficientMatrices;

    GLfloat timeProgress;
    bool areKeyFramesLoaded;

    void setInterpolationMode(int mode);

    void setOrientationMode(int mode);

    void resetPreferences();

    void setKeyFrameAmount(int i);


    void addOnePosition(int index, GLfloat x, GLfloat y, GLfloat z);

    void addOneEulerAngle(int index, GLfloat pitch, GLfloat yaw, GLfloat roll);

    void printLoadedPreferences();

    void resetTimeProgress();

    void calculateCoefficientMatrices();

    int getKeyFrameAmount() const;

    int getInterpolationMode();

    int getOrientationMode();

    bool getAreKeyFramesLoaded();

    void setKeyFramesLoaded(bool value);
};


#endif //ANIMATIONS_ARTICULATEDMAN_H
