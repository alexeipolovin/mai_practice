#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDirModel>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QSettings>
#include <QTextEdit>
#include <QToolBar>
#include <QTreeView>
#include <QFileSystemModel>
#include <QSystemTrayIcon>
#include <markdownhighlighter.h>
//#include <QWebEngineView>
#include "untextedit.h"
#include "webconnector.h"
//#include "libraries/qjsontreeview/qjsonmodel.h"
#include "socketclient.h"
#include "appearancesettings.h"
#include "m3dpreview.h"

#define TYPE_SETTINGS "settings/file_type"
#define FIRST_STARTUP "settings/first_startup"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QString syncType = "Binary";

    AppearanceSettings *w;
    QSystemTrayIcon *systemTrayIcon;

    SocketClient *socketClient;

    WebConnector *webConnector;

    QToolBar *toolBar;
    QToolBar *createToolbar();

    QWidget *mainWidget;
    QHBoxLayout *mainLayout;

    UnTextEdit *textEdit;

    QTextEdit *previewTextEdit;
    // QWebEnginePage *previewTextEdit;
    QTreeView *view;
    // QJsonModel *model;
    // QFileSystemModel *model;
    QDirModel *model;

    QSettings *settings;

    QString fileName;

    bool shown = false;

public:
    M3DPreview *m3DPreview;

    bool livePreview = false;
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    MarkdownHighlighter *highliter;

    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void connectAll();
    bool lightTheme;
signals:
    void lightThemeEnabled();
public slots:

    void updateUnknown();

    void TreeViewDoubleClick(const QModelIndex &index);
};
#endif // MAINWINDOW_H
