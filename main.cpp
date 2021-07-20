#include "m3dpreview.h"
#include "mainwindow.h"

#include <QApplication>
#include <QCamera>
#include <QEntity>
#include <QMaterial>
#include <QMesh>
#include <QOrbitCameraController>
#include <QPhongMaterial>
#include <Qt3DWindow>
#include <qguiapplication.h>



int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    M3DPreview *view = new M3DPreview();

    MainWindow *mainWindow = new MainWindow();

    mainWindow->show();

//    view->show();


    return app.exec();
}
