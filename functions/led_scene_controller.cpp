#include "headers/led_scene_controller.h"
#include "headers/led_controller.h"

// =============================================================================
// led_controller.h - Available colors enum reference
// =============================================================================

/**
 * enum class LEDColor {
 *     black,
 *     red,
 *     orange,
 *     lightorange,
 *     warmyellow,
 *     yellow,
 *     lime,
 *     green,
 *     mint,
 *     cyan,
 *     turquise,
 *     blue,
 *     plum,
 *     violet,
 *     purple,
 *     magenta,
 *     fuchsia,
 *     white
 * };
 */

// =============================================================================
// LED SCENE CONTROLLER - Predefined LED scenes for effects pages
// =============================================================================

/*
* Scene Controller - Manages predefined LED scenes for effects pages
* Each effects page has a unique LED pattern for the 4x4 matrix buttons
*
* @param device: Pointer to the opened HID device
* @param effects_page: Effects page number (1-99)
*/

// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Add effects page scenes here !
// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

void SceneController::setEffectsPageScene(int effects_page) {
    switch (effects_page) {
        case 1:
            applyScene1();
            break;
        case 2:
            applyScene2();
            break;
        default:
            applyDefaultScene();
            break;
    }
}

// =============================================================
// Effects Page Scenes
// =============================================================

/*
* TRUE for all effects pages:
*
* Apply scene for effects pages
* @param device HID device handle
*/


// === Default page ===
// Default scene for undefined pages - Rainbow colors, low brightness
void SceneController::applyDefaultScene() {
    setMatrixButtonLED(1, 1, LEDColor::red, 0.2f, true);
    setMatrixButtonLED(1, 2, LEDColor::orange, 0.2f, true);
    setMatrixButtonLED(1, 3, LEDColor::lightorange, 0.2f, true);
    setMatrixButtonLED(1, 4, LEDColor::warmyellow, 0.2f, true);
    setMatrixButtonLED(2, 1, LEDColor::yellow, 0.2f, true);
    setMatrixButtonLED(2, 2, LEDColor::lime, 0.2f, true);
    setMatrixButtonLED(2, 3, LEDColor::green, 0.2f, true);
    setMatrixButtonLED(2, 4, LEDColor::mint, 0.2f, true);
    setMatrixButtonLED(3, 1, LEDColor::cyan, 0.2f, true);
    setMatrixButtonLED(3, 2, LEDColor::turquise, 0.2f, true);
    setMatrixButtonLED(3, 3, LEDColor::blue, 0.2f, true);
    setMatrixButtonLED(3, 4, LEDColor::plum, 0.2f, true);
    setMatrixButtonLED(4, 1, LEDColor::violet, 0.2f, true);
    setMatrixButtonLED(4, 2, LEDColor::purple, 0.2f, true);
    setMatrixButtonLED(4, 3, LEDColor::magenta, 0.2f, true);
    setMatrixButtonLED(4, 4, LEDColor::fuchsia, 0.2f, true);
}

// === Effects Page 1 ===
// Scene for effects page 1 - Left half red, right half white
void SceneController::applyScene1() {
    // Left half red
    for (int row = 1; row <= 4; row++) {
        for (int col = 1; col <= 2; col++) {
            setMatrixButtonLED(row, col, LEDColor::red, 0.2f, true);
        }
    }
    // Right half white
    for (int row = 1; row <= 4; row++) {
        for (int col = 3; col <= 4; col++) {
            setMatrixButtonLED(row, col, LEDColor::white, 0.2f, true);
        }
    }
}

// === Effects Page 2 ===
// Scene for effects page 2 - Left half blue, right half orange
void SceneController::applyScene2() {
    // Left half blue
    for (int row = 1; row <= 4; row++) {
        for (int col = 1; col <= 2; col++) {
            setMatrixButtonLED(row, col, LEDColor::blue, 0.2f, true);
        }
    }
    // Right half orange
    for (int row = 1; row <= 4; row++) {
        for (int col = 3; col <= 4; col++) {
            setMatrixButtonLED(row, col, LEDColor::lightorange, 0.2f, true);
        }
    }
}

