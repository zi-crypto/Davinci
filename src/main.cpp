// Entry Point
// Here we will Implement the Menu + Integration
#include <iostream>
#include "Image_Class.h"
#include "filters.h"
#include "menu.h"

using namespace std;

int main() {
    // applyRotateImage(image, 90);
    // NOTE: assignment asks for 90, 180, 270..
    // Rotation etc.. to apply that, the function will be called recursively from main.cpp

    // ================= Samir's Filters =================
    // applyGrayscale(image);  // Filter 1 NOTE: working
    // applyMerge(image, image2);  // Filter 4 NOTE: working
    // applyDarkenLighten(image, -90); NOTE: working
    // applyDetectEdges(image);  //  NOTE: working

    // ================= Ziad's Filters =================
    // applyBlackWhite(image);  // Filter 2
    // applyFlipImage(image, true);  // Filter 5 (horizontal flip)
    // applyCropImage(image, 10, 10, 100, 100);  // Filter 8
    // applyResizeImage(image, 200, 300);  // Filter 10
    // applyInfrared(image);  // Filter 17
    // applySkew(image, 45.0);  // Filter 18
    
    // ================= Ammen's Filters =================
    // applyInvertColors(image);  // Filter 3
    // applyRotateImage(image, 90);  // Filter 6
    // applyAddColoredFrame(image, 20, 0, 250, 0, true);  // Filter 9
    // applyGaussianBlur(image, 14, 6.5);  // Filter 12
    // showMenu(); commented out for testing

    return 0;
}
