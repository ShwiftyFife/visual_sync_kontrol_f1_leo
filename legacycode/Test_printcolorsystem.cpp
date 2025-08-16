// ________________________________________________________________
// FUNCTION: PRINT COLOR
// Print the color values with brightness
void printColor(ColorSystem color, float brightness) {
    BRGColor result = getColorWithBrightness(color, brightness);
    
    // Convert enum to string for printing
    const char* colorNames[] = {
        "red", "orange", "lightorange", "warmyellow", "yellow", "lime", 
        "green", "mint", "cyan", "turquise", "blue", "plum", 
        "violet", "purple", "magenta", "fuchsia", "white"
    };
    
    printf("%s at %.1f brightness: B=%d R=%d G=%d\n", 
           colorNames[(int)color], brightness, 
           result.blue, result.red, result.green);
}

void testColorSystem() {
	printf("=== Testing Color System ===\n\n");
	
	// Test a few colors at different brightness levels
	printColor(ColorSystem::red, 1.0f);      // Full brightness
	printColor(ColorSystem::red, 0.5f);      // Half brightness
	printColor(ColorSystem::red, 0.0f);      // Off

	printf("\n");

	printColor(ColorSystem::purple, 1.0f);   // Your favorite purple
	printColor(ColorSystem::purple, 0.8f);   // 80% brightness
	printColor(ColorSystem::purple, 0.3f);   // Dim purple
	
	printf("\n");

	printColor(ColorSystem::white, 1.0f);    // Full white
	printColor(ColorSystem::white, 0.1f);    // Very dim white
	
	printf("\n");
	
	// Test edge cases
	printColor(ColorSystem::green, -0.5f);   // Should clamp to 0.0
	printColor(ColorSystem::blue, 1.5f);     // Should clamp to 1.0

	printf("\n=== Test Complete ===\n");
}

// You would add this to your main.cpp to test:
/*
int main() {
    testColorSystem();
    return 0;
}
*/