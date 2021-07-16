#include <QCamera>
#include <QEntity>
#include <QMaterial>
#include <QMesh>
#include <QOrbitCameraController>
#include <QPhongMaterial>
#include <Qt3DWindow>
#include <qguiapplication.h>

Qt3DCore::QEntity *createScene()
{
    // Root entity
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity;

    // Material
    Qt3DRender::QMaterial *material = new Qt3DExtras::QPhongMaterial(rootEntity);

    // Chest Entity
    Qt3DCore::QEntity *chestEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QMesh *chestMesh = new Qt3DRender::QMesh(rootEntity);
    chestMesh->setSource(QUrl::fromLocalFile("Deer.obj"));

    chestEntity->addComponent(chestMesh);
    chestEntity->addComponent(material);

    return rootEntity;
}

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);
    Qt3DExtras::Qt3DWindow view;

    Qt3DCore::QEntity *scene = createScene();

    // Camera
    Qt3DRender::QCamera *camera = view.camera();
    camera->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(1, 1, 1));
    camera->setViewCenter(QVector3D(0, 0, 0));

    // For camera controls
    Qt3DExtras::QOrbitCameraController *camController = new Qt3DExtras::QOrbitCameraController(scene);
    camController->setLinearSpeed( 100.0f );
    camController->setLookSpeed( 180.0f );
    camController->setCamera(camera);

    view.setRootEntity(scene);
    view.show();

    return app.exec();
}
