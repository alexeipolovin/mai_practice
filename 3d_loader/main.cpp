#include <QCamera>
#include <QDirectionalLight>
#include <QEntity>
#include <QMaterial>
#include <QMesh>
#include <QOrbitCameraController>
#include <QPhongMaterial>
#include <QPointLight>
#include <Qt3DWindow>
#include <qguiapplication.h>
#include <QForwardRenderer>
#include <QTexture>
#include <QTexture>
#include <QParameter>
Qt3DCore::QEntity *createScene()
{
    // Root entity
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity;

    // Material
    Qt3DRender::QMaterial *material = new Qt3DExtras::QPhongMaterial(rootEntity);

    // Chest Entity
    Qt3DCore::QEntity *chestEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QMesh *chestMesh = new Qt3DRender::QMesh(rootEntity);
    chestMesh->setSource(QUrl::fromLocalFile("airplane.obj"));
    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    auto texture = new Qt3DRender::QTexture2D(rootEntity);
    auto image = new Qt3DRender::QTextureImage( texture );
    image->setSource(QUrl::fromLocalFile("airplane.jpg"));
    texture->addTextureImage(image);

    Qt3DRender::QDirectionalLight *light = new Qt3DRender::QDirectionalLight(lightEntity);
    light->setColor("black");
    light->setIntensity(0.75);
    lightEntity->addComponent(light);

    chestEntity->addComponent(chestMesh);
    chestEntity->addComponent(material);
    chestEntity->addComponent(light);

    return rootEntity;
}

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);
    Qt3DExtras::Qt3DWindow view;

    Qt3DCore::QEntity *scene = createScene();

    // Camera
    Qt3DRender::QCamera *camera = view.camera();
    camera->lens()->setPerspectiveProjection(100.0f, 40.0f/9.0f, 1.0f, 2000.0f);
    camera->setPosition(QVector3D(1, 0, 1));
    camera->setViewCenter(QVector3D(0, 0, 0));
    camera->rotate(QQuaternion(0,0,0,0));

    // For camera controls
    Qt3DExtras::QOrbitCameraController *camController = new Qt3DExtras::QOrbitCameraController(scene);
    camController->setLinearSpeed( 200.0f );
    camController->setLookSpeed( 180.0f );
    camController->setCamera(camera);

    view.defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d9f)));

    view.setRootEntity(scene);
    view.show();

    return app.exec();
}
