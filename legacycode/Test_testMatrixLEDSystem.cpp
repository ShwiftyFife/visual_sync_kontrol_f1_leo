void testMatrixLEDSystem(hid_device *handle) {
    printf("=== Testing Matrix LED System ===\n\n");
    
    // Create the matrix LED controller
    MatrixLEDController matrixController(handle);
    
    // Test 1: Set a single button
    printf("Test 1: Setting matrix11 to red at full brightness...\n");
    matrixController.setMatrixButtonLED(MatrixButton::matrix11, ColorSystem::red, 1.0f);
    usleep(1000000); // Wait 1 second
    
    // Test 2: Add another button (should NOT turn off the first one!)
    printf("Test 2: Adding matrix22 in blue (matrix11 should stay red)...\n");
    matrixController.setMatrixButtonLED(MatrixButton::matrix22, ColorSystem::blue, 1.0f);
    usleep(1000000); // Wait 1 second
    
    // Test 3: Add a third button with different brightness
    printf("Test 3: Adding matrix33 in purple at 50%% brightness...\n");
    matrixController.setMatrixButtonLED(MatrixButton::matrix33, ColorSystem::purple, 0.5f);
    usleep(1000000); // Wait 1 second
    
    // Test 4: Change the first button to a different color
    printf("Test 4: Changing matrix11 from red to green (others should stay)...\n");
    matrixController.setMatrixButtonLED(MatrixButton::matrix11, ColorSystem::green, 1.0f);
    usleep(1000000); // Wait 1 second
    
    // Test 5: Clear one specific button
    printf("Test 5: Turning off matrix22 only...\n");
    matrixController.clearMatrixButton(MatrixButton::matrix22);
    usleep(1000000); // Wait 1 second
    
    // Test 6: Clear all matrix LEDs
    printf("Test 6: Clearing all matrix LEDs...\n");
    matrixController.clearAllMatrixLEDs();
    
    printf("\n=== Matrix LED Test Complete ===\n");
}



// You would add this to your main.cpp to test:
/*
int main() {
    testMatrixLEDSystem(handle);
    return 0;
}
*/