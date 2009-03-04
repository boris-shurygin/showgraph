#include "utest_impl.h"

/**
 * UnitTesting of GUI
 */
bool UTestGUI(int argc, char **argv)
{
    QApplication app(argc, argv);
    GraphW graph;
    graph.show();
    return app.exec();
}