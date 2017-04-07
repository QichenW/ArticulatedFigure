//
// Created by Qichen on 9/23/16.
//

#include <articulated/ArticulatedMan.h>
#include "UserInputManager.h"

static int* windowID;

ArticulatedMan *manPointer;

/****
 * This function describes the behavior of the right mouse button menu.
 * @param window is the identifier the opengl window
 */
UserInputManager::UserInputManager(int *window, ArticulatedMan *man) {
    windowID = window;
    manPointer = man;
    // prepare the camera motion parameters
    CameraMotion::prepare();

}

void UserInputManager::setMouseMenuBehavior(int id){
    switch (id)
    {
        case 1:
            // choose the user provided text file in a "native file dialog"
            if (loadUserInputFromFileDialog()) {
                manPointer->setKeyFramesLoaded(true);
                // then calculate the coefficient matrices for interpolation use
                manPointer->calculateCoefficientMatrices();
            }
            break;
        //reset the preferences
        case 2 :
            manPointer->resetPreferences();
            break;
        case 3 :
            if (!manPointer->getAreKeyFramesLoaded()) {
                /* TODO  show a message to let user load file first */
                break;
            }
            manPointer->resetTimeProgress();
            manPointer->isPlaying = true;
            break;
        default :
            break;
    }
}

/****
 * the callback function for mouse event, not yet populated
 */
void UserInputManager::mouseFunc (int button, int state, int x, int y) {
//   TODO implement something here, like changing view direction; drag and drap;
}

/****
 * define the entries of the right-mouse menu and call setMouseMenuBehavior to set the behavior
 */
void UserInputManager::createMouseMenu() {

    glutCreateMenu(UserInputManager::setMouseMenuBehavior);

/** sub-menus not implemented for now **/
    glutAddMenuEntry ("Load", 1);
    glutAddMenuEntry ("Reset", 2);
    glutAddMenuEntry ("Start", 3);
    glutAttachMenu (GLUT_RIGHT_BUTTON);
}

/****
 * @param key press 'q' 'Q' or ESC to quit; press 'a' to reset
 */
void UserInputManager::keyboardFunc(unsigned char key, int x, int y) {
    switch ((char) key) {
        case 'a':
            manPointer->resetPreferences();
            break;
        case 'j':
            CameraMotion::zoom(true);
            break;
        case 'k':
            CameraMotion::zoom(false);
            break;
        case 'h':
            CameraMotion::move(true);
            break;
        case 'l':
            CameraMotion::move(false);
            break;
        case 'u':
            CameraMotion::pitch(true);
            break;
        case 'i':
            CameraMotion::pitch(false);
            break;
        case 'q':
        case 'Q':
        case 27:    /* ESC */
            glutDestroyWindow(*windowID);
            exit(0);
        default:
            break;
    }
    return;
}

/****
 * Get the path to the user input script
 * @return true if loaded; false if not
 */
bool UserInputManager::loadUserInputFromFileDialog() {
    nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_OpenDialog( NULL, NULL, &outPath );

    if ( result == NFD_OKAY ) {
        SetupFileLoader::loadPreferencesFromTextFile(outPath, manPointer);
        puts("Success!");
        puts(outPath);
        free(outPath);
        return true;
    }
    else if ( result == NFD_CANCEL ) {
        puts("User pressed cancel.");
    }
    else {
        printf("Error: %s\n", NFD_GetError() );
    }

    return false;
}
