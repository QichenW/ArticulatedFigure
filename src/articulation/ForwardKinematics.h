//
// Created by Qichen on 10/14/16.
//

#ifndef ANIMATIONS_REVERSEKINEMATICS_H
#define ANIMATIONS_REVERSEKINEMATICS_H


#include "Part.h"

class ForwardKinematics {

public:
    static void setLocalRotation(Part *parts[5], bool isKeyFraming);

    static void setLocalTranslation(Part *torso);
};


#endif //ANIMATIONS_REVERSEKINEMATICS_H
