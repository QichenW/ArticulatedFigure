//
// Created by Qichen on 11/2/16.
//


#include <iostream>
#include "Kinematics.h"

float Kinematics::tInSegment;
GLfloat Kinematics::idleTorsoPos;
GLfloat Kinematics::idleSpeed;
int Kinematics::currentSegment;
GLfloat Kinematics::cubicTimeVector[4];
GLfloat Kinematics::currentRotation8[8];
GLfloat Kinematics::geometryMatrices[20][4][8];
GLfloat Kinematics::coefficientMatrices[20][4][8];
float const Kinematics::controlPoints_l_c[20];
float const Kinematics::controlPoints_l_f[20];
float const Kinematics::controlPoints_l_t[20];
float const Kinematics::controlPoints_l_u[20];
float const Kinematics::controlPoints_r_c[20];
float const Kinematics::controlPoints_r_f[20];
float const Kinematics::controlPoints_r_t[20];
float const Kinematics::controlPoints_r_u[20];
int const Kinematics::LINK_ID_TO_ROTATION_MAPPING[TOTAL_NUMBER_OF_LINKS];


void Kinematics::setLocalRotation(Part **parts, bool isStraightTrajectory) {

    InterpolationHelper::prepareTimeVector(cubicTimeVector, tInSegment);


    InterpolationHelper::prepare8dVector(currentRotation8, cubicTimeVector, coefficientMatrices[currentSegment]);

//TODO test it

    int i;

    for (i = 0; i < TOTAL_NUMBER_OF_LINKS; i++){
        if (i ==0 ) {
            if(isStraightTrajectory) {
                /* interpolated trajectory for torso is done in main.cpp */
                continue;
            }
            if (idleSpeed > 0){
                parts[i]->setLocalRotation((GLfloat [3]){0,270,0});
            } else {
                parts[i]->setLocalRotation((GLfloat [3]){0,90,0});
            }
        }else {
            if(LINK_ID_TO_ROTATION_MAPPING[i] < 0) {
                //if the link is not supposed to rotate
                parts[i]->setLocalRotation((GLfloat[3]) {0, 0, 0});
            } else {
                parts[i]->setLocalRotation((GLfloat[3]) {currentRotation8[LINK_ID_TO_ROTATION_MAPPING[i]], 0, 0});
            }
        }
    }

    tInSegment += deltaT;
    if(tInSegment >= 1.0) {
        // if one segment ends, "modulus" tInSegment for next segment
        tInSegment -= 1.0;
        currentSegment += 1;
        if(currentSegment == NUMBER_OF_SEGMENT - 1) {
            // go back to the first segment
            currentSegment = 0;
        }
    }
}

/**
 * used only when displaying an idle walking-in-straight-line animation
 * @param torso
 */
void Kinematics::setLocalTranslation(Part *torso) {
    if (idleTorsoPos <= -51 || idleTorsoPos >= 51) {
        idleSpeed *= -1;
    }
    torso->setLocalTranslation((GLfloat [3]){idleTorsoPos+=idleSpeed,0,-50});
}


/**
 * calculated all coeffieient matrices for a cycle of two steps forward,
 * i.e. left leg fully streched then right leg fully streched
 * then store in coefficientMatrices[20][4][8] (there are 20 segments for one cycle)
 */

void Kinematics::calculateAllCoefficientMatrices() {

    initiate();

    float controlPointArray8d[NUMBER_OF_DOF_FOR_NON_TORSO][NUMBER_OF_SEGMENT] = {};
    int i,j,k;
    for ( i = 0; i < NUMBER_OF_DOF_FOR_NON_TORSO; i++) {
        for(j = 0; j < NUMBER_OF_SEGMENT; j++){
            switch (i) {
                case 0:
                    controlPointArray8d[i][j] = controlPoints_l_t[j];
                    break;
                case 1:
                    controlPointArray8d[i][j] = controlPoints_r_t[j];
                    break;
                case 2:
                    controlPointArray8d[i][j] = controlPoints_l_c[j];
                    break;
                case 3:
                    controlPointArray8d[i][j] = controlPoints_r_c[j];
                    break;
                case 4:
                    controlPointArray8d[i][j] = controlPoints_l_u[j];
                    break;
                case 5:
                    controlPointArray8d[i][j] = controlPoints_r_u[j];
                    break;
                case 6:
                    controlPointArray8d[i][j] = controlPoints_l_f[j];
                    break;
                case 7:
                    controlPointArray8d[i][j] = controlPoints_r_f[j];
                    break;
                default:
                    break;
            }
        }
    }
    // first prepare geometryMatrices GLfloat geometryMatrices[20][4][8] = {} TODO test this
    for (k = 0; k < NUMBER_OF_SEGMENT; k++) {
        //TODO test
        cout<<"\n";
        for (i = k; i < k + NUMBER_OF_CONTROL_POINT_FOR_CUBIC_SPLINE; i++) {
            cout<<"\n";
            for (j = 0; j < NUMBER_OF_DOF_FOR_NON_TORSO; j++) {
                geometryMatrices[k][i - k][j] =
                        (i < NUMBER_OF_SEGMENT)
                        ? controlPointArray8d[j][i] : controlPointArray8d[j][i % NUMBER_OF_SEGMENT];

                cout<<geometryMatrices[k][i - k][j]<<"\t";
            }
        }
        cout<<"\n";
    }

    // then prepare all coefficient matrices GLfloat coefficientMatrices[20][4][8] = {} TODO test this
    for (i = 0; i < NUMBER_OF_SEGMENT; i++) {
        InterpolationHelper::calculate8dCoefficientMatrix(
                coefficientMatrices[i], 0, geometryMatrices[i]);
    }


}

void Kinematics::initiate() {

    // for idle straight line walking only
    idleTorsoPos = 50;
    idleSpeed = (GLfloat) -0.1;

    geometryMatrices[20][4][8] = {};
    coefficientMatrices[20][4][8] = {};
    currentRotation8[8] = {};

    tInSegment = 0;
    cubicTimeVector[4] = {};
}
