#include "davidquantexteditor.h"
#include "ui_davidquantexteditor.h"

//David Quan
//GUI PROGRAMMING SECTION 1 8AM - 9:30AM - MONDAY, WEDNESDAY

//DEFAULT CONSTRUCTOR
DavidQuanTextEditor::DavidQuanTextEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DavidQuanTextEditor),
    TEXT_EDITOR_HEIGHT(431),
    TEXT_EDITOR_WIDTH(331),
    TEXT_EDITOR_X_POS(10),
    TEXT_EDITOR_Y_POS(60),
    languageActionGroup(this)
{
    ui->setupUi(this);

    addTextEditor(); //adds the default text editor
    qApp->installTranslator(&appTranslator);
    qApp->installTranslator(&qtTranslator);
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(saveFileAs()));

    QString locale = "en";
    ui->actionEnglish->setData(locale);
    locale = "es";
    ui->actionSpanish->setData(locale);
    locale = "sv";
    ui->actionSwedish->setData(locale);
    ui->textEdit->close(); //close the text editor created by the UI for a workaround on the layout

    //setupLanguageActionGroup();
}

//DESTRUCTOR
DavidQuanTextEditor::~DavidQuanTextEditor()
{
    delete ui;
}

//ACTION SECTION//

//NEW ACTION
void DavidQuanTextEditor::on_actionNew_triggered()
{
    addTextEditor(); //calls helper function addTextEditor with no name
}

//CLOSE ACTION
bool DavidQuanTextEditor::on_actionClose_triggered()
{
    if (!textEditors.empty()) //if there is a text editor
    {
        if (textEditors.top()->isWindowModified()) //that text editor has modified data?
        {
            if(reconsiderClose()) //prompt user
                removeTextEditor(); //accepted
            else
                return false; //if he declined, return false
        }
        else //this text editor doesnt have modified data
            removeTextEditor();
    }
    if (textEditors.empty())
    {
        this->close(); //if there are no text editors,exit the application
    }

    return true; //success
}

//EXIT ACTION
void DavidQuanTextEditor::on_actionExit_triggered()
{
    while(!textEditors.empty() && on_actionClose_triggered()); //keep looping for all text editors
    return;
}

//FONT ACTION
void DavidQuanTextEditor::on_actionSelectFont_triggered()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, textEditors.top()->font(), this );
    if (ok)
    {
        ui->statusBar->showMessage(tr("Changed Font"), 2000);
        textEditors.top()->setFont(font);
    }
}

//ABOUT ACTION
void DavidQuanTextEditor::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About"), tr("This program was made in Qt for an Assignment assigned by Mr. Medina"));
}

//OPEN ACTION
void DavidQuanTextEditor::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Document"), QDir::currentPath(), tr("Text documents (*.txt)"));
    if (fileName.isNull())
    {
        return;
    }
    if (names.top().isNull() && !(textEditors.top()->isWindowModified()))
    {
        loadFile(fileName);
    }
    else
    {
        addTextEditor(fileName);
    }
}

//PRINT ACTION; DOES NOT WORK WITH QT5
void DavidQuanTextEditor::on_actionPrint_triggered()
{
    //ui->statusBar->showMessage(tr("QT 5 does not support printing. Please see .cpp file"), 2000);
    //*****RESOLVED QT5 PRINT SUPPORT BY USING QtPrintSupport/QPrinter*****
    const QTextDocument *document = textEditors.top()->document();
    QPrinter printer;
    QPrintDialog printerDialog(&printer,this);
    if (printerDialog.exec() != QDialog::Accepted)
    {
        ui->statusBar->showMessage(tr("Failed to Print"), 2000);
        return;
    }
    document->print(&printer);
    ui->statusBar->showMessage(tr("Document Printed"), 2000);

}
//END OF ACTION SECTION//

