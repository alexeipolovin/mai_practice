#ifndef OGLWIDG_H
#define OGLWIDG_H

#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QWidget>


struct VertexData {
    VertexData() {

    }
    VertexData(QVector3D p, QVector3D coord,  QVector3D n) : pos(p), coords(coord), normal(n)
    {

    }
    QVector3D pos;
    QVector2D coords;
    QVector3D normal;
};

class OGLWidg : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit OGLWidg(QWidget *parent = nullptr);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void initShaders();
    void initCube(float width);

private:
    QMatrix4x4 matrixOfProjection;
    QOpenGLShaderProgram prog;
    QOpenGLTexture *texture;
    QOpenGLBuffer arrayBuff;
    QOpenGLBuffer indexBuff;
signals:

};

#endif // OGLWIDG_H
