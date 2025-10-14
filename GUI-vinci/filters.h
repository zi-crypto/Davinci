// Here We Will Create Our Header
// Where we Declare all filter functions
#ifndef FILTERS_H
#define FILTERS_H

#include "Image_Class.h"

// ================= Samir's Filters =================
void applyGrayscale(Image &image);  // Filter 1
void applyMerge(Image &image1, Image &image2);  // Filter 4
void applyDarkenLighten(Image &image, double range); // Filter 7 (two parts)
void applyDetectEdges(Image &image);  // Filter 10
void applyTV(Image &image);  // Filter TV
void applyRedTint(Image &image, float intensity); // Extra Filter
void applyGreenTint(Image &image); // Extra Filter
// ================= Ziad's Filters =================
void applyBlackWhite(Image &image); // Filter 2
void applyFlipImage(Image &image, bool horizontal);  // Filter 5
void applyCropImage(Image &image, int x, int y, int width, int height);  // Filter 8
void applyResizeImage(Image &image, int newWidth, int newHeight); // Filter 11
void applyInfrared(Image &image); // Filter 17
void applySkew(Image &image, double skewDegree = 45.0);

// ================= Ammen's Filters =================
void applyInvertColors(Image &image);  // Filter 3
void applyRotateImage(Image &image);  // Filter 6
void applyAddColoredFrame(Image &image, int thickness, int r, int g, int b, bool decoration);  // Filter 9
void applyGaussianBlur(Image &image, int kernelSize, double sigma);  // Filter 12
void applySunlight(Image &image, double YellowScale = 1.1); // Filter 13
void applyOilPainting(Image &image, int kernelSize = 3, double sigma = 12, double quality = 0.75, double c = 3, double s = 1.8, double blurKernel = 7, double blurSigma = 37);
 // Filter 14

#endif // FILTERS_H
