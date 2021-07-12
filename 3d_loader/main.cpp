#include <QtGui/QGuiApplication>
#include <QtGui/QKeyEvent>

#include <QtGui/QOpenGLWindow>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLVertexArrayObject>

#include <QOpenGLFunctions>

int main(int argc, char *argv[])
{
    QGuiApplication a(argc,argv);
    return a.exec();
}
