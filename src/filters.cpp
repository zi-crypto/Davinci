// Here we will Implement All filters
#include <iostream>
#include "Image_Class.h"
#include "filters.h"

using namespace std;
// Demo filter to test the functionality of the code:
void applyGrayscaleFilter(Image &image) {
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


// ====================================================================
}
