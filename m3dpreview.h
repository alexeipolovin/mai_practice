#ifndef M3DPREVIEW_H
#define M3DPREVIEW_H

#include <QEntity>
#include <QMesh>
#include <Qt3DWindow>



class M3DPreview : public Qt3DExtras::Qt3DWindow
{
public:
    M3DPreview();
    Qt3DCore::QEntity *createScene();
    Qt3DRender::QMesh *chestMesh;
    QString name;
    void changeSource(QString path);
};

#endif // M3DPREVIEW_H
