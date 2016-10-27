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
#include <articulation/ReverseKinematics.h>

using namespace std;

static const GLfloat NO_VECTOR3[3] ={0,0,0};
static const GLfloat THIGHS_TRANSLTE_1[3] ={0,-4.5,0};
static const GLfloat LEFT_TIHGH_TRANSLTE_2[3] ={-2,-2.5,0};
static const GLfloat RIGHT_TIHGH_TRANSLTE_2[3] ={2,-2.5,0};
static const GLfloat CALFS_TRANSLTE_1[3] ={0,-4.25,0};
static const GLfloat LEFT_CALF_TRANSLTE_2[3] ={0,-4.25,0};
static const GLfloat RIGHT_CALF_TRANSLTE_2[3] ={0,-4.25,0};

Part *parts[5];
Preferences prefs;
int curveSegmentAmount, currentSegment;
GLfloat increment = 0.005, angleInterval = 6;
GLfloat rotationTorso[3] = {0,0,0}, rotationLeftThigh[3] = {}, rotationRightThigh[3]={};
GLfloat translationTorso[3] = {0,0,-15}, translationLeftThigh[3] = {}, translationRightThigh[3]={};
GLfloat rotationLeftCalf[3] = {}, rotationRightCalf[3]={};
GLfloat translationLeftCalf[3] = {}, translationRightCalf[3]={};

GLfloat tVector[4]={}, quaternion[4] = {}, eulerAngle[3] = {}, translation[3] = {};
int window;

static char* CALF_OBJ_NAME = (char *) "calf.obj";
static char* THIGH_OBJ_NAME = (char *) "thigh.obj";
//static char* TORSO_OBJ_NAME = (char *) "teddy.obj";
static char* TORSO_OBJ_NAME = (char *) "torso.obj";

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
    glMultMatrixf((GLfloat []){1,0,0,0,0,1,0,0,0,0,1,0,0,0,-50,1});

    //TODO insert real local rotation and translation
    //only the local translation of torso change
    ReverseKinematics::setLocalTranslation(parts[0]);
    ReverseKinematics::setLocalRotation(parts);

    drawLinks(false);
}

/***
 * generate the transformation matrices for every link (torso, tighs, calfs) w.r.t. world space, then draw them
 */
void drawLinks(bool isKeyFraming) {
    int i;
    GLfloat * combinedTransformation;
    for (i = 0; i < 5; i++) {
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
            }
            //TODO if (isKeyFraming)
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
    glPushMatrix();
    //move the model view away from the camera, so that we are not inside the object1
    glMultMatrixf((GLfloat []){1,0,0,0,0,1,0,0,0,0,1,0,0,0,-150,1});
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
        //TODO trim the code here
        currentSegment++;
        prefs.resetTimeProgress();
        prefs.currentCoefficientMatrices = prefs.currentCoefficientMatrices->next;
        // move and rotate the object1
        glMultMatrixf(RotationHelper::generateFlattenedTransformationMatrix(quaternion, translation, true));
        // draw the faces of the object1
        glCallList(1);
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
        // move the object1
        glMultMatrixf(RotationHelper::generateFlattenedTransformationMatrix(eulerAngle, translation, false));
    } else {
        // if getting quaternion version of animation
        // prepare the quaternion vector
        InterpolationHelper::prepareQuaternionVector(quaternion, tVector, prefs.currentCoefficientMatrices->qRotation);
        // move and rotate the object1
        glMultMatrixf(RotationHelper::generateFlattenedTransformationMatrix(quaternion, translation, true));
    }
    // draw the faces of the object1
    glCallList(1);
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
    glutInitWindowSize(1280, 800);
    glutInitWindowPosition(100, 100);
    window = glutCreateWindow("CSCI 6555 project 2 : Articulated Figure");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    UserInputManager(&window, &prefs);
    glutKeyboardFunc(UserInputManager::keyboardFunc);
    glutMouseFunc(UserInputManager::mouseFunc);

    // load obj files and create Part instances
    GLuint torsoObjID = SimpleObjLoader::loadObj(TORSO_OBJ_NAME, 1);
    parts[0] = new Part(1, torsoObjID, (GLfloat *) NO_VECTOR3, (GLfloat *) NO_VECTOR3, nullptr);
    GLuint thighObjID = SimpleObjLoader::loadObj(THIGH_OBJ_NAME, 2);
    parts[1] = new Part(2, thighObjID, (GLfloat *) THIGHS_TRANSLTE_1, (GLfloat *) LEFT_TIHGH_TRANSLTE_2,parts[0]);
    parts[2] = new Part(3, thighObjID, (GLfloat *) THIGHS_TRANSLTE_1, (GLfloat *) RIGHT_TIHGH_TRANSLTE_2,parts[0]);
    GLuint calfObjID = SimpleObjLoader::loadObj(CALF_OBJ_NAME, 3);
    parts[3] = new Part(4, calfObjID, (GLfloat *) CALFS_TRANSLTE_1, (GLfloat *) LEFT_CALF_TRANSLTE_2, parts[1]);
    parts[4] = new Part(5, calfObjID, (GLfloat *) THIGHS_TRANSLTE_1, (GLfloat *) RIGHT_CALF_TRANSLTE_2, parts[2]);

    // Create the menu structure and attach it to the right mouse button
    UserInputManager::createMouseMenu();
    glutMainLoop();
    return 0;
}