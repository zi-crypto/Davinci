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


//====================================================
Image QImageToImage(const QImage &qimg) {
    Image img(qimg.width(), qimg.height());

    for (int y = 0; y < qimg.height(); ++y) {
        const uchar *row = qimg.scanLine(y);
        for (int x = 0; x < qimg.width(); ++x) {
            for (int c = 0; c < 3; ++c) {
                img.setPixel(x, y, c, row[x*4 + c]); 
            }
        }
    }

    return img;
}

QImage ImageToQImage(const Image &img) {
    QImage qimg(img.width, img.height, QImage::Format_RGB32);

    for (int y = 0; y < img.height; ++y) {
        uchar *row = qimg.scanLine(y);
        for (int x = 0; x < img.width; ++x) {
            row[x*4 + 0] = img(x, y, 0); 
            row[x*4 + 1] = img(x, y, 1); 
            row[x*4 + 2] = img(x, y, 2); 
            row[x*4 + 3] = 255;          
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

    createFilterControls(); 
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
    QMessageBox::about(this, tr("About Image Viewer"),
            tr("<p>The <b>Image Viewer</b> example shows how to combine QLabel "
               "and QScrollArea to display an image. QLabel is typically used "
               "for displaying a text, but it can also display an image. "
               "QScrollArea provides a scrolling view around another widget. "
               "If the child widget exceeds the size of the frame, QScrollArea "
               "automatically provides scroll bars. </p><p>The example "
               "demonstrates how QLabel's ability to scale its contents "
               "(QLabel::scaledContents), and QScrollArea's ability to "
               "automatically resize its contents "
               "(QScrollArea::widgetResizable), can be used to implement "
               "zooming and scaling features. </p><p>In addition the example "
               "shows how to use QPainter to print an image.</p>"));
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
    // TODO: implement file saving
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
    QWidget *dockWidget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(dockWidget);

    auto addFilterButton = [&](const QString &name, std::function<void(Image&)> filterFunc) {
        QGroupBox *group = new QGroupBox(name);
        QVBoxLayout *groupLayout = new QVBoxLayout;

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

    addFilterButton("Grayscale", [](Image &img){ applyGrayscale(img); });
    addFilterButton("Merge (manual second image)", [](Image &img){ /* TODO */ });
    addFilterButton("Darken / Lighten", [](Image &img){ applyDarkenLighten(img, 0.0); });
    addFilterButton("Detect Edges", [](Image &img){ applyDetectEdges(img); });
    addFilterButton("TV Effect", [](Image &img){ applyTV(img); });
    addFilterButton("Red Tint", [](Image &img){ applyRedTint(img, 1.0f); });
    addFilterButton("Green Tint", [](Image &img){ applyGreenTint(img); });
    addFilterButton("Black & White", [](Image &img){ applyBlackWhite(img); });
    addFilterButton("Flip Horizontal", [](Image &img){ applyFlipImage(img, true); });
    addFilterButton("Flip Vertical", [](Image &img){ applyFlipImage(img, false); });
    addFilterButton("Crop Image", [](Image &img){ applyCropImage(img, 0, 0, img.width, img.height); });
    addFilterButton("Resize Image", [](Image &img){ applyResizeImage(img, 400, 400); });
    addFilterButton("Infrared", [](Image &img){ applyInfrared(img); });
    addFilterButton("Skew", [](Image &img){ applySkew(img, 45.0); });
    addFilterButton("Invert Colors", [](Image &img){ applyInvertColors(img); });
    addFilterButton("Rotate", [](Image &img){ applyRotateImage(img); });
    addFilterButton("Add Colored Frame", [](Image &img){ applyAddColoredFrame(img, 5, 255, 0, 0, true); });
    addFilterButton("Gaussian Blur", [](Image &img){ applyGaussianBlur(img, 3, 5.0); });
    addFilterButton("Sunlight", [](Image &img){ applySunlight(img, 1.1); });
    addFilterButton("Oil Painting", [](Image &img){ applyOilPainting(img, 3, 12, 0.75, 3, 1.8, 7, 37); });

    layout->addStretch();
    dockWidget->setLayout(layout);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(dockWidget);

    dock->setWidget(scrollArea);
    addDockWidget(Qt::RightDockWidgetArea, dock);
}

