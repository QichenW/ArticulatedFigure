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
    void setLocalRotation(Part **parts, bool isStraightTrajectory);

    void setLocalTranslation(Part *torso);

    void calculateAllCoefficientMatrices();

private:
    const static int NUMBER_OF_CONTROL_POINT_FOR_CUBIC_SPLINE = 4;
    const static int NUMBER_OF_SEGMENT = 20;
    const static int NUMBER_OF_DOF_FOR_NON_TORSO = 8;
};


#endif //ANIMATIONS_KINEMATICS_H
