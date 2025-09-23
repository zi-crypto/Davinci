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
    applyGrayscaleFilter(image);

    cout << "Pls enter image name to store new image (with extension): ";
    cin >> filename;

    image.saveImage(filename);

    return 0;
}
