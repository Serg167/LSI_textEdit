#pragma once
#include <QtWidgets>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QPlainTextEdit *textEdit;
    QString curFile;
    QUndoStack *undoStack;
    QLabel* amOfSymb;

public:
    MainWindow(QMainWindow* parent=0);
    void showSplash(QSplashScreen* );

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void newFile();
    void open();
    bool save();
    void amountOfSymb();

private:
    void loadFile(const QString &fileName);
    void createActions();
    void createStatusBar();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);

};


