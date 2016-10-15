//
// Created by Qichen on 10/14/16.
//

#include "ReverseKinematics.h"

void ReverseKinematics::setLocalRotation(Part **parts) {
    int i;
    for (i = 0; i < 5; i++){
        if (i ==0 ) {
            parts[i]->setLocalRotation((GLfloat [3]){0,0,0});
        }else
        parts[i]->setLocalRotation((GLfloat [3]){-60,0,0});
    }

}

void ReverseKinematics::setLocalTranslation(Part *torso) {

    torso->setLocalTranslation((GLfloat [3]){10,10,-10});
}
