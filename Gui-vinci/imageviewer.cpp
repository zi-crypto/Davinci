#include "imageviewer.h"
#include "filters.h"
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QImageReader>
#include <QImageWriter>
#include <QPixmap>
#include <QApplication>
#include <QScreen>
#include <QStandardPaths>
#include <QDir>
#include <QPainter>
#include <QPrintDialog>
#include <QPrinter>
#include <QGroupBox>
#include <QStatusBar>
#include <QDesktopServices>
#include <QUrl>


//====================================================
Image QImageToImage(const QImage &qimg) {
    Image img(qimg.width(), qimg.height());

    for (int y = 0; y < qimg.height(); ++y) {
        const uchar *row = qimg.scanLine(y);
        for (int x = 0; x < qimg.width(); ++x) {
            int idx = x * 4; // 4 bytes per pixel (BGRA)
            uchar b = row[idx + 0];
            uchar g = row[idx + 1];
            uchar r = row[idx + 2];
            img.setPixel(x, y, 0, r);
            img.setPixel(x, y, 1, g);
            img.setPixel(x, y, 2, b);
        }
    }

    return img;
}


QImage ImageToQImage(const Image &img) {
    QImage qimg(img.width, img.height, QImage::Format_RGB32);

    for (int y = 0; y < img.height; ++y) {
        uchar *row = qimg.scanLine(y);
        for (int x = 0; x < img.width; ++x) {
            int idx = x * 4;
            uchar r = img(x, y, 0);
            uchar g = img(x, y, 1);
            uchar b = img(x, y, 2);

            // Qt expects BGRA layout
            row[idx + 0] = b;
            row[idx + 1] = g;
            row[idx + 2] = r;
            row[idx + 3] = 255;  // Alpha
        }
    }

    return qimg;
}

//====================================================


ImageViewer::ImageViewer(QWidget *parent)
    : QMainWindow(parent), imageLabel(new QLabel), scrollArea(new QScrollArea),
      scaleFactor(1.0)
{
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    scrollArea->setVisible(false);
    setCentralWidget(scrollArea);

    createActions();

    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);

    // Set window title
    setWindowTitle(tr("DaVinci Image Editor"));
    
    // Apply modern styling
    setStyleSheet(
        "QDockWidget { font-size: 11pt; }"
        "QPushButton { "
        "   background-color: #D97657; "
        "   color: white; "
        "   border: none; "
        "   border-radius: 4px; "
        "   padding: 8px; "
        "   font-size: 10pt; "
        "}"
        "QPushButton:hover { background-color: #A35E47; }"
        "QPushButton:pressed { background-color: #7B4836; }"
        "QGroupBox { "
        "   font-weight: bold; "
        "   border: 1px solid #bdc3c7; "
        "   border-radius: 5px; "
        "   margin-top: 10px; "
        "   padding-top: 10px; "
        "}"
    );

    createFilterControls();
    
    // Show welcome message in status bar
    statusBar()->showMessage(tr("Ready - Open an image to begin editing"), 3000);
}

static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations =
            QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes =
        (acceptMode == QFileDialog::AcceptOpen ? QImageReader::supportedMimeTypes()
                                              : QImageWriter::supportedMimeTypes());
    for (const QByteArray &mimeTypeName : supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    dialog.setAcceptMode(acceptMode);
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}

void ImageViewer::open()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().constFirst())) {}
}

bool ImageViewer::loadFile(const QString &fileName)
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                     .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }

    image = newImage;
    imageLabel->setPixmap(QPixmap::fromImage(image));
    scaleFactor = 1.0;

    scrollArea->setVisible(true);
    printAct->setEnabled(true);
    fitToWindowAct->setEnabled(true);
    updateActions();

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();

    // Show success message
    statusBar()->showMessage(tr("Loaded: %1 (%2x%3)")
        .arg(QFileInfo(fileName).fileName())
        .arg(image.width())
        .arg(image.height()), 3000);

    return true;
}

void ImageViewer::print()
{
    Q_ASSERT(!imageLabel->pixmap(Qt::ReturnByValue).isNull());
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    if (dialog.exec()) {
        QPainter painter(&printer);
        QPixmap pixmap = imageLabel->pixmap(Qt::ReturnByValue);
        QRect rect = painter.viewport();
        QSize size = pixmap.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(pixmap.rect());
        painter.drawPixmap(0, 0, pixmap);
    }
#endif
}

