#include "m3dpreview.h"

#include <QCamera>
#include <QMaterial>
#include <QMesh>
#include <QOrbitCameraController>
#include <QPhongMaterial>
#include <QVector3D>

M3DPreview::M3DPreview() : Qt3DExtras::Qt3DWindow()
{
    Qt3DCore::QEntity *scene = createScene();

    // Camera
    Qt3DRender::QCamera *camera = this->camera();
    camera->lens()->setPerspectiveProjection(50.0f, 100.0f/50.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(-30, 0, 100));
    camera->setViewCenter(QVector3D(20, 40, 0));
    // For camera controls
    Qt3DExtras::QOrbitCameraController *camController = new Qt3DExtras::QOrbitCameraController(scene);
    camController->setLinearSpeed(180.0f);
    camController->setLookSpeed( 180.0f );
    camController->setCamera(camera);

    setRootEntity(scene);

}

void M3DPreview::changeSource(QString path)
{
    chestMesh->setSource(QUrl::fromLocalFile(path));
}

Qt3DCore::QEntity *M3DPreview::createScene()
{
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity;

    Qt3DRender::QMaterial *material = new Qt3DExtras::QPhongMaterial(rootEntity);

    Qt3DCore::QEntity *chestEntity = new Qt3DCore::QEntity(rootEntity);
    chestMesh = new Qt3DRender::QMesh(rootEntity);
    chestMesh->setSource(QUrl::fromLocalFile("air4.obj"));

    chestEntity->addComponent(chestMesh);
    chestEntity->addComponent(material);

    return rootEntity;
}
