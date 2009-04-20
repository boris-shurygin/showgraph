#include "utest_impl.h"

/**
 * UnitTesting of GUI
 */
bool uTestGUI(int argc, char **argv)
{
    QApplication app(argc, argv);
    MainWindow mainWin;
    mainWin.show();
    return app.exec();
}