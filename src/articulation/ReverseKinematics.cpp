//
// Created by Qichen on 10/14/16.
//

#include "ReverseKinematics.h"



GLfloat speed = -0.1;
GLfloat torsoPos = 100;

GLfloat deltaLeftAngleThigh[4] = {1,0,-1,0};
GLfloat deltaRightAngleThigh[4] = {-1,0,1,0};
GLfloat deltaAngleLeftCalf[4] = {-1,1,0,0};
GLfloat deltaAngleRightCalf[4] = {0,0,-1,1};
int phase = 0;
GLfloat xAngles[5] ={0,-30,30,0,0};
void ReverseKinematics::setLocalRotation(Part **parts) {
    int i;
    if (phase == 0 && xAngles[1] >= 30) {
        phase = 1;
    } else if (phase == 1 && xAngles[3] >=0){
        phase = 2;
    } else if (phase == 2 &&  xAngles[1] <= -30){
        phase = 3;
    }else if (phase == 3 && xAngles[4] >=0){
        phase = 0;
    }
    xAngles[1] += deltaLeftAngleThigh[phase];
    xAngles[2] += deltaRightAngleThigh[phase];
    xAngles[3] += deltaAngleLeftCalf[phase];
    xAngles[4] += deltaAngleRightCalf[phase];



    for (i = 0; i < 5; i++){
        if (i ==0 ) {
            parts[i]->setLocalRotation((GLfloat [3]){0,90,0});
        }else
        parts[i]->setLocalRotation((GLfloat [3]){xAngles[i],0,0});
    }

}

void ReverseKinematics::setLocalTranslation(Part *torso) {

    torso->setLocalTranslation((GLfloat [3]){torsoPos+=speed,0,-50});
}
