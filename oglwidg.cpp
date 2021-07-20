#include "oglwidg.h"
#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QVector>
OGLWidg::OGLWidg(QWidget *parent) : QOpenGLWidget(parent), texture(0), indexBuff(QOpenGLBuffer::IndexBuffer)
{

}

void OGLWidg::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShaders();
    initCube(1.0f);
}

void OGLWidg::resizeGL(int w, int h)
{
    float aspect = w/(float) h;
    matrixOfProjection.setToIdentity();
    matrixOfProjection.perspective(45, aspect, 0.1f, 10.0f);
}

void OGLWidg::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 matrixModel;
    matrixModel.setToIdentity();
    matrixModel.translate(0.0, 0.0, -5.0);

    texture->bind(0);

    prog.bind();
    prog.setUniformValue("qt_ModelViewProjectionMatrix", matrixOfProjection * matrixModel);
    prog.setUniformValue("qt_Texture0", 0);

    int offset = 0;

    int vertLoc = prog.attributeLocation("qt_Vertex");
    prog.enableAttributeArray(vertLoc);
    prog.setAttributeBuffer(vertLoc, GL_FLOAT, offset, 2, sizeof (VertexData));

    offset = sizeof (QVector3D);

    int texLoc = prog.attributeLocation("qt_MultiTexCoord0");
    prog.enableAttributeArray(texLoc);
    prog.setAttributeBuffer(texLoc, GL_FLOAT, offset, 2, sizeof(VertexData));

    indexBuff.bind();

    glDrawElements(GL_TRIANGLES, indexBuff.size(), GL_UNSIGNED_INT, 0);
}

void OGLWidg::initShaders()
{
    if(!prog.addShaderFromSourceFile(QOpenGLShader::Vertex, "qrc:/vshader.vsh"))
        close();
    if(!prog.addShaderFromSourceFile(QOpenGLShader::Fragment, "qrc:/fshader.fsh"))
        close();
    if(!prog.link())
        close();
}

void OGLWidg::initCube(float width)
{
    float divWidth = width/2.0;
    QVector<VertexData> vertexes = *new QVector<VertexData>;

    vertexes.append(VertexData(QVector3D(-divWidth, divWidth, divWidth), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-divWidth, -divWidth, divWidth), QVector2D(0.0, 0.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(divWidth, divWidth, divWidth), QVector2D(1.0, 1.0), QVector3D(0.0, 0.0, 1.0)));
    vertexes.append(VertexData(QVector3D(divWidth, -divWidth, divWidth), QVector2D(1.0, 0.0), QVector3D(0.0, 0.0, 1.0)));

    vertexes.append(VertexData(QVector3D(divWidth, divWidth, divWidth), QVector2D(0.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(divWidth, -divWidth, divWidth), QVector2D(0.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(divWidth, divWidth, -divWidth), QVector2D(0.0, 1.0), QVector3D(1.0, 0.0, 0.0)));
    vertexes.append(VertexData(QVector3D(divWidth, -divWidth, -divWidth), QVector2D(0.0, 1.0), QVector3D(1.0, 0.0, 0.0)));

    vertexes.append(VertexData(QVector3D(divWidth, divWidth, divWidth), QVector2D(0.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(divWidth, divWidth, -divWidth), QVector2D(0.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-divWidth, divWidth, divWidth), QVector2D(0.0, 1.0), QVector3D(0.0, 1.0, 0.0)));
    vertexes.append(VertexData(QVector3D(-divWidth, divWidth, -divWidth), QVector2D(0.0, 1.0), QVector3D(0.0, 1.0, 0.0)));

    vertexes.append(VertexData(QVector3D(divWidth, divWidth, -divWidth), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(divWidth, -divWidth, -divWidth), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-divWidth, divWidth, -divWidth), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, -1.0)));
    vertexes.append(VertexData(QVector3D(-divWidth, -divWidth, -divWidth), QVector2D(0.0, 1.0), QVector3D(0.0, 0.0, -1.0)));

    vertexes.append(VertexData(QVector3D(-divWidth, divWidth, divWidth), QVector2D(0.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
        vertexes.append(VertexData(QVector3D(-divWidth, divWidth, -divWidth), QVector2D(0.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
        vertexes.append(VertexData(QVector3D(-divWidth, -divWidth, divWidth), QVector2D(0.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));
        vertexes.append(VertexData(QVector3D(-divWidth, -divWidth, -divWidth), QVector2D(0.0, 1.0), QVector3D(-1.0, 0.0, 0.0)));

        vertexes.append(VertexData(QVector3D(-divWidth, -divWidth, divWidth), QVector2D(0.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
        vertexes.append(VertexData(QVector3D(-divWidth, -divWidth, -divWidth), QVector2D(0.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
        vertexes.append(VertexData(QVector3D(divWidth, -divWidth, divWidth), QVector2D(0.0, 1.0), QVector3D(0.0, -1.0, 0.0)));
        vertexes.append(VertexData(QVector3D(divWidth, -divWidth, -divWidth), QVector2D(0.0, 1.0), QVector3D(0.0, -1.0, 0.0)));


        QVector<GLuint> indexes;
        for(int i = 0; i < 24; i+=4) {
        indexes.append(i + 0);
        indexes.append(i + 1);
        indexes.append(i + 2);
        indexes.append(i + 2);
        indexes.append(i + 1);
        indexes.append(i + 3);
        }

        arrayBuff.create();
        arrayBuff.bind();
        arrayBuff.allocate(vertexes.constData(), vertexes.size() + sizeof(VertexData));
        arrayBuff.release();

        indexBuff.create();
        indexBuff.bind();
        indexBuff.allocate(indexes.constData(), indexes.size() + sizeof(GLuint));
        indexBuff.release();

        texture = new QOpenGLTexture(QImage(":/cube.png").mirrored());

        texture->setMinificationFilter(QOpenGLTexture::Nearest);
        texture->setMinificationFilter(QOpenGLTexture::Linear);
        texture->setWrapMode(QOpenGLTexture::Repeat);

}
