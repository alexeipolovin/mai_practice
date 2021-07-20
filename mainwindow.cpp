/*
 * Copyright (c) 2021 Alexei Polovin (alexeipolovin@gmail.com)

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "mainwindow.h"
#include "settingswindow.h"
#include "markdownhighlighter.h"
#include "m3dpreview.h"

#include <QMenu>
#include <QToolButton>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QShortcut>
#include <QCloseEvent>
#include <QInputDialog>
#include <QMessageBox>
#include <QFontDialog>
#include <QStyleFactory>
#include <QApplication>
#include <QJsonObject>

#define AUTO_UPDATES "AUTO_UPDATES_AVAILABLE"

#define STANDART_TITLE_EDITED "* - KerNotes"
#define STANDART_TITLE " - KerNotes"

/**
 * @brief MainWindow::MainWindow
 * @param QWidget parent
 * @author Alexei Polovin(alexeipolovin@gmail.com)
 * Конструктор класса главного окна
*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    systemTrayIcon = new QSystemTrayIcon();
    socketClient = new SocketClient();
    textEdit = new UnTextEdit();
    textEdit->setTabStopDistance(QFontMetricsF(textEdit->font()).horizontalAdvance(' ') * 4);

    m3DPreview = new M3DPreview();

    m3DPreview->show();

    previewTextEdit = new QTextEdit();
    previewTextEdit->setContextMenuPolicy(Qt::NoContextMenu);
    previewTextEdit->setReadOnly(true);

    addToolBar(createToolbar());

    mainWidget = new QWidget();
    mainLayout = new QHBoxLayout();
    livePreview = false;

//    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "KerNotes",
//                                    tr("One side preview?\n"),
//                                    QMessageBox::No | QMessageBox::Yes,
//                                    QMessageBox::Yes);
//    if(resBtn == QMessageBox::Yes)
//    {
//        livePreview = true;
//        qDebug() << livePreview;

//    }
    settings = new QSettings("Kernux", "KerNotes");

    // if(settings->value(FIRST_STARTUP).toString() == "" || settings->value(TYPE_SETTINGS).toString() == "")
    // {
    QStringList items;
    items << "HTML";
    items << "MarkDown";
    items << "Txt";

    QInputDialog dialog;
    dialog.setOptions(QInputDialog::UseListViewForComboBoxItems);
    dialog.setComboBoxItems(items);
    dialog.setWindowTitle("Choose File Type");
    if (dialog.exec())
    {
        auto text = dialog.textValue();
        qDebug() << text;
        if(text == "HTML")
        {
            QMessageBox::warning(nullptr, "Warning", "HTML is in beta");
            textEdit->setTextType(1);
            settings->setValue(TYPE_SETTINGS, 1);
        } else if(text == "MarkDown")
        {
            textEdit->setTextType(2);
            settings->setValue(TYPE_SETTINGS, 2);
        } else if(text == "Txt")
        {
            textEdit->setTextType(3);
            settings->setValue(TYPE_SETTINGS, 3);
        }
        settings->setValue(FIRST_STARTUP, "0");
        // }
        // } else {
        //     textEdit->setTextType(settings->value(TYPE_SETTINGS).toInt());
        // }
    connect(textEdit, &UnTextEdit::textChanged, this, [this] ()
    {
        qDebug() << "Live preview:" << livePreview;
        qDebug() << "Text changed event";
       short textType = this->textEdit->getTextType();
       qDebug() << "Editing:"<< textType;
       textEdit->setIsTextChanged(true);
       setWindowTitle(textEdit->getFileName() + STANDART_TITLE_EDITED);
       // TODO: Optimize this
       if(!this->shown) {
       if(previewTextEdit->toPlainText().length() > 500000)
       {
           QMessageBox::warning(nullptr, "Too large file", "Your file is too large, live preview will be disabled");
           this->shown = true;
       } else {
           switch (textType) {
               case 1:
               if(!livePreview)
               {
                   qDebug() << this->textEdit->toHtml();
                   this->previewTextEdit->setHtml(this->textEdit->toHtml());
               }
                   break;
               case 2:
               if(!livePreview)
               {
                   this->previewTextEdit->setMarkdown(this->textEdit->toMarkdown());
               } else {
               }
                   break;
               default:
                   break;
           }
       }
       }

    });
//    connect(textEdit, &UnTextEdit::updateTextEdit, this, [this] ()
//    {
//        qDebug() << "One side update event";
//       short textType = this->textEdit->getTextType();
//       textEdit->setIsTextChanged(true);
//       setWindowTitle(textEdit->getFileName() + STANDART_TITLE_EDITED);
//       // TODO: Optimize this
//       if(!this->shown) {
//       if(previewTextEdit->toPlainText().length() > 500000)
//       {
//           QMessageBox::warning(nullptr, "Too large file", "Your file is too large, live preview will be disabled");
//           this->shown = true;
//       } else {
//           int cursorPosition = this->textEdit->textCursor().position();
//           switch (textType) {
//                case 1:
//                    if(livePreview)
//                    {
//                        qDebug() <<"CurrentText:" << this->textEdit->toHtml();
////                        this->textEdit->setHtml(this->textEdit->toHtml());
//                    }
//               break;
//                case 2:
//                    if(livePreview)
//                    {
//                        qDebug() <<"CurrentText:" << this->textEdit->toMarkdown();
////                        this->textEdit->setMarkdown(this->textEdit->toMarkdown());
//                    }
//               break;
//           default:
//               break;
//           }
//           QTextCursor cursor = this->textEdit->textCursor();
//           cursor.setPosition(cursorPosition);
//           this->textEdit->setTextCursor(cursor);
//
//       }
//       }
//
//    });
//    textEdit->setTextType(2);
    qDebug() << "UnTextEdit text type:" << textEdit->getTextType();

        }
    // TODO: Remove this
    if(textEdit->getTextType() == 2)
    {
        auto doc = this->textEdit->document();
        highliter = new MarkdownHighlighter(doc);


        qDebug() << highliter;
    }
//    QString json = R"({
//"name":"John", "age":31, "city":"New York"
//                   })";
    view = new QTreeView();
    // model = new QJsonModel();
    model = new QDirModel();
    view->setModel(model);
    view->setRootIndex(model->index(QDir::currentPath()));
    // model->loadJson(json.toUtf8());
    // TODO: fix?
    view->setFixedWidth(this->width() * 1/4);

    mainLayout->addWidget(view);
    mainLayout->addWidget(textEdit);
    if(!livePreview)
    {
        mainLayout->addWidget(previewTextEdit);
    }
    auto testButton = new QPushButton("Test");
    connect(testButton, &QPushButton::clicked, this, [this](){
        delete highliter;
    });
//    mainLayout->addWidget(testButton);
    mainWidget->setLayout(mainLayout);

    setCentralWidget(mainWidget);
    connect(view, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(TreeViewDoubleClick(const QModelIndex &)));

//    auto updateShortcut = new QShortcut(this);
//    updateShortcut->setKey(Qt::Key_Enter);
//    connect(updateShortcut, &QShortcut::activated, this, [this] (){
//       emit textEdit->textChanged();
//       qDebug() << "Text changed";
//    });
    auto *openShortcut = new QShortcut(this);
    openShortcut->setKey(Qt::CTRL + Qt::Key_O);
    connect(openShortcut, &QShortcut::activated, textEdit, &UnTextEdit::openFile);

    auto *increaseFontSize = new QShortcut(this);
    increaseFontSize->setKey(Qt::CTRL+Qt::Key_Minus);
    connect(increaseFontSize, &QShortcut::activated,textEdit,&UnTextEdit::increaseFontSize);

    auto *saveShortcut = new QShortcut(this);
    saveShortcut->setKey(Qt::CTRL + Qt::Key_S);
    connect(saveShortcut, &QShortcut::activated, textEdit, &UnTextEdit::saveFile);
    connect(textEdit, &UnTextEdit::fileSaved, this, [this](){
        setWindowTitle(this->textEdit->getFileName() + STANDART_TITLE);
    });

    auto imageInsertShortcut = new QShortcut(this);
    imageInsertShortcut->setKey(Qt::CTRL+Qt::Key_N);
    connect(imageInsertShortcut, &QShortcut::activated, textEdit, &UnTextEdit::insertImageSnippet);
    auto *boldShortcut = new QShortcut(this);
    boldShortcut->setKey(Qt::CTRL + Qt::Key_B);
    connect(boldShortcut, &QShortcut::activated, textEdit, &UnTextEdit::placeBoldText);

    auto *cursShortcut = new QShortcut(this);
    cursShortcut->setKey(Qt::CTRL + Qt::Key_I);
    connect(cursShortcut, &QShortcut::activated, textEdit, &UnTextEdit::placeCursText);

    auto *previewShortcut = new QShortcut(this);
    previewShortcut->setKey(Qt::CTRL + Qt::Key_P);
    connect(previewShortcut, &QShortcut::activated, this, [this]() {
        switch (this->textEdit->getTextType()) {
            case 1:
                this->previewTextEdit->setHtml(textEdit->toHtml());
//                if(livePreview)
//                {
//                    this->textEdit->setHtml(textEdit->toHtml());
//                }
                break;
            case 2:
                if(!livePreview)
                {
                    this->previewTextEdit->setMarkdown(textEdit->toMarkdown());
                }
                break;
        }
    });

    webConnector = new WebConnector();
    connect(webConnector, &WebConnector::autoUpdatesUnknown, this, &MainWindow::updateUnknown);
    webConnector->checkUpdates();

    connect(view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(TreeViewDoubleClick(QModelIndex)));

    if(settings->value("lasteditedfile").toString() != "")
    {
        qDebug() << settings->value("lasteditedfile").toString();
        this->textEdit->openLastFile(settings->value("lasteditedfile").toString());
        textEdit->setFileName(settings->value("lasteditedfile").toString());
        setWindowTitle(textEdit->getFileName() + STANDART_TITLE_EDITED);
    }
    connect(textEdit, &UnTextEdit::openSettingsEvent, this, [this]()
    {
       auto w = new AppearanceSettings(nullptr, textEdit->getTextType());
       w->show();
    });
    QSettings darkTheme(R"(HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Themes\Personalize)",QSettings::NativeFormat);

    this->lightTheme = !(darkTheme.value("AppsUseLightTheme") == 0);

    connect(socketClient, &SocketClient::newNoteReceived, this, [this]()
    {
        if(QDir(QDir::currentPath() + "/Sync").exists())
        {
            QFile file(QDir::currentPath() + "/Sync/" + socketClient->getFilePath());
            if(file.open(QFile::ReadWrite))
            {
                file.write(socketClient->getNoteText().toUtf8());
                qDebug() << "New note received";
                systemTrayIcon->showMessage("New note received", socketClient->getFilePath(), QIcon(":/images/favicon.png"), 15000);
            } else {
                systemTrayIcon->showMessage("Sync error", socketClient->getFilePath(), QIcon(":/images/favicon.png"), 15000);
                qDebug() << "Eroor while sync";
            }
        } else {
            QDir().mkdir(QDir::currentPath() + "/Sync");
            QFile file(QDir::currentPath() + "/Sync/" + socketClient->getFilePath());
            if(file.open(QFile::ReadWrite))
            {
                file.write(socketClient->getNoteText().toUtf8());
                qDebug() << "New note received";
            } else {
                qDebug() << "Eroor while sync";
            }
        }
    });

}
// TODO: Remove this
void MainWindow::TreeViewDoubleClick(const QModelIndex &index)
{
   QString path = model->filePath(index);
   QString fileType = path.split("/").last().split(".").first().split(".").first();
   if(fileType == "obj")
   {
       qDebug() << "FileType is .obj";
   } else {
       qDebug() << "GOG" << path.split("/").last();
       qDebug() << "GOG LAST" << path.split(".").first().split(".").first();
   }

   QFileInfo fi(path);
   fileType = fi.suffix();
   if(fileType == "obj")
   {
       qDebug() << "GOG IS TRUE";
        m3DPreview->changeSource(path);
   } else {
       qDebug() << "GOG IS" << fileType;
   }
   QFile file(model->filePath(index));

   if(fileType != "obj") {
       if(file.open(QIODevice::ReadOnly))
       {
           this->textEdit->setText(file.readAll());
           this->fileName = path.split("/").back();
           qDebug() << this->fileName << '\n';
           setWindowTitle(this->fileName + STANDART_TITLE);
           textEdit->setFileName(this->fileName);
       }
   }
}

void MainWindow::updateUnknown()
{
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "KerNotes",
                                    tr("Check updates automatically\n"),
                                    QMessageBox::No | QMessageBox::Yes,
                                    QMessageBox::Yes);
        if(resBtn == QMessageBox::Yes)
        {
            this->settings->setValue(AUTO_UPDATES, true);
        } else {
            this->settings->setValue(AUTO_UPDATES, false);
        }

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    view->setFixedWidth(this->width() * 1/4);

    qDebug() << "Window resized";

}

void MainWindow::closeEvent (QCloseEvent *event)
{
    if(textEdit->getIsTextChanged()) {
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, "KerNotes",
                                                                tr("Are you sure?\n"),
                                                            QMessageBox::No | QMessageBox::Save  | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn == QMessageBox::No)
    {
        event->ignore();
    } else if(resBtn == QMessageBox::Yes){
        event->accept();
    } else {
        textEdit->saveFile();
        event->accept();
    }
} else {
        event->accept();
    }
}

void MainWindow::connectAll()
{
    connect(textEdit, &UnTextEdit::textChanged, this, [this] ()
    {
        qDebug() << "Live preview:" << livePreview;
        qDebug() << "Text changed event";
        short textType = this->textEdit->getTextType();
        textEdit->setIsTextChanged(true);
        setWindowTitle(textEdit->getFileName() + STANDART_TITLE_EDITED);
        // TODO: Optimize this
        if(!this->shown) {
            if(previewTextEdit->toPlainText().length() > 500000)
            {
                QMessageBox::warning(nullptr, "Too large file", "Your file is too large, live preview will be disabled");
                this->shown = true;
            } else {
                switch (textType) {
                    case 1:
                        if(!livePreview)
                        {
                            this->previewTextEdit->setHtml(this->textEdit->toHtml());

                        }
                        break;
                    case 2:
                        if(!livePreview)
                        {
                            this->previewTextEdit->setMarkdown(this->textEdit->toMarkdown());
                        } else {
                        }
                        break;
                    default:
                        break;
                }
            }
        }

    });
}

QToolBar *MainWindow::createToolbar()
{
    toolBar = new QToolBar();

    toolBar->setMovable(false);

    auto *fileToolButton = new QToolButton();
    auto textWorkerButton = new QToolButton();

    auto *menu = new QMenu();

    auto *openFileAction = new QAction("Open file");
    connect(openFileAction, &QAction::triggered, this->textEdit, &UnTextEdit::openFile);

    auto *saveFileAction = new QAction("Save file");
    connect(saveFileAction, &QAction::triggered, this->textEdit, &UnTextEdit::saveFile);

    auto *openDirAction = new QAction("Open folder");
    connect(openDirAction, &QAction::triggered, this, [this]()
    {
       QString path = QFileDialog::getExistingDirectory(this, "Choose Directory",QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
       this->view->setRootIndex(this->model->index(path));

    });

    auto boldAction = new QAction("Make text bold");
    connect(boldAction, &QAction::triggered, textEdit, &UnTextEdit::placeBoldText);

    auto cursAction = new QAction("Make text cursive");
    connect(cursAction, &QAction::triggered, textEdit, &UnTextEdit::placeCursText);

    auto imageAction = new QAction("Insert image");
    connect(imageAction, &QAction::triggered, textEdit, &UnTextEdit::insertImageSnippet);
    auto *makeTextMenu = new QMenu();

    auto *changeTextLayoutAction = new QAction("Change text layout");
    connect(changeTextLayoutAction, &QAction::triggered, this, []() {

    });
    auto *fontTestActon = new QAction("Open Font");
    connect(fontTestActon, &QAction::triggered, this, []()
    {
//       auto font = QFontDialog::getFont(0, this->font());
    });

    auto syncAction = new QAction("Sync");
    connect(syncAction, &QAction::triggered, this, [this]()
    {
        QJsonObject data;
        data.insert("file", QJsonValue::fromVariant(textEdit->getFileName()));
        switch (textEdit->getTextType()) {
            case 1:
                data.insert("text", QJsonValue::fromVariant(textEdit->toHtml()));
                break;
            case 2:
                data.insert("text", QJsonValue::fromVariant(textEdit->toMarkdown()));
                break;
            default:
                data.insert("text", QJsonValue::fromVariant(textEdit->toPlainText()));
        }

        QJsonDocument doc(data);
        if(syncType == "Text")
        {
            socketClient->syncNotes(doc.toJson());
        } else {
            socketClient->syncNotes(QString(doc.toJson()));
        }


    });


    auto *settingsAction = new QAction("Settings");
    connect(settingsAction, &QAction::triggered, this, [this]() {
       w = new AppearanceSettings(nullptr, this->textEdit->getTextType(), lightTheme, syncType, "ws://localhost:12345");
        QTextDocument *docum = this->textEdit->document();
        connect(w, &AppearanceSettings::textTypeChanged, this, [this, docum](){
            if(w->getTextType() != this->textEdit->getTextType())
            {
                qDebug() << "New text type " <<w->getTextType();
                switch (w->getTextType())
                {
                    case 1:
                        disconnect(textEdit, &UnTextEdit::textChanged, nullptr, nullptr);
                        this->connectAll();
                        if(this->textEdit->getTextType() == 2)
                        {
                            delete highliter;
                        }
                        break;

                    case 2:
                        highliter = new MarkdownHighlighter(docum);
                        qDebug()<<highliter;
                        break;
                    default:
                        qDebug() << "new text type" << w->getTextType();
                        break;
                }
                this->textEdit->setTextType(w->getTextType());
            }
           this->textEdit->setTextType(w->getTextType());

        });

        connect(w, &AppearanceSettings::newSyncType, this, [this]()
        {
            syncType = w->getSyncType();
        });
        connect(w, &AppearanceSettings::newUrl, this, [this]()
        {
            socketClient->setUrl(w->getUrl());
        });

        connect(w, &AppearanceSettings::lightThemeEnabled, this, [this]{
            qDebug() << "LIGHT THEME ALERT";
            QApplication::setPalette(this->style()->standardPalette());
            this->lightTheme = true;
        });
        connect(w, &AppearanceSettings::darkThemeEnabled, this, [this]{
            QPalette darkPalette;
            QColor darkColor = QColor(45,45,45);
            QColor disabledColor = QColor(127, 127, 127);
            darkPalette.setColor(QPalette::Window, darkColor);
            darkPalette.setColor(QPalette::WindowText, Qt::white);
            darkPalette.setColor(QPalette::Base, QColor(18,18,18));
            darkPalette.setColor(QPalette::AlternateBase, darkColor);
            darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
            darkPalette.setColor(QPalette::ToolTipText, Qt::white);
            darkPalette.setColor(QPalette::Text, Qt::white);
            darkPalette.setColor(QPalette::Disabled, QPalette::Text, disabledColor);
            darkPalette.setColor(QPalette::Button, darkColor);
            darkPalette.setColor(QPalette::ButtonText, Qt::white);
            darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, disabledColor);
            darkPalette.setColor(QPalette::BrightText, Qt::red);
            darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
            QApplication::setPalette(darkPalette);
            this->lightTheme = false;
        });
        connect(w, &AppearanceSettings::newFontSelected, this, [this]()
        {
            this->setFont(w->getNewFont());
        });

        w->show();

    });

    menu->addAction(openFileAction);
    menu->addAction(saveFileAction);
    menu->addAction(openDirAction);

    makeTextMenu->addAction(boldAction);
    makeTextMenu->addAction(cursAction);
    makeTextMenu->addAction(imageAction);

    fileToolButton->setMenu(menu);
    fileToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fileToolButton->setText("File");
    connect(fileToolButton, &QToolButton::clicked, textEdit, &UnTextEdit::newFile);

    connect(textEdit, &UnTextEdit::clearTitle, this, [this]()
    {
        setWindowTitle("");
    });

    textWorkerButton->setMenu(makeTextMenu);
    textWorkerButton->setPopupMode(QToolButton::InstantPopup);
    textWorkerButton->setText("Text Worker");

    toolBar->addWidget(fileToolButton);
    toolBar->addWidget(textWorkerButton);
    toolBar->addAction(settingsAction);
//    toolBar->addAction(fontTestActon);

    return toolBar;
}

MainWindow::~MainWindow() = default;
