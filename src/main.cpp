//  Created by Qichen on 9/17/16.
// As required opengl toolkit's api about any matrix and vector related
// calculations are not used. The author implemented them.
//  Copyright © 2016 Qichen Wang. All rights reserved.
//

#include <RotationHelper.h>
#include <UserInputManager.h>
#include <StringUtils.h>
#include <matrix/InterpolationHelper.h>
#include <articulation/Part.h>
#include <articulation/ForwardKinematics.h>
#include <articulation/DrawLinks.h>

using namespace std;

Part *parts[15];
Preferences prefs;
int curveSegmentAmount, currentSegment;
GLfloat increment = 0.008;

GLfloat tVector[4]={}, quaternion[4] = {}, eulerAngle[3] = {}, translation[3] = {};
int window;

void drawFrame();


void drawLinks(bool);

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat) w / (GLfloat) h, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

/***
 * display the figure when idle
 */
void displayObject() {

    glLoadIdentity();
    glColor3f(0.1, 0.45, 0.1);
    glMatrixMode(GL_MODELVIEW);
    //move the model view away from the camera, so that we are not inside the object1
    glMultMatrixf((GLfloat []){1,0,0,0,0,1,0,0,0,0,1,0,0,10,-20,1});

    //TODO insert real local rotation and translation
    //only the local translation of torso change
    ForwardKinematics::setLocalTranslation(parts[0]);
    ForwardKinematics::setLocalRotation(parts, false);

    drawLinks(false);
}

/***
 * generate the transformation matrices for every link (torso, tighs, calfs) w.r.t. world space, then draw them
 */
void drawLinks(bool isKeyFraming) {
    int i;
    GLfloat * combinedTransformation;
    for (i = 0; i < 15; i++) {
        combinedTransformation = (GLfloat [16]){1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
        glPushMatrix();

        if (parts[i]->parent != nullptr) {
            //finally, go with parent
            RotationHelper::rightDotProduct(combinedTransformation, parts[i]->parent->combinedTransformation);
            //third, move the part to the appropriate position w.r.t parent
            RotationHelper::rightDotProduct(combinedTransformation, parts[i]->secondAlignFlatMatrix);
            //second, local rotate
            RotationHelper::rightDotProduct(combinedTransformation,
                                            RotationHelper::generateFlattenedTransformationMatrix(
                                                    parts[i]->localRotation, nullptr, false));
            //first, move w.r.t parent to align axis
            RotationHelper::rightDotProduct(combinedTransformation, parts[i]->firstAlignFlatMatrix);
            parts[i]->setCombinedTransitions(combinedTransformation);

        } else {
            if (!isKeyFraming) {
                combinedTransformation = RotationHelper::generateFlattenedTransformationMatrix(
                        parts[i]->localRotation, parts[i]->localTranslation, false);
                parts[i]->setCombinedTransitions(combinedTransformation);
            } else{
                //TODO if (isKeyFraming)
                combinedTransformation = RotationHelper::
                generateFlattenedTransformationMatrix(quaternion, translation, true);
                parts[i]->setCombinedTransitions(combinedTransformation);
            }

        }
        glMultMatrixf(combinedTransformation);
        glCallList(parts[i]->objListID);
        glPopMatrix();
    }
}

/****
 * This function is for drawing the frames in the interpolated animation.
 */
void drawFrame() {
    glLoadIdentity();
    glPushMatrix();
    //move the model view away from the camera, so that we are not inside the object
    glMultMatrixf((GLfloat []){1,0,0,0,0,1,0,0,0,0,1,0,0,10,-95,1});
    glColor3f(0.1, 0.45, 0.1);

    // prepare the T vector for current time, then increment time.
    if(prefs.getTimeProgress()< 1.0){
        InterpolationHelper::prepareTimeVector(tVector, prefs.getTimeProgress());
        prefs.timeProceed(increment);
    } else if (currentSegment <= curveSegmentAmount) {
        prefs.currentCoefficientMatrices->printCurrentCoefficientMatrices();
        if (currentSegment == curveSegmentAmount) {
            glPopMatrix();
            prefs.setIsPlaying(false);
            return;
        }
        //TODO figure out what causes the minor pause
        currentSegment++;
        prefs.resetTimeProgress();
        prefs.currentCoefficientMatrices = prefs.currentCoefficientMatrices->next;
        // move and rotate the figure
        ForwardKinematics::setLocalRotation(parts, true);
        drawLinks(true);
        glPopMatrix();
        return;
    }

    // prepare the translation vector
    InterpolationHelper::
    prepareTranslationOrEulerAngleVector(translation, tVector, prefs.currentCoefficientMatrices->translation);

    if (prefs.getOrientationMode() == 0) {
        // if getting euler angle version of animation
        // prepare the euler angle vector
        InterpolationHelper::
        prepareTranslationOrEulerAngleVector(eulerAngle, tVector, prefs.currentCoefficientMatrices->eRotation);
        // move the figure
        glMultMatrixf(RotationHelper::generateFlattenedTransformationMatrix(eulerAngle, translation, false));
    } else {
        // if getting quaternion version of animation
        // prepare the quaternion vector
        InterpolationHelper::prepareQuaternionVector(quaternion, tVector, prefs.currentCoefficientMatrices->qRotation);
        // move and rotate the object1
        ForwardKinematics::setLocalRotation(parts, true);
        drawLinks(true);
    }
    glPopMatrix();
}

void display(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // the setup info char * on the bottom left corner on window when it is idle,
    // or the type of animation when playing animation
    UserInterfaceManager::renderStatusMessage(
            prefs.getOrientationMode(), prefs.getInterpolationMode(), prefs.getIsPlaying());
    if(!prefs.getIsPlaying()){
        displayObject();
        //TODO the following two line is bad practice
        currentSegment = 1;
        prefs.currentCoefficientMatrices = prefs.pCoefficientMatrices;
    } else {
        curveSegmentAmount = prefs.getKeyFrameAmount() - 3;
        drawFrame();
    }
    glutSwapBuffers(); //swap the buffers

}
/**
 * openGL works in a right-handed coordinate system by default
 * **/
int main(int argc, char **argv) {
    // initiate an instance of prefs to store the user preference
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(100, 100);
    window = glutCreateWindow("CSCI 6555 project 2 : Articulated Figure");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    UserInputManager(&window, &prefs);
    glutKeyboardFunc(UserInputManager::keyboardFunc);
    glutMouseFunc(UserInputManager::mouseFunc);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //load the obj files of parts, and create the drawing list
    DrawLinks::drawLinks(parts);

    UserInputManager::createMouseMenu();
    glutMainLoop();
    return 0;
}