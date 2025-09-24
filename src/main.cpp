// Entry Point
// Here we will Implement the Menu + Integration
#include <iostream>
#include "Image_Class.h"
#include "filters.h"

using namespace std;

int main() {
    string filename;
    cout << "Pls enter colored image name to turn to gray scale: ";
    cin >> filename;

    Image image(filename);

    // Call your filter
    // applyGrayscale_Filter_1(image);
    // applyInvertColors_Filter_3(image);
    //applyRotateImage_Filter_6(image, 90); //NOTE: assignment asks for 90, 180, 270..
    //..rotation etc.. to apply that, the function will be called recursively from main.cpp
  
    cout << "Pls enter image name to store new image (with extension): ";
    cin >> filename;

    image.saveImage(filename);

    return 0;
}
