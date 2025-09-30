// Here we will implement Menu, Load & Save functions
#include <iostream>
#include <string>
#include "Image_Class.h"
#include "filters.h"
using namespace std;

const string red = "\033[31m";
const string orange = "\033[38;5;208m"; 
const string yellow = "\033[33m";
const string blue = "\033[34m";
const string reset = "\033[0m";

void askSave(Image& image){
    string filename;
    char saveChoice;
    cout << red << "Do you want to save? (y/n): " << reset;
    cin >> saveChoice;
    if (saveChoice == 'y' || saveChoice == 'Y') {
        cout << "Enter filename: ";
        cin >> filename;
        image.saveImage(filename);
    }
}

void showMenu(){

    cout 
        << yellow << " ______    _______            _________  _         _______  _________\n"
        << yellow << "(  __  \\  (  ___  ) |\\     /| \\__   __/ ( (    /| (  ____ \\ \\__   __/\n"
        << orange << "| (  \\  ) | (   ) | | )   ( |    " << yellow << "| |    |  \\  ( | | (    \\/    ) (   \n"
        << red << "| |   ) | | (___) | | |   | |    | |    |   \\ | | | |          | |   \n"
        << orange << "| |   | | |  ___  | ( (   ) )    | |    | (\\ \\) | | " << yellow << "|          | |   \n"
        << red << "| |   ) | | (   ) |  \\ \\_/ /     | |    | | \\   | | |          | |   \n"
        << red << "| (__/  ) | )   ( |   " << blue << "\\   /   ___) (___ | )  \\  " << red << "| | (____/\\ ___) (___\n"
        << red << "(______/ " << blue << " |/     \\|    \\_/    \\_______/ |/    )_)" << red << " (_______/ \\_______/\n"
        << reset;


    string filename;
    cout << "Load Image File: ";
    cin >> filename;

    Image image(filename);
    while (true) {
        int choose;
        cout << endl << "======================================================================\n";
        cout << orange << "Enter your choice\n" << reset;
        cout  << yellow << R"(
        0.  Load a new image
        1.  Grayscale Conversion
        2.  Black and White
        3.  Invert Image
        4.  Merge Images
        5.  Flip Image
        6.  Rotate Image
        7.  Darken and Lighten
        8.  Crop Image
        9.  Add Frame
        10. Edge Detection
        11. Resize Image
        12. Gaussian Blur
        13. Save the image
        14. Exit
        Your Option: )" << reset;

        cin >> choose;
        // TODO: CHECK IF FILE EXISTS (TRY & CATCH)
        if (choose == 0) {
        // the if statement that was here doesn't let the user load a new image without having to close the program
        //  if the user wants to save, he could have saved from the menu before this. Here, he loads directly. 
          cout << red << "Load new image file: " << reset;
          cin >> filename;
          image.loadNewImage(filename); // used built in loading function for images 
                                  
		    }
        else if (choose == 1) {
            applyGrayscale(image);
            cout << "Grayscale filter applied.\n";
            askSave(image);
        }
        else if (choose == 2) {
            applyBlackWhite(image);
            cout << "Black and White filter applied.\n";
            askSave(image);
        }
        else if (choose == 3) {
            applyInvertColors(image);
            cout << "Invert Colors filter applied.\n";
            askSave(image);
        }
        else if (choose == 4) {
            string secondFilename;
	          cout << red << "NOTE:" << reset;
            cout << " Images must be of the same size to merge!\n";
            cout << "Enter second image filename to merge:  \n";
            cin >> secondFilename;
            Image secondImage(secondFilename);
            applyMerge(image, secondImage);
            askSave(image); // removed a success message from here so that it only applies if the merge is done.
        }
        else if (choose == 5) {
            while (true){
                cout << "Horizontal or Vertical (h/v): ";
                string fchoice;
                cin >> fchoice;
                if (fchoice == "h"){
                    applyFlipImage(image, true);
                    break;
                }
                else if (fchoice == "v"){
                    applyFlipImage(image, false);
                    break;
                }
                else {
                    cout << red << "Invalid Argument\n" << reset;
                }
            }
            cout << "Flip filter applied.\n";
            askSave(image);
        }
        else if (choose == 6) {
            cout << "Angle (90, 180, or 270): ";
            int angle;
            cin >> angle;
            int ncounts = angle/90;
            for (int i = 0; i < ncounts; i++) {
              applyRotateImage(image);
            } 
            cout << "Rotation applied.\n";
            askSave(image);
        }
        else if (choose == 7) {
            double range;
            cout << "Enter value between -100 (darken) and +100 (lighten):  \n";
            cin >> range;
            applyDarkenLighten(image, range);
            cout << "Darken/Lighten filter applied.\n";
            askSave(image);
        }
        else if (choose == 8) {
            int x, y, width, height;
            cout << "Enter the x and y position of the crop, separated by a space:  \n";
            cin >> x >> y;
            cout << "Enter the width and height of the crop, separated by a space:  \n";
            cin >> width >> height; 
            applyCropImage(image, x, y, width, height);
            cout << "Crop image filter applied.\n";
            askSave(image);
        }
        else if (choose == 9) { // NOTE: ADD BOUNDARY CHECK FOR RGB VALUES AND GENERAL ERROR HANDLING
            int thickness, r, g, b;
            bool decoration;
            cout << "Enter the thickness of the frame:  \n";
            cin >> thickness;
            cout << "Enter the rgb values for the frame, separated by a space:  \n";
            cin >> r >> g >> b;
            cout << "Enter 0 for a normal frame, 1 for a decorated frame:  \n";
            cin >> decoration;
            applyAddColoredFrame(image, thickness, r, g, b, decoration);
            cout << "Colored frame filter applied.\n";
            askSave(image);
        }
        else if (choose == 10) {
            applyDetectEdges(image);
            cout << "Detect edge filter applied.\n";
            askSave(image);
        }
        else if (choose == 11) {
            int width, height;
            cout << "Enter the desired width and height, separated by a space:  \n";
            cin >> width >> height; 
            applyResizeImage(image, width, height);
            cout << "Resize image filter applied.\n";
            askSave(image);
        }
        else if (choose == 12) {
            int kernelSize;
            double sigma;
            cout << "Enter the blur-kernel size (values closer to 3-7 are much quicker):  \n";
            cin >> kernelSize;
            cout << "Enter the sigma value (higher = more blur):  \n";
            cin >> sigma;
            applyGaussianBlur(image, kernelSize, sigma);
            cout << "Gaussian blur image filter applied.\n";
            askSave(image);
        }
        else if (choose == 13) {
            cout << "Enter filename to save (with extension .jpg/.png/.bmp): ";
            cin >> filename;
            image.saveImage(filename);
            cout << "Image saved successfully!\n";
        }
        else if (choose == 14) {
            cout << red << "Do you want to save before exit? (y/n): " << reset;
            char saveChoice;
            cin >> saveChoice;
            if (saveChoice == 'y' || saveChoice == 'Y') {
                cout << "Enter filename: ";
                cin >> filename;
                image.saveImage(filename);
            }
            cout << blue << "Exiting program. Bye!\n" << reset;
            break;
        }
    }
}
