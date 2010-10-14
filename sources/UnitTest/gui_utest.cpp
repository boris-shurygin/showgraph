#include "utest_impl.h"

/**
 * UnitTesting of GUI
 */
bool uTestGUI(int argc, char **argv)
{
    QApplication app(argc, argv);
    QMainWindow mainWin;
    //StyleEdit edit( &mainWin);
    //mainWin.setCentralWidget( &edit);
    mainWin.show();
    
    return app.exec();
}