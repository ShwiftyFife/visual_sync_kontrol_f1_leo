#ifndef LED_SCENE_CONTROLLER_H
#define LED_SCENE_CONTROLLER_H

#include "led_controller.h"
#include <hidapi/hidapi.h>


// =============================================================================
// LED SCENE CONTROLLER - Predefined LED scenes for effects pages
// =============================================================================
/*
 * Scene Controller - Manages predefined LED scenes for effects pages
 * Each effects page has a unique LED pattern for the 4x4 matrix buttons
 */
class SceneController {
public:
    /*
     * Apply the LED scene for the specified effects page
     * @param device HID device handle
     * @param effects_page Effects page number (1-99)
     */
    static void setEffectsPageScene(int effects_page);

private:
    /*
     * Apply scene for effects page 1 - All Green
     * @param device HID device handle
     */
    static void applyScene1();
    static void applyScene2();
    
    /*
     * Apply default scene for undefined pages - All Off
     * @param device HID device handle
     */
    static void applyDefaultScene();
};

#endif // LED_SCENE_CONTROLLER_H