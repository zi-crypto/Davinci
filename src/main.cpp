// Entry Point
// Here we will Implement the Menu + Integration
#include <iostream>
#include "Image_Class.h"
#include "filters.h"

using namespace std;

int main() {
    string filename;
    cout << "Enter Your Image: ";
    cin >> filename;

    Image image(filename);

    // Prompt the user to choose the Available Filters
    int selectedFilter;
    cout << "Select Your Wanted Filter to Apply:" << endl;
    cout << "Filter 1: Grayscale Conversion" << endl;
    cout << "Filter 2: Black and White" << endl;
    cout << "Your Wanted Filter Number: ";
    cin >> selectedFilter;

    // Call your filter
    switch (selectedFilter)
    {
    case 1:
        applyGrayscaleFilter(image);
        break;
    
    case 2:
        applyBlackAndWhiteFilter(image);
        break;

    default:
        break;
    }
    

    cout << "Please Enter image name to store new image (with extension): ";
    cin >> filename;

    image.saveImage(filename);

    return 0;
}
