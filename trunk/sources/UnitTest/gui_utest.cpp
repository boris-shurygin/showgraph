#include "utest_impl.h"

/**
 * UnitTesting of GUI
 */
bool UTestGUI(int argc, char **argv)
{
    QApplication app(argc, argv);
    QGraphicsView view;
    QGraphicsScene scene( &view);
    
    view.setScene(&scene);
    view.setCacheMode( QGraphicsView::CacheBackground);
    view.setViewportUpdateMode( QGraphicsView::BoundingRectViewportUpdate);
    view.setRenderHint(QPainter::Antialiasing);
    view.setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    view.setResizeAnchor(QGraphicsView::AnchorViewCenter);
    view.setMinimumSize(400, 400);
    
    EdgeControl* c1 = new EdgeControl( NULL);
    c1->setPos( -30, -30);
    scene.addItem( c1);
    
    EdgeControl* c2 = new EdgeControl( NULL);
    c2->setPos( 30, 30);
    scene.addItem( c2);

    EdgeSegment* seg = new EdgeSegment( NULL, c1, c2);
    scene.addItem( seg);
   
    view.show();
    return app.exec();
}