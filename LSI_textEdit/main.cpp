#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //QSplashScreen splash(QPixmap(":/splash.gif"));
    //splash.show();

    MainWindow mainWin;
    mainWin.setWindowIcon(QIcon(":/LSI"));

    //mainWin.showSplash(&splash);
    //splash.finish(&mainWin);

    mainWin.showMaximized();

    return app.exec();
}
