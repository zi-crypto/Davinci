// Here we will implement Menu, Load & Save functions
#include <iostream>
#include <string>
#include "Image_Class.h"
#include "filters.h"
using namespace std;

void askSave(Image& image){
    string filename;
    char saveChoice;
    cout << "Do you want to save? (y/n): ";
    cin >> saveChoice;
    if (saveChoice == 'y' || saveChoice == 'Y') {
        cout << "Enter filename: ";
        cin >> filename;
        image.saveImage(filename);
    }
}

void showMenu(){
    const string red = "\033[31m";
    const string orange = "\033[38;5;208m"; 
    const string yellow = "\033[33m";
    const string blue = "\033[34m";
    const string reset = "\033[0m";

    cout 
        << yellow << " __   ___          ___ _        ___ ___\n"
        << yellow << "(  _  \\ (  __  )|\\     /|\\_   _/( (    /|(  __ \\\\_   _/\n"
        << orange << "| (  \\  )| (   ) || )   ( |   " << yellow << "| |   |  \\  ( || (    \\/   ) (   \n"
        << red << "| |   ) || (_) || |   | |   | |   |   \\ | || |         | |   \n"
        << orange << "| |   | ||  _  |( (   ) )   | |   | (\\ \\) || " << yellow << "|         | |   \n"
        << red << "| |   ) || (   ) | \\ \\_/ /    | |   | | \\   || |         | |   \n"
        << red << "| (_/  )| )   ( |  " << blue << "\\   /  _) (_| )  \\  " << red << "|| (_/\\_) (_\n"
        << red << "(__/ " << blue << "|/     \\|   \\/   \\__/|/    ))" << red << "(__/\\___/\n"
        << reset;

    string filename;
    cout << "Load Image File: ";
    cin >> filename;

    Image image(filename);
    while (true) {
        int choose;
        cout << "Enter your choice\n";
        cout << R"(
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
        12. Blur Image
        13. Save the image
        14. Exit
        Your Option: )";

        cin >> choose;
        // TODO: CHECK IF FILE EXISTS (TRY & CATCH)
        if (choose == 0) {
            if (image){
                askSave(image);
            }
            else {
                cout << "Load Image File: ";
                cin >> filename;
                Image image(filename);
            }
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
            cout << "Enter second image filename to merge:  \n";
            cin >> secondFilename;
            Image secondImage(secondFilename);
            applyMerge(image, secondImage);
            cout << "Merge filter applied.\n";
            askSave(image);
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
                    cout << "Invalid Argument";
                }
            }
            cout << "Flip filter applied.\n";
            askSave(image);
        }
        else if (choose == 6) {
            cout << "Angle: ";
            int angle;
            cin >> angle;
            int ncounts = angle/90;
            applyRotateImage(image, ncounts);
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
        else if (choose == 10) {
            applyDetectEdges(image);
            cout << "Detect edge filter applied.\n";
            askSave(image);
        }
        else if (choose == 13) {
            cout << "Enter filename to save (with extension .jpg/.png/.bmp/.tga): ";
            cin >> filename;
            image.saveImage(filename);
            cout << "Image saved successfully!\n";
        }
        else if (choose == 14) {
            cout << "Do you want to save before exit? (y/n): ";
            char saveChoice;
            cin >> saveChoice;
            if (saveChoice == 'y' || saveChoice == 'Y') {
                cout << "Enter filename: ";
                cin >> filename;
                image.saveImage(filename);
            }
            cout << "Exiting program. Bye!\n";
            break;
        }
    }
}