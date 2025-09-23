// Here We Will Create Our Header
// Where we Declare all filter functions
#ifndef FILTERS_H
#define FILTERS_H

#include "Image_Class.h"

// Filter 1: Grayscale Conversion
void applyGrayscaleFilter(Image &image);

// Filter 2: Black and White
void applyBlackAndWhiteFilter(Image &image);

#endif // FILTERS_H