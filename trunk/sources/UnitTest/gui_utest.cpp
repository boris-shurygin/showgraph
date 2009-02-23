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
    
    EdgeControl c1( NULL);
    c1.setPos( 0, 0);
    scene.addItem( &c1);
    
    EdgeControl c2( NULL);
    c2.setPos( 10, 10);
    scene.addItem( &c2);

    EdgeControl c3( NULL);
    c3.setPos( 20, 20);
    scene.addItem( &c3);
    
    EdgeSegment seg1( NULL, &c1, &c2);
    EdgeSegment seg2( NULL, &c2, &c3);
    scene.addItem( &seg1);
    scene.addItem( &seg2);
    view.show();
    return app.exec();
}