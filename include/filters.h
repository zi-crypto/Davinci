// Here We Will Create Our Header
// Where we Declare all filter functions
#ifndef FILTERS_H
#define FILTERS_H

#include "Image_Class.h"


// ================= Samir's Filters =================
void applyGrayscale(Image &image);  // Filter 1
void applyMerge(Image &image1, Image &image2);  // Filter 4
// Filter 7 (two parts)
void applyDarkenLighten(Image &image, double range);
void applyResizeImage(Image &image, int newWidth, int newHeight); // Filter 10

// ================= Ziad's Filters =================
void applyBlackWhite(Image &image);  // Filter 2
void applyFlipImage(Image &image, bool horizontal);  // Filter 5
void applyCropImage(Image &image, int x, int y, int width, int height);  // Filter 8
void applyDetectEdges(Image &image);  // Filter 11

// ================= Ammen's Filters =================
void applyInvertColors(Image &image);  // Filter 3
void applyRotateImage(Image &image, int angle);  // Filter 6
void applyAddColoredFrame(Image &image, int thickness, int r, int g, int b, bool decoration);  // Filter 9
void applyGaussianBlur(Image &image, int kernelSize, double sigma);  // Filter 12


#endif // FILTERS_H
