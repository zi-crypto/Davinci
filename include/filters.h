// Here We Will Create Our Header
// Where we Declare all filter functions
#ifndef FILTERS_H
#define FILTERS_H

#include "Image_Class.h"



void applyGrayscale_Filter_1(Image &image);
void applyBlackWhite_Filter_2(Image &image);
void applyInvertColors_Filter_3(Image &image);

void applyMerge_Filter_4(Image &image1, const Image &image2);
void applyFlipImage_Filter_5(Image &image, bool horizontal);
void applyRotateImage_Filter_6(Image &image, int angle);

void applyDarkenImage_Filter_7(Image &image, float factor);
void applyLightenImage_Filter_7(Image &image, float factor);
void applyCropImage_Filter_8(Image &image, int x, int y, int width, int height);

void applyAddColoredFrame_Filter_9(Image &image, int thickness, int r, int g, int b);
void applyDetectEdges_Filter_10(Image &image);
void applyResizeImage_Filter_11(Image &image, int newWidth, int newHeight);

void applyApplyBlur_Filter_12(Image &image, int intensity);



#endif // FILTERS_H
