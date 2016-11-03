//
// Created by Qichen on 11/2/16.
//

#include "Part.h"
#include "matrix/InterpolationHelper.h"
#include <vector>

#ifndef ANIMATIONS_KINEMATICS_H
#define ANIMATIONS_KINEMATICS_H


class Kinematics {
public:
    static void setLocalRotation(Part **parts, bool isStraightTrajectory);

    static void setLocalTranslation(Part *torso);

    static void calculateAllCoefficientMatrices();

private:
    const static int NUMBER_OF_CONTROL_POINT_FOR_CUBIC_SPLINE = 4;
    const static int NUMBER_OF_SEGMENT = 20;
    const static int NUMBER_OF_DOF_FOR_NON_TORSO = 8;
    const static int TOTAL_NUMBER_OF_LINKS = 15; // including torso and non-rotational links
    static GLfloat geometryMatrices[20][4][8];
    static GLfloat coefficientMatrices[20][4][8];
    static GLfloat currentRotation8[8];
    static GLfloat cubicTimeVector[4];

    // t parameter for interpolation and its incremental interval
    static constexpr float deltaT =  0.1; //TODO write the reasoning for this
    static GLfloat tInSegment;

    // identifier of current curve for curve concatenation
    static int currentSegment;

    // the mapping from parts[15] indices to the 8-d rotation vector, -1 means non-rotational
    static constexpr int LINK_ID_TO_ROTATION_MAPPING[TOTAL_NUMBER_OF_LINKS] = {
            -1, 0, 1, 2, 3, -1, -1, -1, -1, 4, 5, 6, 7, -1, -1};



    // for idle straight line walking only
    static GLfloat idleTorsoPos;
    static float idleSpeed;

    // the control points for left/right calfs, thighs, upper-arms, forearms and neck
    //each cycle has 20 control points
    // left thigh's rotation

    float static constexpr controlPoints_l_t[20] =   {  -28.53, -30.00, -28.53, -24.27, -17.63, -9.27, 0,
                                              9.27, 17.63, 24.27, 28.53, 30.00, 28.53, 24.27,
                                              17.63, 9.27, 0.00, -9.27, -17.63, -24.27};
    // right thigh's rotation
    float static constexpr controlPoints_r_t[20] =   {   28.53, 30.00, 28.53, 24.27, 17.63, 9.27, 0.00,
                                               -9.27, -17.63, -24.27, -28.53, -30.00, -28.53, -24.27,
                                               -17.63, -9.27, 0, 9.27, 17.63, 24.27};

    // left calf's rotation
    float static constexpr controlPoints_l_c[20] =   {         0, 0, -5.73, -20.73, -39.27, -54.27,
                                                     -60.00, -54.27, -39.27, -20.73, -5.73,
                                                     0, 0, 0, 0, 0, 0, 0, 0, 0};
    // right calf's rotation
    float static constexpr controlPoints_r_c[20] =   {-5.73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                            0, -5.73, -20.73, -39.27, -54.27,
                                            -60.00, -54.27, -39.27, -20.73};
    //TODO arms control points
    // left upper arm's rotation
    float static constexpr controlPoints_l_u[20] = {   28.53, 30.00, 28.53, 24.27, 17.63, 9.27, 0.00,
                                                       -9.27, -17.63, -24.27, -28.53, -30.00, -28.53, -24.27,
                                                       -17.63, -9.27, 0, 9.27, 17.63, 24.27};
    // right upper arm's rotation
    float static constexpr controlPoints_r_u[20] = {  -28.53, -30.00, -28.53, -24.27, -17.63, -9.27, 0,
                                                      9.27, 17.63, 24.27, 28.53, 30.00, 28.53, 24.27,
                                                      17.63, 9.27, 0.00, -9.27, -17.63, -24.27};
    // left forearm's rotation
    float static constexpr controlPoints_l_f[20] = {5.73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                    0, 5.73, 20.73, 39.27, 54.27,
                                                    60.00, 54.27, 39.27, 20.73};
    // right forearm's rotation
    float static constexpr controlPoints_r_f[20] = {         0, 0, 5.73, 20.73, 39.27, 54.27,
                                                             60.00, 54.27, 39.27, 20.73, 5.73,
                                                             0, 0, 0, 0, 0, 0, 0, 0, 0};

    static void initiate();
};


#endif //ANIMATIONS_KINEMATICS_H
