// Here we will Implement All filters
#include <iostream>
#include "Image_Class.h"
#include "filters.h"
#include <cmath>
#include <vector>
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

void applyGaussianBlur(Image &image, int kernelSize, double sigma) {
  int m = image.width;
  int n = image.height;
  typedef vector<vector<double>> matrix;
  Image blurred = image;
  matrix kernel(kernelSize, vector<double>(kernelSize));
  double kernelSum = 0;

  for (int i = 0; i < kernelSize; i++) {
    for (int j = 0; j < kernelSize; j++) {
      kernel[i][j] = exp(-(i*i + j*j) / (2.0 * sigma * sigma)); 
      kernelSum += kernel[i][j];
    }
  }
  
  int kCenter = kernelSize / 2;

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      for (int k = 0; k < 3; ++k) {

        double sum = 0.0;
        for (int x = 0; x < kernelSize; x++) {
          for (int y = 0; y < kernelSize; y++) {
            int si = i + x - kCenter;
            int sj = j + y - kCenter;
            if (si >= 0 && si < m && sj >= 0 && sj < n) {
              sum += image(si, sj, k) * kernel[x][y];
            }
          }
        }
        blurred(i, j, k) = sum / kernelSum; 
      }
    }
  }

  image = blurred;
}
// ====================================================================

