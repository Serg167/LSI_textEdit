#include "mainwindow.h"
#include "about.h"

MainWindow::MainWindow(QMainWindow *parent)
    : QMainWindow(parent)
{
    textEdit = new QPlainTextEdit;
    QFont LSI("Times", 12, QFont::Bold);
    textEdit->setFont(LSI);
    amOfSymb = new QLabel;

    undoStack = new QUndoStack(this);

    setCentralWidget(textEdit);

    createActions();
    createStatusBar();

    setCurrentFile(QString());

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::newFile()
{
    if (maybeSave()) {
        textEdit->clear();
        setCurrentFile(QString());
    }
}

void MainWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,tr("Open file"));
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}


bool MainWindow::save()
{
    if (curFile.isEmpty()) {
        QString fileName = QFileDialog::getSaveFileName(this,tr("Save file"),tr("Text files (*.txt)"));
        return saveFile(fileName);
    } else {
        return saveFile(curFile);
    }
}

void MainWindow::amountOfSymb()
{
    amOfSymb->setText(tr("Amount of symbols in the text: %1").arg(textEdit->toPlainText().size()));
}


void MainWindow::createActions()
{
    //create File menu bar
    QMenu *fileMenu = new QMenu(tr("&File"));
    menuBar()->addMenu(fileMenu);
    //create File toolbar
    QToolBar *fileToolBar = new QToolBar(tr("File menu"));
    fileToolBar->setToolTip(tr("File toolbar"));
    fileToolBar->setMovable(false);

    addToolBar(fileToolBar);    

    //create actions for "FILE" menu bar and toolbar

    //New File action
    QAction *newAct = new QAction(QIcon(":/new.png"), tr("&New"),this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("New file"));
    newAct->setToolTip(tr("New File"));
    connect(newAct, SIGNAL(triggered(bool)), this, SLOT(newFile()));
    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);

    //Open file action
    QAction *openAct = new QAction(QIcon(":/open.png"), tr("&Open..."),this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open file"));
    openAct->setToolTip(tr("Open file"));
    connect(openAct, SIGNAL(triggered(bool)), this, SLOT(open()));
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

    //Save file action
    QAction *saveAct = new QAction(QIcon(":/save.png"), tr("&Save"),this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save file"));
    saveAct->setToolTip(tr("Save file"));
    connect(saveAct, SIGNAL(triggered(bool)), this, SLOT(save()));
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);

    //add separator
    fileMenu->addSeparator();

    //Exit action only in File menu
    QAction *exitAct = new QAction(tr("E&xit"),this);
    exitAct->setStatusTip(tr("Exit"));
    exitAct->setShortcut(Qt::CTRL+Qt::Key_Q);
    connect(exitAct, SIGNAL(triggered(bool)),this,SLOT(close()));
    fileMenu->addAction(exitAct);


    //Create Edit menu bar
    QMenu *editMenu = new QMenu(tr("&Edit"));
    editMenu->setToolTip(tr("Edit"));
    menuBar()->addMenu(editMenu);

    //Create Edit toolbar
    QToolBar *editToolBar = new QToolBar(tr("Edit"));
    editToolBar->setToolTip(tr("Edit toolbar"));
    editToolBar->setMovable(false);
    addToolBar(editToolBar);


    //Undo action
    QAction *undoAct = undoStack->createUndoAction(this, tr("&Undo"));
    undoAct->setIcon(QIcon(":/undo.png"));
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo"));
    undoAct->setToolTip(tr("Undo"));
    connect(undoAct, SIGNAL(triggered(bool)), textEdit, SLOT(undo()));
    editMenu->addAction(undoAct);
    editToolBar->addAction(undoAct);
    connect(textEdit, SIGNAL(undoAvailable(bool)), undoAct, SLOT(setEnabled(bool)));


    //Redo action
    QAction *redoAct = undoStack->createRedoAction(this, tr("&Redo"));
    redoAct->setIcon(QIcon(":/redo.png"));
    redoAct->setShortcuts(QKeySequence::Redo);
    redoAct->setStatusTip(tr("Redo"));
    redoAct->setToolTip(tr("Redo"));
    connect(redoAct, SIGNAL(triggered(bool)), textEdit, SLOT(redo()));
    editMenu->addAction(redoAct);
    editToolBar->addAction(redoAct);
    connect(textEdit, SIGNAL(redoAvailable(bool)), redoAct, SLOT(setEnabled(bool)));


    //add separator
    editMenu->addSeparator();


    //Cut action
    QAction *cutAct = new QAction(QIcon(":/cut.png"), tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut(copy to clipboard)"));
    cutAct->setToolTip(tr("Cut(copy to clipboard)"));
    connect(cutAct, SIGNAL(triggered(bool)), textEdit, SLOT(cut()));
    editMenu->addAction(cutAct);
    editToolBar->addAction(cutAct);
    cutAct->setEnabled(false);
    connect(textEdit, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));


    //Copy to clipboard action
    QAction *copyAct = new QAction(QIcon(":/copy.png"), tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy to the clipboard"));
    copyAct->setToolTip(tr("Copy to the clipboard"));
    connect(copyAct, SIGNAL(triggered(bool)), textEdit, SLOT(copy()));
    editMenu->addAction(copyAct);
    editToolBar->addAction(copyAct);
    copyAct->setEnabled(false);
    connect(textEdit, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));

    //Paste from clipboard action
    QAction *pasteAct = new QAction(QIcon(":/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste from the clipboard"));
    pasteAct->setToolTip(tr("Paste from the clipboard"));
    connect(pasteAct, SIGNAL(triggered(bool)), textEdit, SLOT(paste()));
    editMenu->addAction(pasteAct);
    editToolBar->addAction(pasteAct);

    //"About" action
    menuBar()->addAction(tr("About"),new about,SLOT(show()));

}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
    statusBar()->setToolTip(tr("Status bar"));
    statusBar()->addPermanentWidget(amOfSymb);
    connect(textEdit, SIGNAL(textChanged()),this,SLOT(amountOfSymb()));
}

void MainWindow::showSplash(QSplashScreen* psplash)
{
    QTime time;
    time.start();
    for (int i = 0; i < 100; ) {
        if (time.elapsed() > 30) {
            time.start();
            ++i;
        }
        psplash->showMessage("Loading LSI_TextEditor: "
                             + QString::number(i) + "%",
                             Qt::AlignHCenter | Qt::AlignBottom,
                             Qt::black
                             );
        qApp->processEvents();
    }
}




bool MainWindow::maybeSave()
{
    if (!textEdit->document()->isModified())
        return true;
    QMessageBox* pmBox = new QMessageBox(QMessageBox::Warning, tr("Save?"), tr("Save changes?"),
                                         QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    int n = pmBox->exec();
    if (n == QMessageBox::Yes)
        return save();
    if (n == QMessageBox::Cancel)
        return false;
    return true;
}


void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Load file error"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName),
                                  file.errorString()));
        return;
    }
    QTextStream in(&file);
    textEdit->setPlainText(in.readAll());
    setCurrentFile(fileName);
}


bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Save file error"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName),
                                  file.errorString()));
        return false;
    }

    QTextStream out(&file);
    out << textEdit->toPlainText();
    setCurrentFile(fileName);
    return true;
}


void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "LSI_untitled.txt";
    setWindowFilePath(shownName);
}


