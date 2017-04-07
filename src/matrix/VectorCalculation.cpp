//
// Created by Qichen Wang on 4/7/17.
//

#include "VectorCalculation.h"
/****
 * get the distance bwtween two 3-d positions
 */
GLfloat VectorCalculation::getDistance(GLfloat *pos1, GLfloat *pos2) {
    GLfloat result = 0;
    int i;
    for(i = 0; i < 3; i++){
        result += pow(*(pos1 + i) - *(pos2 + i),2);
    }
    return (GLfloat) sqrtf(result);
}

/***
 * used for getting acceleration caused by friction on groud
 * get the unit vector for a vector; if the vector is {0,0,0}, the unity vector is also {0,0,0}
 */
void VectorCalculation::getUnitDirection(GLfloat *des, GLfloat *src) {
    if(isZeroVector(src)){
        des[0] = des[1] = des[2] = 0;
    } else{
        GLfloat magnitude = sqrtf(powf(src[0],2) + powf(src[1],2) + powf(src[2],2));
        int i;
        for (i = 0; i < 3; i++){
            *(des + i) = *(src + i)/ magnitude;
        }
    }
}

/****
 * get the magnitude of a 3-d vector
 * @param vctor - the vector
 */
GLfloat VectorCalculation::getMagnitude(GLfloat *v) {
 return sqrtf((float) (pow(v[0], 2) + pow(v[1], 2)
                                             + pow(v[2], 2)));
}

/***
 * test if a velocity or acceleration is {0,0,0}
 */
bool VectorCalculation::isZeroVector(GLfloat *v) {
    int i;
    for(i = 0; i < 3; i++){
        if (*(v + i) != 0){
            return false;
        }
    }
    return true;
}