void DavidQuanTextEditor::switchLanguage(QAction * action)
{
    QString locale = action->data().toString();
    if (!appTranslator.load("/translations/translations/davidquantexteditor_" + locale))
    {
        ui->statusBar->showMessage ("Failed to Load appTranslator", 5000);
    }
    if (qtTranslator.load(":/translations/translations/davidquantexteditor_" + locale))
    {
        ui->statusBar->showMessage ("Failed to Load qtTranslator", 5000);
    }

    ui->retranslateUi(this);

    ui->statusBar->showMessage(tr("Language changed to: ") + action->text(),2000);
}

//CREATELANGUAGEACTIONGROUP

void DavidQuanTextEditor::setupLanguageActionGroup()
{
    languageMenu = new QMenu(this);
    languageActionGroup.isExclusive();

    QString qmPath = (qApp->applicationDirPath() + "/translations");

    connect(&languageActionGroup, SIGNAL(triggered(QAction *)), this, SLOT(switchLanguage(QAction *)));

    QDir directory(qmPath);
    QStringList fileNames = directory.entryList(QStringList("davidquantexteditor_*.qm"));

    for (int i = 0; i < fileNames.size(); i++)
    {
        QString locale = fileNames[i]; //get file names iteratively
        locale.remove(0, locale.indexOf('_') + 1); //get the locale out of the file name
        locale.truncate(locale.lastIndexOf('.'));

        QTranslator translator;
        translator.load(fileNames[i], qmPath);
        QString language = translator.translate("MainWindow", "English");

        QAction *action = new QAction(tr("%1 + %2").arg(i + 1).arg(language), this);
        QIcon icon(":/icons" + locale + ".png"); //gets the icon for that locale
        action->setText(locale);
        action->setIcon(icon); //sets the icon for the action

        action->setCheckable(true);
        action->setData(locale);

        languageMenu->addAction(action);
        languageActionGroup.addAction(action);

        if (language == "English")
        {
            action->setChecked(true); //set english to default
            action->setVisible(true);
        }
    }
    ui->menuBar->addMenu(languageMenu);     //add the menu to the menubar
    languageMenu->setTitle(tr("&Language")); //set the title
    languageMenu->addActions(languageActionGroup.actions()); //add all the actions via action list
    languageMenu->show();
}

//SWITCHLANGUAGE SLOT

//CLOSE EVENT
void DavidQuanTextEditor::closeEvent(QCloseEvent *e)
{
    if (textEditors.empty())
    {
        e->accept();
    }
    else
    {
        e->ignore();
        while(!textEditors.empty() && on_actionClose_triggered());
        e->accept();
    }
    //****I used a different implementation; left these in comments to show that I understand the concept***

    //handle close for text editor
    //if (!this->isWindowModified()) //this window isnt modified
    //{
    //    e->accept();
    //}
    //else //this window is modified
    //{
    //    if(reconsiderClose())
    //        e->accept(); //if he said yes, accept
    //   else
    //        e->ignore(); //user declined to close
    //}
    return;
}

//DOCUMENT MODIFIED SLOT
void DavidQuanTextEditor::documentModified()
{
    if (!textEditors.empty())
    {
        (textEditors.top())->setWindowModified(true); //the top text editor had to be the modified one
    }

    if (!isWindowModified()) //we didnt mess with the main window
    {
        setWindowTitle(windowTitle() + "[*]");
        setWindowModified(true);
    }
}

//HELPER FUNCTION SECTION//

//PROMPTS THE USER IF HE WANTS TO SAVE OR NAH
bool DavidQuanTextEditor::reconsiderClose()
{
    switch(QMessageBox::warning(this, tr("Unsaved Changes"),
                tr("Your document has been modified. Do you want save your changes?\n You will lose all unsaved changes."),
                QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel, QMessageBox::Cancel))
    {
    case QMessageBox::Yes:
        return saveFile();
    case QMessageBox::No:
        return true;
    case QMessageBox::Cancel:
        return false;
    default:
        return false;
    }
}

