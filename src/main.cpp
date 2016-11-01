//  Created by Qichen on 9/17/16.
// As required opengl toolkit's api about any matrix and vector related
// calculations are not used. The author implemented them.
//  Copyright © 2016 Qichen Wang. All rights reserved.
//

#include <GL/glew.h>
#include <RotationHelper.h>
#include <UserInputManager.h>
#include <SimpleObjLoader.h>
#include <StringUtils.h>
#include <matrix/InterpolationHelper.h>
#include <articulation/Part.h>
#include <articulation/ForwardKinematics.h>

using namespace std;

static const GLfloat NO_VECTOR3[3] ={0,0,0};
static const GLfloat NECK_TRANSLATE_1[3] ={0,1.5,0};
static const GLfloat NECK_TRANSLATE_2[3] ={0,6.5,0};

static const GLfloat HEAD_TRANSLATE[3] ={0,4,0};

static const GLfloat UPPER_ARM_TRANSLATE_1[3] ={0,4.25,0};
static const GLfloat LEFT_UPPER_ARM_TRANSLATE_2[3] ={-4.75,-2,0};
static const GLfloat RIGHT_UPPER_ARM_TRANSLATE_2[3] ={4.75,-2,0};

static const GLfloat FOREARM_TRANSLATE_1[3] ={0,-4,0};
static const GLfloat FOREARM_TRANSLATE_2[3] ={0,-4,0};

static const GLfloat FIST_TRANSLATE_1[3] ={0,-0.75,0};
static const GLfloat FIST_TRANSLATE_2[3] ={0,-2.8,-0.3};

static const GLfloat THIGH_TRANSLATE_1[3] ={0,-4.5,0};
static const GLfloat LEFT_THIGH_TRANSLATE_2[3] ={-2,-6,0};
static const GLfloat RIGHT_THIGH_TRANSLATE_2[3] ={2,-6,0};

static const GLfloat CALF_TRANSLATE_1[3] ={0,-4.25,0};
static const GLfloat CALF_TRANSLATE_2[3] ={0,-4.25,0};

static const GLfloat FOOT_TRANSLATE_1[3] ={0,-4.5,0};
static const GLfloat FOOT_TRANSLATE_2[3] ={0,-0.2,-1};

Part *parts[15];
Preferences prefs;
int curveSegmentAmount, currentSegment;
GLfloat increment = 0.008, angleInterval = 6;
GLfloat rotationTorso[3] = {0,0,0}, rotationLeftThigh[3] = {}, rotationRightThigh[3]={};
GLfloat translationTorso[3] = {0,0,-15}, translationLeftThigh[3] = {}, translationRightThigh[3]={};
GLfloat rotationLeftCalf[3] = {}, rotationRightCalf[3]={};
GLfloat translationLeftCalf[3] = {}, translationRightCalf[3]={};

GLfloat tVector[4]={}, quaternion[4] = {}, eulerAngle[3] = {}, translation[3] = {};
int window;

static char* CALF_OBJ_NAME = (char *) "calf.obj";
static char* THIGH_OBJ_NAME = (char *) "thigh.obj";
static char* TORSO_OBJ_NAME = (char *) "torso.obj";
static char* NECK_OBJ_NAME = (char *) "neck.obj";
static char* UPPER_ARM_OBJ_NAME = (char *) "upper_arm.obj";
static char* FIST_OBJ_NAME = (char *) "fist.obj";
static char* HEAD_OBJ_NAME = (char *) "head.obj";
static char *FOOT_OBJ_NAME = (char *) "foot.obj";
static char *FOREARM_OBJ_NAME = (char *) "forearm.obj";

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
    // load obj files and create Part instances
    GLuint torsoObjID = SimpleObjLoader::loadObj(TORSO_OBJ_NAME, 1, 1.0);
    parts[0] = new Part(1, torsoObjID, (GLfloat *) NO_VECTOR3, (GLfloat *) NO_VECTOR3, nullptr);
    GLuint thighObjID = SimpleObjLoader::loadObj(THIGH_OBJ_NAME, 2, 1.0);
    parts[1] = new Part(2, thighObjID, (GLfloat *) THIGH_TRANSLATE_1, (GLfloat *) LEFT_THIGH_TRANSLATE_2,parts[0]);
    parts[2] = new Part(3, thighObjID, (GLfloat *) THIGH_TRANSLATE_1, (GLfloat *) RIGHT_THIGH_TRANSLATE_2,parts[0]);
    GLuint calfObjID = SimpleObjLoader::loadObj(CALF_OBJ_NAME, 3, 1.0);
    parts[3] = new Part(4, calfObjID, (GLfloat *) CALF_TRANSLATE_1, (GLfloat *) CALF_TRANSLATE_2, parts[1]);
    parts[4] = new Part(5, calfObjID, (GLfloat *) CALF_TRANSLATE_1, (GLfloat *) CALF_TRANSLATE_2, parts[2]);
    GLuint footObjID = SimpleObjLoader::loadObj(FOOT_OBJ_NAME, 4, 1.0);
    parts[5] = new Part(6, footObjID, (GLfloat *) FOOT_TRANSLATE_1, (GLfloat *) FOOT_TRANSLATE_2, parts[3]);
    parts[6] = new Part(7, footObjID, (GLfloat *) FOOT_TRANSLATE_1, (GLfloat *) FOOT_TRANSLATE_2, parts[4]);

    GLuint neckObjID = SimpleObjLoader::loadObj(NECK_OBJ_NAME, 5, 1.0);
    parts[7] = new Part(8, neckObjID, (GLfloat *) NECK_TRANSLATE_1, (GLfloat *) NECK_TRANSLATE_2, parts[0]);

    GLuint headObjID = SimpleObjLoader::loadObj(HEAD_OBJ_NAME, 5, 1.0);
    parts[8] = new Part(9, headObjID, (GLfloat *) HEAD_TRANSLATE, (GLfloat *) NO_VECTOR3, parts[7]);

    GLuint upperArmObjID = SimpleObjLoader::loadObj(UPPER_ARM_OBJ_NAME, 5, 1.0);
    parts[9] = new Part(10, upperArmObjID, (GLfloat *) UPPER_ARM_TRANSLATE_1, (GLfloat *) LEFT_UPPER_ARM_TRANSLATE_2, parts[0]);
    parts[10] = new Part(11, upperArmObjID, (GLfloat *) UPPER_ARM_TRANSLATE_1, (GLfloat *) RIGHT_UPPER_ARM_TRANSLATE_2, parts[0]);

    GLuint foreArmObjID = SimpleObjLoader::loadObj(FOREARM_OBJ_NAME, 5, 1.0);
    parts[11] = new Part(12, foreArmObjID, (GLfloat *) FOREARM_TRANSLATE_1, (GLfloat *) FOREARM_TRANSLATE_2, parts[9]);
    parts[12] = new Part(13, foreArmObjID, (GLfloat *) FOREARM_TRANSLATE_1, (GLfloat *) FOREARM_TRANSLATE_2, parts[10]);

    GLuint fistObjID = SimpleObjLoader::loadObj(FIST_OBJ_NAME, 5, 1.0);
    parts[13] = new Part(14, fistObjID, (GLfloat *) FIST_TRANSLATE_1, (GLfloat *) FIST_TRANSLATE_2, parts[11]);
    parts[14] = new Part(15, fistObjID, (GLfloat *) FIST_TRANSLATE_1, (GLfloat *) FIST_TRANSLATE_2, parts[12]);
    // Create the menu structure and attach it to the right mouse button
    UserInputManager::createMouseMenu();
    glutMainLoop();
    return 0;
}