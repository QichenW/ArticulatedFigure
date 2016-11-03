////
//// Created by Qichen on 11/2/16.
////
//
//
//#include "Kinematics.h"
//
//
//
//int currentSegment = 0;
////TODO write the reasoning for this
//GLfloat deltaT = (GLfloat) 0.1;
//GLfloat tInSegment = 0;
//
//GLfloat geometryMatrices[20][4][8] = {};
//GLfloat coefficientMatrices[20][4][8] = {};
//GLfloat currentRotation[8] = {};
//
//GLfloat current8dCoefficientMatrix[4][8] = {};
//GLfloat cubicTimeVector[4] = {};
//
//GLfloat deltaLeftAngleThigh[4] = {0.5,0.5,-0.5,-0.5};
//GLfloat deltaRightAngleThigh[4] = {-0.5,-0.5,0.5,0.5};
//GLfloat deltaAngleLeftCalf[4] = {-1,1,0,0};
//GLfloat deltaAngleRightCalf[4] = {0,0,-1,1};
//
//GLfloat torsoPos = 50;
//GLfloat speed = (GLfloat) -0.1;
//int phase = 0;
//// the control points for left/right calfs, thighs, upper-arms, forearms and neck
////each cycle has 20 control points
//// left thigh's rotation
//
//float static controlPoints_l_t[20] =   {  -28.53, -30.00, -28.53, -24.27, -17.63, -9.27, 0,
//                                          9.27, 17.63, 24.27, 28.53, 30.00, 28.53, 24.27,
//                                          17.63, 9.27, 0.00, -9.27, -17.63, -24.27};
//// right thigh's rotation
//float static controlPoints_r_t[20] =   {   28.53, 30.00, 28.53, 24.27, 17.63, 9.27, 0.00,
//                                           -9.27, -17.63, -24.27, -28.53, -30.00, -28.53, -24.27,
//                                           -17.63, -9.27, 0, 9.27, 17.63, 24.27};
//
//// left calf's rotation
//float static controlPoints_l_c[20] =   {         0, 0, -5.73, -20.73, -39.27, -54.27,
//                                                 -60.00, -54.27, -39.27, -20.73, -5.73,
//                                                 0, 0, 0, 0, 0, 0, 0, 0, 0};
//// right calf's rotation
//float static controlPoints_r_c[20] =   {-5.73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//                                        0, -5.73, -20.73, -39.27, -54.27,
//                                        -60.00, -54.27, -39.27, -20.73};
////TODO arms control points
//// left upper arm's rotation
//float static controlPoints_l_u[20] = {};
//// right upper arm's rotation
//float static controlPoints_r_u[20] = {};
//// left forearm's rotation
//float static controlPoints_l_f[20] = {};
//// right forearm's rotation
//float static controlPoints_r_f[20] = {};
//
//float currentControlPointForCubicSpline[4][8];
//
//GLfloat xAngles[15] ={0,-30,30,0,0,0,0,0,0,0,0,0,0,0,0};
//
//void Kinematics::setLocalRotation(Part **parts, bool isStraightTrajectory) {
////TODO
//    InterpolationHelper::prepareTimeVector(cubicTimeVector, tInSegment);
//    tInSegment += deltaT;
//    if(tInSegment >= 1) {
//        // if one segment ends, "modulus" tInSegment for next segment
//        tInSegment -= 1;
//        currentSegment += 1;
//        if(currentSegment == NUMBER_OF_SEGMENT) {
//            // go back to the first segment
//            currentSegment -= 0;
//        }
//    }
//
//    InterpolationHelper::prepare8dVector(currentRotation, cubicTimeVector, coefficientMatrices[currentSegment]);
//
//
//
//
//
//    int i;
//    if (phase == 0 && xAngles[1] >= 0) {
//        phase = 1;
//    } else if (phase == 1 && xAngles[3] >=0){
//        phase = 2;
//    } else if (phase == 2 &&  xAngles[1] <= 0){
//        phase = 3;
//    }else if (phase == 3 && xAngles[4] >=0){
//        phase = 0;
//    }
//    xAngles[1] += deltaLeftAngleThigh[phase];
//    xAngles[2] += deltaRightAngleThigh[phase];
//    xAngles[3] += deltaAngleLeftCalf[phase];
//    xAngles[4] += deltaAngleRightCalf[phase];
//
//    for (i = 0; i < 15; i++){
//        if (i ==0 ) {
//            if(isStraightTrajectory) {
//                /* interpolated trajectory for torso is done in main.cpp */
//                continue;
//            }
//            if (speed > 0){
//                parts[i]->setLocalRotation((GLfloat [3]){0,270,0});
//            } else {
//                parts[i]->setLocalRotation((GLfloat [3]){0,90,0});
//            }
//        }else {
//            parts[i]->setLocalRotation((GLfloat[3]) {xAngles[i], 0, 0});
//        }
//    }
//
//}
//
///**
// * used when displaying a walking-in-straight-line animation
// * @param torso
// */
//void Kinematics::setLocalTranslation(Part *torso) {
//    if (torsoPos <= -51 || torsoPos >= 51) {
//        speed *= -1;
//    }
//    torso->setLocalTranslation((GLfloat [3]){torsoPos+=speed,0,-50});
//}
//
//
//
//
//void static Kinematics::calculateAllCoefficientMatrices() {
//    float controlPointArray8d[NUMBER_OF_DOF_FOR_NON_TORSO][NUMBER_OF_SEGMENT] = {};
//    int i,j,k;
//    for ( i = 0; i < NUMBER_OF_DOF_FOR_NON_TORSO; i++) {
//        for(j = 0; j < NUMBER_OF_SEGMENT; j++){
//            switch (i) {
//                case 0:
//                    controlPointArray8d[i][j] = controlPoints_l_t[j];
//                    break;
//                case 1:
//                    controlPointArray8d[i][j] = controlPoints_r_t[j];
//                    break;
//                case 2:
//                    controlPointArray8d[i][j] = controlPoints_l_c[j];
//                    break;
//                case 3:
//                    controlPointArray8d[i][j] = controlPoints_r_c[j];
//                    break;
//                case 4:
//                    controlPointArray8d[i][j] = controlPoints_l_u[j];
//                    break;
//                case 5:
//                    controlPointArray8d[i][j] = controlPoints_r_u[j];
//                    break;
//                case 6:
//                    controlPointArray8d[i][j] = controlPoints_l_f[j];
//                    break;
//                case 7:
//                    controlPointArray8d[i][j] = controlPoints_r_f[j];
//                    break;
//                default:
//                    break;
//            }
//        }
//    }
//    // first prepare geometryMatrices GLfloat geometryMatrices[20][4][8] = {} TODO test this
//    for (k = 0; k < NUMBER_OF_SEGMENT; k++) {
//        for (i = k; i < k + NUMBER_OF_CONTROL_POINT_FOR_CUBIC_SPLINE; i++) {
//            for (j = 0; j < NUMBER_OF_DOF_FOR_NON_TORSO; j++) {
//                geometryMatrices[k][i - k][j] =
//                        (i < NUMBER_OF_SEGMENT)
//                        ? controlPointArray8d[j][i] : controlPointArray8d[j][i % NUMBER_OF_SEGMENT];
//            }
//        }
//    }
//
//    // then prepare all coefficient matrices GLfloat coefficientMatrices[20][4][8] = {} TODO test this
//    for (i = 0; i < NUMBER_OF_SEGMENT; i++) {
//        InterpolationHelper::calculate8dCoefficientMatrix(
//                coefficientMatrices[i], 0, geometryMatrices[i]);
//    }
//}