#ifndef DAVIDQUANTEXTEDITOR_H
#define DAVIDQUANTEXTEDITOR_H

//David Quan
//GUI PROGRAMMING SECTION 1 8AM - 9:30AM - MONDAY, WEDNESDAY

#include <QMainWindow>
#include <stack>
#include <QTextEdit>
#include <string>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFontDialog>
#include <QTextStream>
#include <QFileInfo>
#include <QFileDialog>
#include <QTranslator>
#include <QActionGroup>
#include <QPrinter>
#include <QPrintDialog>


namespace Ui {
class DavidQuanTextEditor;
}

class DavidQuanTextEditor : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit DavidQuanTextEditor(QWidget *parent = 0);
    ~DavidQuanTextEditor();

private:
    bool reconsiderClose(); //prompts the user if he wishes to save the file before exiting
    void removeTextEditor(); //removes the text editor at the top of the stack
    void addTextEditor(const QString &fileName); //add a text editor to the top of the stack with a name
    void addTextEditor(); //add a text editor to the top of the stack without a name
    void reconnectActionsToEditor(); //connects all the actions to the top text editor
    void loadFile(const QString &fileName); //load the contents of a file name to the top text editor
    void setFileName(const QString &fileName); //sets the file name of the top text editor also renames window
    void setupLanguageActionGroup();
private slots:
    void on_actionNew_triggered();

    bool on_actionClose_triggered();

    void on_actionExit_triggered();

    void documentModified();

    void on_actionSelectFont_triggered();

    void on_actionAbout_triggered();

    void on_actionOpen_triggered();

    bool saveFile();

    bool saveFileAs();

    void on_actionPrint_triggered();

    void switchLanguage(QAction *);

    void on_actionEnglish_triggered();

    void on_actionSpanish_triggered();

    void on_actionSwedish_triggered();

private:
    Ui::DavidQuanTextEditor *ui;
    std::stack<QTextEdit*> textEditors; //holds all the text editors
    std::stack<QString> names; //holds all the text editors' names
    int TEXT_EDITOR_HEIGHT;
    int TEXT_EDITOR_WIDTH;
    int TEXT_EDITOR_X_POS;
    int TEXT_EDITOR_Y_POS;

    QTranslator appTranslator;
    QTranslator qtTranslator;

    QMenu *languageMenu;

    QActionGroup languageActionGroup;

protected:
    void closeEvent(QCloseEvent *);

};

#endif // DAVIDQUANTEXTEDITOR_H
