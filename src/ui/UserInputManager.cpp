//
// Created by Qichen on 9/23/16.
//

#include <cstdlib>
#include <setup/Preferences.h>
#include <nfd.h>
#include <setup/SetupFileLoader.h>
#include "UserInputManager.h"

static int* windowID;

Preferences *prefsPointer;

/****
 * This function describes the behavior of the right mouse button menu.
 * @param window is the identifier the opengl window
 * @param preferences is the setup for current animation
 */
UserInputManager::UserInputManager(int * window, Preferences* preferences) {
    windowID = window;
    prefsPointer = preferences;
}

void UserInputManager::setMouseMenuBehavior(int id){
    switch (id)
    {
        case 1:
            // choose the user provided text file in a "native file dialog"
            if (loadUserInputFromFileDialog()) {
                prefsPointer->setKeyFramesLoaded(true);
                // then calculate the coefficient matrices for interpolation use
                prefsPointer->calculateCoefficientMatrices();
            }
            break;
        //reset the preferences
        case 2 :
            prefsPointer->resetPreferences();
            break;
        case 3 :
            if (!prefsPointer->getAreKeyFramesLoaded()){
                /* TODO  show a message to let user load file first */
                break;
            }
            prefsPointer->resetTimeProgress();
            prefsPointer->setIsPlaying(true);
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
            prefsPointer->resetPreferences();
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
        SetupFileLoader::loadPreferencesFromTextFile(outPath, prefsPointer);
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
