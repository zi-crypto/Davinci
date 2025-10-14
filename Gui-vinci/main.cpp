#include "mainwindow.h"
#include <QApplication>
#include "imageviewer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ImageViewer viewer;
    viewer.show();
    QPalette dark;
    dark.setColor(QPalette::Window, QColor(34, 34, 34));
    dark.setColor(QPalette::WindowText, Qt::white);
    dark.setColor(QPalette::Base, QColor(24, 24, 24));
    dark.setColor(QPalette::AlternateBase, QColor(45, 45, 45));
    dark.setColor(QPalette::ToolTipBase, Qt::white);
    dark.setColor(QPalette::ToolTipText, Qt::black);
    dark.setColor(QPalette::Text, Qt::white);
    dark.setColor(QPalette::Button, QColor(45, 45, 45));
    dark.setColor(QPalette::ButtonText, Qt::white);
    dark.setColor(QPalette::BrightText, Qt::red);
    dark.setColor(QPalette::Highlight, QColor(90, 120, 200));
    dark.setColor(QPalette::HighlightedText, Qt::black);

    app.setPalette(dark);

    app.setStyleSheet(R"(
    QMainWindow {
        background-color: #222;
        color: #eee;
    }

    QPushButton {
        background-color: #D98C41;
        color: #fff;
        border: 1px solid #666;
        border-radius: 6px;
        padding: 6px;
    }

    QPushButton:hover {
        background-color: #a65a11;
    }

    QGroupBox {
        border: 1px solid #555;
        margin-top: 10px;
        border-radius: 8px;
        padding: 6px;
        color: #eee;
        font-weight: bold;
    }

    QDockWidget {
        titlebar-close-icon: none;
        titlebar-normal-icon: none;
        background-color: #2b2b2b;
        border: 1px solid #444;
    }

    QScrollArea {
        background-color: #2b2b2b;
        border: none;
    }

    QSlider::groove:horizontal {
        height: 6px;
        background: #444;
        border-radius: 3px;
    }

    QSlider::handle:horizontal {
        background: #888;
        width: 12px;
        border-radius: 6px;
        margin: -3px 0;
    }

    QLabel {
        color: #ddd;
    }

    QMenuBar, QMenu {
        background-color: #333;
        color: #eee;
        border: none;
    }

    QMenu::item:selected {
        background-color: #555;
    }
)");

    return app.exec();
}

