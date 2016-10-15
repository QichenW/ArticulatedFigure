//
// Created by Qichen on 10/14/16.
//

#ifndef ANIMATIONS_REVERSEKINEMATICS_H
#define ANIMATIONS_REVERSEKINEMATICS_H


#include "Part.h"

class ReverseKinematics {

public:
    static void setLocalRotation(Part *parts[5]);

    static void setLocalTranslation(Part *torso);
};


#endif //ANIMATIONS_REVERSEKINEMATICS_H
