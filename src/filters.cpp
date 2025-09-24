// Here we will Implement All filters
#include <iostream>
#include "Image_Class.h"
#include "filters.h"

using namespace std;
// Demo filter to test the functionality of the code:
void applyGrayscale(Image &image) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            unsigned int avg = 0; // Initialize average value

            for (int k = 0; k < 3; ++k) {
                avg += image(i, j, k); // Accumulate pixel values
            }

            avg /= 3; // Calculate average

            // Set all channels to the average value
            image(i, j, 0) = avg;
            image(i, j, 1) = avg;
            image(i, j, 2) = avg;
        }
    }
}
// MAIN FILTERS:
// @samirkahlawy ================================ Filters (1, 4, 7, 10)


// ====================================================================





    
// @zi-crypto =================================== Filters (2, 5, 8, 11)


// ====================================================================


   


    
// @nytril-ark ================================== Filters (3, 6, 9, 12)
void applyInvertColors(Image &image) {
  for (int i = 0; i < image.width; ++i) {
    for (int j = 0; j < image.height; ++j) {
      for (int k = 0; k < 3; ++k) {
        image(i, j, k) = 255 - image(i, j, k);
      }
    }
  }
}

void  applyRotateImage(Image &image, int angle) {
  //angle hardcoded as 90 for now
  int m = image.width;
  int n = image.height;
  Image rot(n, m);

  for (int i = 0; i < image.width; ++i) {
    for (int j = 0; j < image.height; ++j) {
      for (int k = 0; k < 3; ++k) {
        rot(j , m-1-i , k) = image(i, j, k);
      }
    }
  }
  image = rot; 
}


void applyAddColoredFrame(Image &image, int thickness, int r, int g, int b, bool decoration) {
  if (!decoration) {
    int rgb[3] = {r, g, b};
    int m = image.width;
    int n = image.height;
    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        if (i < thickness || i >= (m - thickness) || j < thickness || j >= n - thickness) { 
          for (int k = 0; k < 3; ++k) {
            image(i, j, k) = rgb[k]; 
          }
        }
      }
    } 
  } else { 
    int rgb[3] = {r, g, b};
    int m = image.width;
    int n = image.height;
    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        if (i < thickness || i >= m - thickness || j < thickness || j >= n - thickness ||  ( (i < thickness * 3 && i >= thickness * 2) || 
           (i >= m - thickness * 3 && i < m - thickness * 2)     || 
           (j < thickness * 3 && j >= thickness * 2)             || 
           (j >= n - thickness * 3 && j < n - thickness * 2) ) ) { 
            for (int k = 0; k < 3; ++k) {
            image(i, j, k) = rgb[k]; 
          }
        }
      }
    } 
  }
}
// ====================================================================

