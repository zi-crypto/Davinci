// Here we will Implement All filters
#include <iostream>
#include "Image_Class.h"
#include "filters.h"
#include <cmath>
#include <vector>
using namespace std;

#define _USE_MATH_DEFINES // Required for some compilers

// MAIN FILTERS:
// @samirkahlawy ================================ Filters (1, 4, 7, 10)
// Filter 1: Grayscale Conversion
void applyGrayscale(Image &image) {
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            int red = image(i,j,0), green = image(i,j,1), blue = image(i,j,2);
			int gray = (red + green + blue) / 3;
			image(i,j,0)=image(i,j,1)=image(i,j,2)=gray;
        }
    }
}

// Filter 4: Merge Images
void applyMerge(Image &image1, Image &image2) {
	if (image1.width != image2.width || image1.height != image2.height) {
    cout << "Error: Images must be of the same size to merge!\n";
    return;
	}
    for (int i = 0; i < image1.width; ++i) {
        for (int j = 0; j < image1.height; ++j) {
            for (int k = 0; k < 3; ++k) {

                image1(i, j, k) = floor(( image1(i, j, k) + image2(i, j, k) ) / 2);// 0.5 by defult but we can change it
            }
        }
    }
    cout << "Merge filter applied!\n";
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
	applyGrayscale(image);// transform to gray scal to be more easy
	int Threshold = 30 ;//Affects edge clarity
	for (int i = 0; i < image.width; ++i) {
		for (int j = 0; j < image.height; ++j) {
			// compare between current , right and botton
			if ( (i < image.width - 1 && abs(image(i, j, 0) - image(i+1, j, 0)) > Threshold) ||
     		(j < image.height - 1 && abs(image(i, j, 0) - image(i, j+1, 0)) > Threshold) )
			{
    			image(i, j, 0) = image(i, j, 1) = image(i, j, 2) = 0; // Edge → Black
			}
			else {
    			image(i, j, 0) = image(i, j, 1) = image(i, j, 2) = 255; // No edge → White
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
    if (horizontal){
        // Swap (i, j) with (image.width - 1 - x, y)
        // Each Row is Reversed

        for (int j = 0; j < image.height; j++) {
            for (int i = 0; i < image.width/2 ; i++) {
                int OppositeI = image.width - 1 - i;
                for (int k = 0; k < 3; ++k) {
                    unsigned char temp = image.getPixel(OppositeI, j, k);
                    image.setPixel(OppositeI, j, k, image.getPixel(i, j, k));
                    image.setPixel(i, j, k, temp);
                }
            }
        }
    }
    else {
        // Vertical Flip:
        // Swap (i, j) with (i, image.height - 1 - y)
        // Rows' Order is reversed

        for (int j = 0; j < image.height/2; j++) {
            for (int i = 0; i < image.width ; i++) {
                int OppositeJ = image.height - 1 - j;
                for (int k = 0; k < 3; ++k) {
                    unsigned char temp = image.getPixel(i, OppositeJ, k);
                    image.setPixel(i, OppositeJ, k, image.getPixel(i, j, k));
                    image.setPixel(i, j, k, temp);
                }
            }
        }
    }
}

// Filter 8: Crop Images
void applyCropImage(Image &image, int x, int y, int width, int height){
    Image cropped(width, height);
    if (((x + width) <= image.width) && ((y + height) <= image.height) && (x >= 0) && (y >= 0)){
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height ; j++) {
                for (int k = 0; k < 3; ++k) {
                    cropped.setPixel(i, j, k, image.getPixel(i + x, j + y , k));
                }
            }
        }
        image = cropped;
    } else {
        std::cerr << "Error: Invalid crop parameters (" << x << ", " << y << ", " << width << ", " << height << ") for image of size (" << image.width << ", " << image.height << ")." << std::endl;
    }
}

// Filter 11: Resizing Images
void applyResizeImage(Image &image, int newWidth, int newHeight){
    Image resized(newWidth, newHeight);
    double scaleX = static_cast<double>(image.width) / newWidth;
    double scaleY = static_cast<double>(image.height) / newHeight;
    for (int i = 0; i < newWidth; i++) {
        for (int j = 0; j < newHeight ; j++) {
            double origI = i * scaleX;
            double origJ = j * scaleY;
            for (int k = 0; k < 3; ++k) {
                resized.setPixel(i, j, k, image.getPixel(min(static_cast<int>(floor(origI)), image.width - 1), min(static_cast<int>(floor(origJ)), image.width - 1), k));
            }
        }
    }
    image = resized;
}

// Filter 17: Infrared Effect
void applyInfrared(Image &image){
    for (int i = 0; i < image.width; i++) {
        for (int j = 0; j < image.height ; j++) {
          for (int k = 0; k < 3; k++){
            int red = image(i,j,0), green = image(i,j,1), blue = image(i,j,2);
            int newRed = min(255.0, red*1.5), newGreen = green*0.5, newBlue = blue*0.3; // the scalers here are just artistic choices
            image(i, j, 0) = newRed;
            image(i, j, 1) = newGreen;
            image(i, j, 2) = newBlue;
          }
        }
      }
}

// Filter 18: Skew Image
void applySkew(Image &image, double skewDegree){
  float skewSlope = tan(skewDegree * M_PI  / 180.0);
  int newWidth = image.width + abs(ceil(image.height / skewSlope));
  Image skewed(newWidth, image.height);

  for (int y = 0; y < image.height; y++){
    for (int x = 0; x < newWidth; x++){
      for (int k = 0; k < 3; k++){
        skewed.setPixel(x, y, k, 255); // white background
      }
    }
  }

  for (int i = 0; i < image.width; i++) {
      for (int j = 0; j < image.height ; j++) {
        for (int k = 0; k < 3; k++){
          int newI = i + skewSlope * j;  // shift depends on vertical position
          skewed.setPixel(newI, j, k, image.getPixel(i, j, k));
        }
      }
    }
  image = skewed;
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
void applyRotateImage(Image &image) {
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

// Filter: 13 sunlight
void applySunlight(Image &image, double YellowScale) {
  srand(time(NULL));
  double red, green, blue;
  for (int i = 0; i < image.width; ++i) {
    for (int j = 0; j < image.height; ++j) {
      if (image(i, j, 0) >= 231 || image(i, j, 1) >= 231 || image(i, j, 2) >= 231) {
        continue;
      } else {
        red = image(i, j, 0) * YellowScale;
        green = image(i, j, 1) * YellowScale;
        blue = image(i, j, 2) *  0.9;
        image(i, j, 0) = red;
        image(i, j, 1) = green;
        image(i, j, 2) = blue;
      }
    }
  }
}








// Filter: 14 oil painting

  // PARAMETERS:  
  // int kernelSize     ||  second blur sigma
  // double sigma       ||  first blur sigma
  // double quality     ||  rescale image for fastness
  // double c           ||  local contrast scale
  // double s           ||  saturation scale
  // double blurKernel  ||  final blur kernel size
  // double blurSigma   ||  final blur sigma

void applyOilPainting(Image &image, int kernelSize, double sigma, double quality, double c, double s, double blurKernel, double blurSigma) {
  srand(time(NULL));
  applyResizeImage(image, image.width * quality, image.height * quality);
  Image temp;
  int m = image.width;
  int n = image.height;
  typedef vector<vector<double>> matrix;
  Image oilPainting = image;
  matrix kernel(kernelSize, vector<double>(kernelSize));
  double kernelSum = 0;

  // Filter X: utility filter, horizontal smear/shift:
  temp = image;
  int maxShift = m / 100;  // max pixels to shift
  int delta;
  int X = n / 25;
  for (int j = 0; j < n; j++) {          // column
      delta = ((j / X) % 2 == 0) ? (rand() % maxShift) + 1 : -((rand() % maxShift) + 1);
      for (int i = 0; i < m; i++) {      // row
          int shifted_i = i + delta;
          if (shifted_i < 0) shifted_i = 0;
          if (shifted_i >= m) shifted_i = m - 1;

          temp(i, j, 0) = image(shifted_i, j, 0);
          temp(i, j, 1) = image(shifted_i, j, 1);
          temp(i, j, 2) = image(shifted_i, j, 2);
      }
  }

  image = temp;

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
        double M = sum / kernelSum; // gaussian mean
        double I = image(i, j, k) * 1.0;
        int red = image(i,j,0), green = image(i,j,1), blue = image(i,j,2);
        double A = (red + green + blue) / 3.0;  // per pixel average
        int randNum = (rand() % 8) + 1; // generating random number for noise effect
        oilPainting(i, j, k) = min(255.0, max(0.0, A + s * ((M + c * (I - M)) - A) + randNum));
      } 
    } 
  } 

  image = oilPainting;
  applyGaussianBlur(image, blurKernel, blurSigma);
}

// ====================================================================
