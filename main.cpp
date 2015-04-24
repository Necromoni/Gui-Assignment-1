#include "davidquantexteditor.h"
#include <QApplication>

//David Quan
//GUI PROGRAMMING SECTION 1 8AM - 9:30AM - MONDAY, WEDNESDAY

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DavidQuanTextEditor w;
    w.show();
    a.setApplicationName("David Quan Text Editor");
    a.setApplicationVersion("1.0");
    a.setOrganizationName("University of Belize CMPS2212: GUI Programming");
    a.setOrganizationDomain("online.ub.edu.bz");
    return a.exec();
}
