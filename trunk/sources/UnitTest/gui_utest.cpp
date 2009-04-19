#include "utest_impl.h"

/**
 * UnitTesting of GUI
 */
bool uTestGUI(int argc, char **argv)
{
    QApplication app(argc, argv);
    GraphView graph;
    graph.readFromXML( "graph.xml");
#if GENERATE_NODES    
    for ( int i = 0; i < 10; i++)
    {
        for ( int j = 0; j < 10; j++)
        {
            NodeItem* node = graph.newNode();
            node->setPos( i * 80, j * 80);
        }
    }
#endif
 
    graph.show();
    return app.exec();
}