void ImageViewer::zoomIn()
{
    scaleImage(1.25);
}

void ImageViewer::zoomOut()
{
    scaleImage(0.8);
}

void ImageViewer::normalSize()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void ImageViewer::fitToWindow()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow)
        normalSize();
    updateActions();
}

void ImageViewer::about()
{
    QMessageBox::about(this, tr("About DaVinci Image Editor"),
            tr("<h2>DaVinci Image Editor</h2>"
               "<p>A powerful image processing application with various filters and effects.</p>"
               "<p><b>Features:</b></p>"
               "<ul>"
               "<li>Color adjustments (Grayscale, B&W, Invert, Tints)</li>"
               "<li>Effects (Infrared, Sunlight, TV, Edge Detection, Blur, Oil Painting)</li>"
               "<li>Transformations (Rotate, Flip, Skew, Resize, Crop)</li>"
               "<li>Frames & Decorations</li>"
               "<li>Image Merging</li>"
               "</ul>"
               "<p>Created with Qt and C++</p>"));
}

void ImageViewer::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &ImageViewer::open);
    openAct->setShortcut(QKeySequence::Open);

    saveAsAct = fileMenu->addAction(tr("&Save As..."), this, &ImageViewer::saveAs);
    saveAsAct->setEnabled(false);

    printAct = fileMenu->addAction(tr("&Print..."), this, &ImageViewer::print);
    printAct->setShortcut(QKeySequence::Print);
    printAct->setEnabled(false);

    fileMenu->addSeparator();

    QAction *exitAct = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    copyAct = editMenu->addAction(tr("&Copy"), this, &ImageViewer::copy);
    copyAct->setShortcut(QKeySequence::Copy);
    copyAct->setEnabled(false);

    QAction *pasteAct = editMenu->addAction(tr("&Paste"), this, &ImageViewer::paste);
    pasteAct->setShortcut(QKeySequence::Paste);

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

    zoomInAct = viewMenu->addAction(tr("Zoom &In (25%)"), this, &ImageViewer::zoomIn);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setEnabled(false);

    zoomOutAct = viewMenu->addAction(tr("Zoom &Out (25%)"), this, &ImageViewer::zoomOut);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setEnabled(false);

    normalSizeAct = viewMenu->addAction(tr("&Normal Size"), this, &ImageViewer::normalSize);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);

    viewMenu->addSeparator();

    fitToWindowAct = viewMenu->addAction(tr("&Fit to Window"), this, &ImageViewer::fitToWindow);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    helpMenu->addAction(tr("&About"), this, &ImageViewer::about);
    helpMenu->addAction(tr("About &Qt"), this, &QApplication::aboutQt);
}

