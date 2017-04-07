//
// Created by Qichen Wang on 4/7/17.
//

#include "ArticulatedMan.h"

const GLfloat ArticulatedMan::TIME_INCREMENT = 0.008;
GLfloat ArticulatedMan::tVector[4] = {};

ArticulatedMan::ArticulatedMan() {
    isPlaying = false;
    resetPreferences();
}

/**
 * update time and the time vector for interpolation
 */
void ArticulatedMan::updateTimeVector() {
    InterpolationHelper::prepareTimeVector(tVector, timeProgress);
    timeProgress += TIME_INCREMENT;
}

/**
 * @return true, if the current interpolated sub-tour is not finished
 */
bool ArticulatedMan::isInterpolatingSubPath() {
    return timeProgress < 1.0;
}

/**
 * @return if all sub-tour has been interpolated
 */
bool ArticulatedMan::finishedWhole() {
    return currentSegment == curveSegmentAmount;
}

/**
 * reset the flag indicating that interpolating is done
 */
void ArticulatedMan::stopPlaying() {
    isPlaying = false;
}

// setters
void ArticulatedMan::setKeyFramesLoaded(bool value) {
    areKeyFramesLoaded = value;
}

void ArticulatedMan::setInterpolationMode(int mode) {
    interpolationMode = mode;
}

void ArticulatedMan::setOrientationMode(int mode) {
    orientationMode = mode;
}

// getters
bool ArticulatedMan::getAreKeyFramesLoaded() {
    return areKeyFramesLoaded;
}

int ArticulatedMan::getOrientationMode() {
    return orientationMode;
}

int ArticulatedMan::getInterpolationMode() {
    return interpolationMode;
}

void ArticulatedMan::resetPreferences() {
    areKeyFramesLoaded = false;
    interpolationMode = -1;
    orientationMode = -1;
    keyFrameAmount = 0;
    timeProgress = 0;
}


void ArticulatedMan::setKeyFrameAmount(int i) {
    keyFrameAmount = i;
    listOfPositions = new GLfloat *[i];
    listOfEulerAngle = new GLfloat *[i];
    //listOfQuaternion = new GLfloat*[i];

    pCoefficientMatrices = new CoefficientMatrices(orientationMode == 0, 0, keyFrameAmount - 3);
}

int ArticulatedMan::getKeyFrameAmount() const {
    return keyFrameAmount;
}

void ArticulatedMan::addOnePosition(int index, GLfloat x, GLfloat y, GLfloat z) {
    listOfPositions[index] = new GLfloat[3];
    listOfPositions[index][0] = x;
    listOfPositions[index][1] = y;
    listOfPositions[index][2] = z;
}

void ArticulatedMan::addOneEulerAngle(int index, GLfloat pitch, GLfloat yaw, GLfloat roll) {
    listOfEulerAngle[index] = new GLfloat[3];
    listOfEulerAngle[index][0] = pitch;
    listOfEulerAngle[index][1] = yaw;
    listOfEulerAngle[index][2] = roll;
}

/****
 * print out the infomation for debug purposes
 */
void ArticulatedMan::printLoadedPreferences() {
    cout << "number of key frames: " << keyFrameAmount << endl;
    cout << "orientation / interpolation: " << orientationMode << '/' << interpolationMode << endl;
    int i, j;
    float l;
    for (i = 0; i < keyFrameAmount; i++) {
        for (j = 0; j < 3; j++) {
            l = listOfPositions[i][j];
            cout << l << '\t';
        }
        cout << '\n';
    }
    cout << '\n';
    for (i = 0; i < keyFrameAmount; i++) {
        for (j = 0; j < 3; j++) {
            l = listOfEulerAngle[i][j];
            cout << l << '\t';
        }
        cout << '\n';
    }

}

/****
 *  get the coefficient matrices
 */
void ArticulatedMan::calculateCoefficientMatrices() {

    int i;
    currentCoefficientMatrices = pCoefficientMatrices;
    for (i = 0; i < keyFrameAmount - 3; i++) {

        // calculate the position coefficient matrix for t
        // then store in translationCoefficientMatrix
        InterpolationHelper::calculate3dCoefficientMatrix(currentCoefficientMatrices->translation,
                                                          interpolationMode, listOfPositions, i);

        if (orientationMode == 0) {
            // calculate the euler angle coefficient matrix for t
            // then store in translationCoefficientMatrix
            InterpolationHelper::calculate3dCoefficientMatrix(currentCoefficientMatrices->eRotation,
                                                              interpolationMode, listOfEulerAngle, i);
        } else {
            //convert the user-provided euler angles to quaternions
            QuaternionConverter::eulerAngleToQuaternion(listOfQuaternion, listOfEulerAngle, i);
            //TODO then get the quaternion version of coefficient matrix
            InterpolationHelper::calculate4dCoefficientMatrix(currentCoefficientMatrices->qRotation,
                                                              interpolationMode, listOfQuaternion);
        }
        currentCoefficientMatrices = currentCoefficientMatrices->next;
    }
}

void ArticulatedMan::resetTimeProgress() {
    timeProgress = 0;
}
