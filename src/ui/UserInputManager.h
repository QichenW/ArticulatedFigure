//
// Created by Qichen on 9/23/16.
//

#ifndef GLUTPROJECT_USERINPUTMANAGER_H
#define GLUTPROJECT_USERINPUTMANAGER_H

#include <cstdlib>
#include <setup/Preferences.h>
#include <nfd.h>
#include <setup/SetupFileLoader.h>
#include <manual/CameraMotion.h>
// glut
#if defined(__APPLE__)
#include <GLUT/glut.h>
#include <setup/Preferences.h>

#else
#include <GL/glut.h>
#endif

class UserInputManager {
private:
    static void setMouseMenuBehavior(int id);

/** sub-menus not implemented for now **/
//    static void orientationMenu(int id);
//    static void inbetweeningMenu(int id);
public:
    UserInputManager(int * window, Preferences * preferences);
    static void createMouseMenu();
    static void mouseFunc (int button, int state, int x, int y);
    static void keyboardFunc(unsigned char, int, int);

    static bool loadUserInputFromFileDialog();
};


#endif //GLUTPROJECT_USERINPUTMANAGER_H
