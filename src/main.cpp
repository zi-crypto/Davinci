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
    Image image2(filename);
    //applyRotateImage(image, 90); //NOTE: assignment asks for 90, 180, 270..
    //..rotation etc.. to apply that, the function will be called recursively from main.cpp

    // ================= Samir's Filters =================
    // applyGrayscale(image);  // Filter 1
    // applyMerge(image, image2);  // Filter 4
    // applyDarkenImage(image, 0.5f);  // Filter 7a
    // applyLightenImage(image, 1.5f);  // Filter 7b
    // applyAddColoredFrame(image, 5, 255, 0, 0);  // Filter 10
    //
    // // ================= Ziad's Filters =================
    // applyBlackWhite(image);  // Filter 2
    // applyFlipImage(image, true);  // Filter 5 (horizontal flip)
    // applyCropImage(image, 10, 10, 100, 100);  // Filter 8
    // applyDetectEdges(image);  // Filter 11
    //
    // // ================= Ammen's Filters =================
    // applyInvertColors(image);  // Filter 3
    // applyRotateImage(image, 90);  // Filter 6
    // applyResizeImage(image, 200, 300);  // Filter 9
    // applyApplyBlur(image, 5);  // Filter 12
 
    cout << "Pls enter image name to store new image (with extension): ";
    cin >> filename;

    image.saveImage(filename);

    return 0;
}
