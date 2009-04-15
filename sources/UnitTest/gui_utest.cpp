#include "utest_impl.h"

/**
 * UnitTesting of GUI
 */
bool UTestGUI(int argc, char **argv)
{
    QApplication app(argc, argv);
    GraphW graph;
    for ( int i = 0; i < 10; i++)
    {
        for ( int j = 0; j < 10; j++)
        {
            NodeItem* node = graph.NewNode();
            node->setPos( i * 80, j * 80);
        }
    }
    graph.show();
    return app.exec();
}