//REMOVES THE TEXT EDITOR AT THE TOP OF THE STACK
void DavidQuanTextEditor::removeTextEditor()
{
    textEditors.top()->hide(); //hide the top text editor
    ui->gridLayout_2->removeWidget(textEditors.top());
    textEditors.top()->close();//close the top text editor
    textEditors.pop(); //pop it off the stack
    names.pop(); //pop old name off stack
    if (!textEditors.empty()) //check if theres no more text editors
    {
        ui->statusBar->showMessage(tr("File Closed"), 2000);
        textEditors.top()->show(); //if there is one show it
        ui->gridLayout_2->addWidget(textEditors.top()); //add it to the layout
        reconnectActionsToEditor();
        setFileName(names.top());
    }
}

//ADD A TEXT EDITOR TO THE TOP OF THE STACK WITH A NAME
void DavidQuanTextEditor::addTextEditor(const QString &fileName = QString()) //add a text editor with a name
{
    QTextEdit * textEditor = new QTextEdit(ui->textWidget); //create a new text editor, set the parent to the default one
    ui->gridLayout_2->addWidget(textEditor);                //add the widget to the layout for the textWidget
    if (!textEditors.empty())
    {
        textEditor->hide();
        ui->gridLayout_2->removeWidget(textEditors.top());
    } //hide the last text editor if there is one
    textEditors.push(textEditor);                           //put this text editor on the stack
    textEditor->show();                                     //show this text editor
    textEditor->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    reconnectActionsToEditor();
    names.push(fileName);

    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);

    ui->statusBar->showMessage(tr("New File Opened"), 2000);
    if(names.top().isNull())
    {
        loadFile(names.top());
    }
    else
    {
        loadFile(names.top());
    }
    textEditors.top()->setFocus();
}

//ADD A TEXT EDITOR TO THE TOP OF THE STACK WITHOUT A NAME
void DavidQuanTextEditor::addTextEditor() //add a text editor without a name
{
    QTextEdit * textEditor = new QTextEdit(ui->textWidget); //create a new text editor, set the parent to the textWidget one
    ui->gridLayout_2->addWidget(textEditor);                //add the widget to the layout for the textWidget
    if (!textEditors.empty())
    {
        textEditor->hide();
        ui->gridLayout_2->removeWidget(textEditors.top());
    }       //hide the last text editor if there is one
    textEditors.push(textEditor);                           //put this text editor on the stack
    textEditor->show();                                     //show this text editor
    reconnectActionsToEditor();
    textEditor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    names.push(QString()); //push a null name on stack

    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);

    ui->statusBar->showMessage(tr("New File Opened"), 2000);
    setFileName(names.top());
    textEditors.top()->setFocus();

}

//RECONNECT ALL THE ACTIONS TO THE TOP TEXT EDITOR
void DavidQuanTextEditor::reconnectActionsToEditor()
{
    connect(ui->actionCut, SIGNAL(triggered()), textEditors.top(), SLOT(cut()));
    connect(ui->actionCopy, SIGNAL(triggered()), textEditors.top(), SLOT(copy()));
    connect(ui->actionPaste, SIGNAL(triggered()), textEditors.top(), SLOT(paste()));
    connect(ui->actionUndo, SIGNAL(triggered()), textEditors.top(), SLOT(undo()));
    connect(ui->actionRedo, SIGNAL(triggered()), textEditors.top(), SLOT(redo()));


    connect(textEditors.top(), SIGNAL(copyAvailable(bool)), ui->actionCopy, SLOT(setEnabled(bool)));
    connect(textEditors.top(),SIGNAL(copyAvailable(bool)), ui->actionCut, SLOT(setEnabled(bool)));
    connect(textEditors.top(),SIGNAL(undoAvailable(bool)), ui->actionUndo, SLOT(setEnabled(bool)));
    connect(textEditors.top(),SIGNAL(redoAvailable(bool)), ui->actionRedo, SLOT(setEnabled(bool)));

    connect(textEditors.top(),SIGNAL(textChanged()),this,SLOT(documentModified()));

    return;
}

