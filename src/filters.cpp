// Here we will Implement All filters
#include <iostream>
#include "Image_Class.h"
#include "filters.h"
#include <cmath>
#include <vector>
using namespace std;


// MAIN FILTERS:
// @samirkahlawy ================================ Filters (1, 4, 7, 10)

// Filter 1: Grayscale Conversion
void applyGrayscale(Image &image) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            int red   = image(i, j, 0);
            int green = image(i, j, 1);
            int blue  = image(i, j, 2);

            int gray = 0.299 * red + 0.587 * green + 0.114 * blue;//Luminance method is better than avg

            image(i, j, 0) = gray;
            image(i, j, 1) = gray;
            image(i, j, 2) = gray;
        }
    }
}

// Filter 4: Merge Images
void applyMerge(Image &image1, Image &image2) {
    for (int i = 0; i < image1.width; ++i) {
        for (int j = 0; j < image1.height; ++j) {
            for (int k = 0; k < 3; ++k) {
                image1(i, j, k) = image1(i, j, k) * 0.5 + image2(i, j, k) * 0.5;// 0.5 by defult but we can change it
            }
        }
    }
}

// Filter 7: Darken and Lighten Image
void applyDarkenLighten(Image &image, double range) {
    // range: from % -100  to % +100
	range/=100;
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            for (int k = 0; k < 3; ++k) {
                int newValue = image(i, j, k) * (1 + range);//each of pixel multiplayed of 1+range ,+1 must
                if (newValue > 255) newValue = 255;// 255 means white pixel
                if (newValue < 0)   newValue = 0;// 0 means black pixel
                image(i, j, k) = newValue;
            }
        }
    }
}

// Filter 10: Detect Image Edges
void applyDetectEdges(Image &image) {
	applyGrayscale(image);// transform to grayscale to be more easy
	int Threshold = 30 ;//Affects edge clarity
	for (int i = 0; i < image.width; ++i) {
		for (int j = 0; j < image.height; ++j) {
			// compare between current , right and botton
			if( abs( image(i, j, 0) - image(i+1, j, 0) ) > Threshold || abs( image(i, j, 0) - image(i, j+1, 0) ) > Threshold) {
           		image(i, j, 0) = image(i, j, 1) = image(i, j, 2) = 0;// This means there is an edge.The pixel black (0).
			}
			else {
				image(i, j, 0) = image(i, j, 1) = image(i, j, 2) = 255;// no edge ,pixel white(255)
			}
		}
	}
}
// ====================================================================
    
// @zi-crypto =================================== Filters (2, 5, 8, 11)
// Filter 2: Black and White
void applyBlackWhite(Image &image) {
    const int threshold = 128; // Threshold for black/white decision
    
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            unsigned int avg = 0;

            for (int k = 0; k < 3; ++k) {
                avg += image(i, j, k);
            }

            avg /= 3;

            if (avg < threshold) {
                image(i, j, 0) = 0;
                image(i, j, 1) = 0;
                image(i, j, 2) = 0;
            }
            else {
                image(i, j, 0) = 255;
                image(i, j, 1) = 255;
                image(i, j, 2) = 255;
            }
        }
    }
}

// Filter 5: Flip Image
void applyFlipImage(Image &image, bool horizontal){
}

// Filter 8: Crop Images
void applyCropImage(Image &image, int x, int y, int width, int height){
}

// Filter 11: Resizing Images
void applyResizeImage(Image &image, int newWidth, int newHeight){
}
// ====================================================================
    
// @nytril-ark ================================== Filters (3, 6, 9, 12)
// Filter 3: Invert Image
void applyInvertColors(Image &image) {
  for (int i = 0; i < image.width; ++i) {
    for (int j = 0; j < image.height; ++j) {
      for (int k = 0; k < 3; ++k) {
        image(i, j, k) = 255 - image(i, j, k);
      }
    }
  }
}

// Filter 6: Rotate Image
void applyRotateImage(Image &image, int angle) {
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

// Filter 9: Adding a Frame to the Picture
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

// Filters 12: Blur Images
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