void ImageViewer::updateActions()
{
    saveAsAct->setEnabled(!image.isNull());
    copyAct->setEnabled(!image.isNull());
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void ImageViewer::scaleImage(double factor)
{
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap(Qt::ReturnByValue).size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

void ImageViewer::saveAs()
{
    if (imageLabel->pixmap(Qt::ReturnByValue).isNull()) {
        QMessageBox::information(this, tr("No Image"), tr("There is no image to save."));
        return;
    }

    QFileDialog dialog(this, tr("Save File As"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptSave);

    if (dialog.exec() == QDialog::Accepted) {
        QString fileName = dialog.selectedFiles().constFirst();
        
        QImage qimg = imageLabel->pixmap(Qt::ReturnByValue).toImage();
        Image img = QImageToImage(qimg);
        
        try {
            img.saveImage(fileName.toStdString());
            statusBar()->showMessage(tr("Image saved successfully: %1").arg(fileName), 3000);
        } catch (const std::exception& e) {
            QMessageBox::warning(this, tr("Save Error"), 
                tr("Could not save image: %1").arg(e.what()));
        }
    }
}

void ImageViewer::copy()
{
    // TODO: implement copy
}
void ImageViewer::paste()
{
    // TODO: implement save
}

/*
void ImageViewer::createFilterControls()
{
    QDockWidget *dock = new QDockWidget(tr("Filters"), this);
    QWidget *dockWidget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(dockWidget);

    auto addFilterButton = [&](const QString &name, std::function<void(Image&)> filterFunc, QList<QWidget*> params = {}) {
        QGroupBox *group = new QGroupBox(name);
        QVBoxLayout *groupLayout = new QVBoxLayout;

        for (QWidget* w : params)
            groupLayout->addWidget(w);

        QPushButton *btn = new QPushButton(tr("Apply"));
        groupLayout->addWidget(btn);
        group->setLayout(groupLayout);
        layout->addWidget(group);

        connect(btn, &QPushButton::clicked, this, [=]() {
            if (!imageLabel->pixmap(Qt::ReturnByValue).isNull()) {
                QImage qimg = imageLabel->pixmap(Qt::ReturnByValue).toImage();
                Image img = QImageToImage(qimg);

                filterFunc(img);

                imageLabel->setPixmap(QPixmap::fromImage(ImageToQImage(img)));
            }
        });
    };

    // ----------------- Samir's Filters -----------------
    addFilterButton("Grayscale", [](Image &img){ applyGrayscale(img); });

    // addFilterButton("Merge (select second image manually)", [](Image &img){  });

    QSlider *darkSlider = new QSlider(Qt::Horizontal);
    darkSlider->setRange(-100, 100); darkSlider->setValue(0);
    addFilterButton("Darken / Lighten", [=](Image &img){ applyDarkenLighten(img, darkSlider->value() / 100.0); }, {darkSlider});

    addFilterButton("Detect Edges", [](Image &img){ applyDetectEdges(img); });
    addFilterButton("TV Effect", [](Image &img){ applyTV(img); });

    QSlider *redSlider = new QSlider(Qt::Horizontal);
    redSlider->setRange(0, 100); redSlider->setValue(50);
    addFilterButton("Red Tint", [=](Image &img){ applyRedTint(img, redSlider->value() / 100.0f); }, {redSlider});

    addFilterButton("Green Tint", [](Image &img){ applyGreenTint(img); });

    // ----------------- Ziad's Filters -----------------
    addFilterButton("Black & White", [](Image &img){ applyBlackWhite(img); });
    addFilterButton("Flip Horizontal", [](Image &img){ applyFlipImage(img, true); });
    addFilterButton("Flip Vertical", [](Image &img){ applyFlipImage(img, false); });

   // addFilterButton("Crop Image (TODO file selection / params)", [](Image &img){ });

    QSlider *resizeWidth = new QSlider(Qt::Horizontal); resizeWidth->setRange(10, 2000); resizeWidth->setValue(400);
    QSlider *resizeHeight = new QSlider(Qt::Horizontal); resizeHeight->setRange(10, 2000); resizeHeight->setValue(400);
    addFilterButton("Resize Image", [=](Image &img){ applyResizeImage(img, resizeWidth->value(), resizeHeight->value()); }, {resizeWidth, resizeHeight});

    addFilterButton("Infrared", [](Image &img){ applyInfrared(img); });

    QSlider *skewSlider = new QSlider(Qt::Horizontal); skewSlider->setRange(0, 180); skewSlider->setValue(45);
    addFilterButton("Skew", [=](Image &img){ applySkew(img, skewSlider->value()); }, {skewSlider});

    // ----------------- Ammen's Filters -----------------
    addFilterButton("Invert Colors", [](Image &img){ applyInvertColors(img); });
    addFilterButton("Rotate", [](Image &img){ applyRotateImage(img); });

    QSlider *frameThickness = new QSlider(Qt::Horizontal); frameThickness->setRange(1, 50); frameThickness->setValue(5);
    addFilterButton("Add Colored Frame (default color)", [=](Image &img){ applyAddColoredFrame(img, frameThickness->value(), 255, 0, 0, true); }, {frameThickness});

    QSlider *blurSlider = new QSlider(Qt::Horizontal); blurSlider->setRange(1, 31); blurSlider->setValue(5);
    QSlider *sigmaSlider = new QSlider(Qt::Horizontal); sigmaSlider->setRange(1, 20); sigmaSlider->setValue(5);
    addFilterButton("Gaussian Blur", [=](Image &img){ applyGaussianBlur(img, blurSlider->value(), sigmaSlider->value()); }, {blurSlider, sigmaSlider});

    QSlider *sunSlider = new QSlider(Qt::Horizontal); sunSlider->setRange(5, 20); sunSlider->setValue(11);
    addFilterButton("Sunlight", [=](Image &img){ applySunlight(img, sunSlider->value() / 10.0); }, {sunSlider});

    QSlider *oilKernel = new QSlider(Qt::Horizontal); oilKernel->setRange(1, 7); oilKernel->setValue(3);
    QSlider *oilSigma = new QSlider(Qt::Horizontal); oilSigma->setRange(1, 50); oilSigma->setValue(12);
    addFilterButton("Oil Painting", [=](Image &img){ applyOilPainting(img, oilKernel->value(), oilSigma->value()); }, {oilKernel, oilSigma});

    dockWidget->setLayout(layout);
    dock->setWidget(dockWidget);
    addDockWidget(Qt::RightDockWidgetArea, dock);
}
*/ // ^^^ WITH SLIDERS: slightly malfunctional, experimental.


void ImageViewer::createFilterControls()
{
    QDockWidget *dock = new QDockWidget(tr("Filters"), this);
    dock->setMinimumWidth(250);
    QWidget *dockWidget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(dockWidget);

    auto addFilterButton = [&](const QString &name, std::function<void(Image&)> filterFunc, const QString &tooltip = "") {
        QPushButton *btn = new QPushButton(name);
        if (!tooltip.isEmpty()) {
            btn->setToolTip(tooltip);
        }
        btn->setMinimumHeight(35);
        layout->addWidget(btn);

        connect(btn, &QPushButton::clicked, this, [=]() {
            if (!imageLabel->pixmap(Qt::ReturnByValue).isNull()) {
                QImage qimg = imageLabel->pixmap(Qt::ReturnByValue).toImage();
                Image img = QImageToImage(qimg);
                
                // Apply filter
                filterFunc(img);
                
                // Update image
                imageLabel->setPixmap(QPixmap::fromImage(ImageToQImage(img)));
                
                // Show status message
                statusBar()->showMessage(tr("Applied: %1").arg(name), 2000);
            } else {
                statusBar()->showMessage(tr("Please open an image first!"), 2000);
            }
        });
    };

    auto addSectionLabel = [&](const QString &text) {
        QLabel *label = new QLabel(text);
        QFont font = label->font();
        font.setBold(true);
        font.setPointSize(10);
        label->setFont(font);
        label->setStyleSheet("QLabel { color: #262624; margin-top: 10px; }");
        layout->addWidget(label);
    };

    // ============== COLOR ADJUSTMENTS ==============
    addSectionLabel("🎨 Color Adjustments");
    
    addFilterButton("Grayscale", [](Image &img){ applyGrayscale(img); }, "Convert image to grayscale");
    addFilterButton("Black & White", [](Image &img){ applyBlackWhite(img); }, "Convert to pure black and white");
    addFilterButton("Invert Colors", [](Image &img){ applyInvertColors(img); }, "Invert all colors");
    
    addFilterButton("Darken / Lighten", [this](Image &img){ 
        bool ok;
        int value = QInputDialog::getInt(this, "Darken/Lighten", 
            "Enter percentage (-100 to darken, +100 to lighten):", 0, -100, 100, 10, &ok);
        if (ok) {
            applyDarkenLighten(img, value);
        }
    }, "Adjust brightness");

    addFilterButton("Red Tint", [this](Image &img){ 
        bool ok;
        int value = QInputDialog::getInt(this, "Red Tint", 
            "Enter tint intensity (0-100):", 50, 0, 100, 10, &ok);
        if (ok) {
            applyRedTint(img, value / 100.0f);
        }
    }, "Apply red color tint");
    
    addFilterButton("Green Tint", [](Image &img){ applyGreenTint(img); }, "Apply green color tint");

    // ============== EFFECTS & FILTERS ==============
    addSectionLabel("✨ Effects & Filters");
    
    addFilterButton("Infrared", [](Image &img){ applyInfrared(img); }, "Simulate infrared photography");
    addFilterButton("Sunlight", [](Image &img){ applySunlight(img, 1.1); }, "Add warm sunlight effect");
    addFilterButton("TV Effect", [](Image &img){ applyTV(img); }, "Apply TV scan lines effect");
    addFilterButton("Detect Edges", [](Image &img){ applyDetectEdges(img); }, "Detect and highlight edges");
    addFilterButton("Gaussian Blur", [](Image &img){ applyGaussianBlur(img, 5, 3.0); }, "Apply blur effect");
    addFilterButton("Oil Painting", [](Image &img){ applyOilPainting(img); }, "Transform to oil painting style");

    // ============== TRANSFORMATIONS ==============
    addSectionLabel("🔄 Transformations");
    
    addFilterButton("Rotate 90°", [](Image &img){ applyRotateImage(img); }, "Rotate image clockwise");
    addFilterButton("Flip Horizontal", [](Image &img){ applyFlipImage(img, true); }, "Mirror horizontally");
    addFilterButton("Flip Vertical", [](Image &img){ applyFlipImage(img, false); }, "Mirror vertically");
    
    addFilterButton("Skew", [this](Image &img){ 
        bool ok;
        int angle = QInputDialog::getInt(this, "Skew Image", 
            "Enter skew angle (0-180 degrees):", 45, 0, 180, 5, &ok);
        if (ok) {
            applySkew(img, angle);
        }
    }, "Skew the image");

    addFilterButton("Resize", [this](Image &img){ 
        bool ok;
        int width = QInputDialog::getInt(this, "Resize Image", 
            "Enter new width:", img.width, 10, 5000, 50, &ok);
        if (ok) {
            int height = QInputDialog::getInt(this, "Resize Image", 
                "Enter new height:", img.height, 10, 5000, 50, &ok);
            if (ok) {
                applyResizeImage(img, width, height);
            }
        }
    }, "Resize to custom dimensions");

    addFilterButton("Crop", [this](Image &img){ 
        bool ok;
        int x = QInputDialog::getInt(this, "Crop Image", 
            "Enter X position (left):", 0, 0, img.width-1, 10, &ok);
        if (ok) {
            int y = QInputDialog::getInt(this, "Crop Image", 
                "Enter Y position (top):", 0, 0, img.height-1, 10, &ok);
            if (ok) {
                int w = QInputDialog::getInt(this, "Crop Image", 
                    "Enter crop width:", img.width/2, 1, img.width-x, 10, &ok);
                if (ok) {
                    int h = QInputDialog::getInt(this, "Crop Image", 
                        "Enter crop height:", img.height/2, 1, img.height-y, 10, &ok);
                    if (ok) {
                        applyCropImage(img, x, y, w, h);
                    }
                }
            }
        }
    }, "Crop to specific area");

    // ============== FRAMES & DECORATION ==============
    addSectionLabel("🖼️ Frames & Decoration");
    
    addFilterButton("Add Colored Frame", [](Image &img){ 
        applyAddColoredFrame(img, 10, 255, 0, 0, true); 
    }, "Add decorative frame");

    // ============== ADVANCED ==============
    addSectionLabel("🔧 Advanced");
    
    addFilterButton("Merge Images", [this](Image &img){ 
        QString fileName = QFileDialog::getOpenFileName(this, 
            tr("Select Image to Merge"), "", tr("Images (*.png *.jpg *.jpeg *.bmp)"));
        if (!fileName.isEmpty()) {
            try {
                Image img2(fileName.toStdString());
                if (img.width == img2.width && img.height == img2.height) {
                    applyMerge(img, img2);
                } else {
                    QMessageBox::warning(this, "Error", 
                        "Images must have the same dimensions to merge!");
                }
            } catch (...) {
                QMessageBox::warning(this, "Error", "Failed to load merge image!");
            }
        }
    }, "Merge with another image");

    // ============== EXTERNAL TOOLS ==============
    // Add a standalone button that doesn't require an image
    QPushButton *geminiBtn = new QPushButton("HUG YOURSELF");
    geminiBtn->setToolTip("Open in browser");
    geminiBtn->setMinimumHeight(35);
    layout->addWidget(geminiBtn);
    
    connect(geminiBtn, &QPushButton::clicked, this, [this]() {
        QUrl url("https://gemini.google.com/share/ed81b794dc6f");
        if (!QDesktopServices::openUrl(url)) {
            QMessageBox::warning(this, "Error", 
                "Failed to open browser. Please visit: https://gemini.google.com/share/ed81b794dc6f");
        } else {
            statusBar()->showMessage(tr("Opening in browser..."), 2000);
        }
    });

    layout->addStretch();
    dockWidget->setLayout(layout);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(dockWidget);

    dock->setWidget(scrollArea);
    addDockWidget(Qt::RightDockWidgetArea, dock);
}