//LOAD A FILE'S CONTENTS INTO THE TOP TEXT EDITOR'S CONTENTS
void DavidQuanTextEditor::loadFile(const QString &fileName) //calls setFileName
{
    if (fileName.isEmpty())
    {
        setFileName(QString());
    }
    if (fileName.isNull())
    {
        setFileName(QString());
        return;
    }

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QMessageBox::warning(this, tr("Cannot Open File"), tr("Unable to open file ") + fileName + ".",QMessageBox::Ok);
        setFileName(QString());
        return;
    }
    int fd =file.handle();
    FILE* f = fdopen(fd, "rb");
    QTextStream textStream(f);

    textEditors.top()->setText(textStream.readAll()); //set the body text of the current text editor
    ui->statusBar->showMessage(tr("Document Opened"), 2000);
    file.close();
    names.top() = fileName; //set this file's name
    setFileName(names.top());
    //setWindowModified(false);
    textEditors.top()->setWindowModified(false);
}

//SETS THE APPLICATION NAME AND SETS THE NAME AT THE TOP OF THE NAME STACK TO NAME GIVEN
void DavidQuanTextEditor::setFileName(const QString &fileName)
{
    names.top() = fileName;
    QString applicationName = (QApplication::applicationName());
    if (names.top().isNull())
        setWindowTitle("untitled - " + applicationName);
    else
        setWindowTitle(QFileInfo(names.top()).fileName() + " - " + applicationName);
    setWindowModified(false);
}

//COPIES THE TEXT EDITOR AT THE TOP OF THE STACK'S CONTENTS INTO THE FILE SUPPLIED BY THE USER (CALLS SAVE AS IF NO FILE)
bool DavidQuanTextEditor::saveFile() //we check if name is null and set name to null to prevent infinite loop
{
    if (names.top().isNull())
    {
        return saveFileAs();
    }
    else if (!(names.top().isNull()))
    {
        QFile file(names.top());
        if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            QMessageBox::warning(this, tr("Cannot Open File"), tr("Unable to open file") + names.top() + ".",QMessageBox::Ok);
            setFileName(QString());
            ui->statusBar->showMessage(tr("Failed to save Document"), 2000);
            return false;
        }
        QTextStream textStream(&file);

        textStream << textEditors.top()->toPlainText();

        qDebug("Saving");
        file.close();
        textEditors.top()->setWindowModified(false);
        setWindowModified(false);
        setFileName(names.top()); //reset window name indirectly
        ui->statusBar->showMessage(tr("Document Saved"), 2000 );
        return true;
    }
    return false;
}

//DOES THE SAME AS SAVE FILE EXCEPT THE FILE IS CREATED IF IT DOESNT EXIST (CALLS SAVE AS TO PERFORM SAVING)
bool DavidQuanTextEditor::saveFileAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save document",
                                                    names.top().isNull()?QDir::currentPath():names.top(),"Text documents (*.txt)");
    if (fileName.isNull())
    {
        ui->statusBar->showMessage(tr("Failed to save Document"), 2000);
        return false;

    }
    else
    {
        setFileName(fileName);
        if (saveFile())
        {
            ui->statusBar->showMessage(tr("Document saved"), 2000);
            return true;
        }

    }
    return false;
}

//END OF HELPER FUNCTION SECTION//

//LANGUAGE ACTIONS
void DavidQuanTextEditor::on_actionEnglish_triggered()
{
    switchLanguage(ui->actionEnglish);
}

void DavidQuanTextEditor::on_actionSpanish_triggered()
{
    switchLanguage(ui->actionSpanish);
}

void DavidQuanTextEditor::on_actionSwedish_triggered()
{
    switchLanguage(ui->actionSwedish);
}
