/**
 * File: node_w.cpp - Node Widget implementation.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gui_impl.h"

/** We can't create nodes separately, do it through newNode method of graph */
GNode::GNode( GraphView *graph_p, int _id):
    AuxNode( ( AuxGraph *)graph_p, _id)
{
    item_p = new NodeItem( this);
}

/** Contructor of node with specified position */
GNode::GNode( GraphView *graph_p, int _id, QPointF _pos):
    AuxNode( ( AuxGraph *)graph_p, _id)
{
    item_p = new NodeItem( this);
    item_p->setPos( _pos);
}

GNode::~GNode()
{
    if ( isEdgeControl() 
         && isNotNullP( firstPred()) 
         && isNotNullP( firstSucc())
         && isNotNullP( firstPred()->pred())
         && isNotNullP( firstSucc()->succ()))
    {
        graph()->newEdge( firstPred()->pred(), firstSucc()->succ());
    } else if ( isSimple())
    {
        QList< GNode *> nodes;
        GEdge* edge;
        for ( edge = firstSucc(); isNotNullP( edge); edge = edge->nextSucc())
        {
            edge->item()->adjust();
            GNode* succ = edge->succ();

            while ( succ->isEdgeControl())
            {
                assert( isNotNullP( succ->firstSucc()));
                nodes << succ;
                succ = succ->firstSucc()->succ();
            }
        }
        for ( edge = firstPred(); isNotNullP( edge); edge = edge->nextPred())
        {
            edge->item()->adjust();
            GNode* pred = edge->pred();

            while ( pred->isEdgeControl())
            {
                assert( isNotNullP( pred->firstPred()));
                nodes << pred;
                pred = pred->firstPred()->pred();
            }
        }
        
        foreach ( GNode *n, nodes)
        {
            delete n;
        }
    }
    item()->remove();
}

GraphView * GNode::graph() const
{
    return static_cast< GraphView *>( AuxNode::graph());
}

/**
 * Update DOM tree element
 */
void
GNode::updateElement()
{
    AuxNode::updateElement();// Base class method call
    QDomElement e = elem();
    e.setAttribute( "x", item()->x());
    e.setAttribute( "y", item()->y());
    e.setAttribute( "label", item()->toPlainText());
    if ( isEdgeControl())
    {
        e.setAttribute( "edge_control", 1);
    }
}

/**
 * read properties from DOM tree element
 */
void
GNode::readFromElement( QDomElement e)
{
    assert( !e.isNull());
    assert( e.tagName() == QString( "node"));
    
    if ( e.hasAttribute( "x") && e.hasAttribute( "y"))
    {
        item()->setPos( e.attribute( "x").toDouble(),
                        e.attribute( "y").toDouble());
    }
    if ( e.hasAttribute( "label"))
    {
        item()->setPlainText( e.attribute( "label"));
    }
    if ( e.hasAttribute("edge_control"))
    {
        setTypeEdgeControl();
    }
    AuxNode::readFromElement( e); // Base class method
}

/**
 * NodeItem implementation
 */
/** Initialization */
void 
NodeItem::SetInitFlags()
{
    QString text = QString("Node %1").arg( node()->id());
    setPlainText( text);
    setFlag( ItemIsMovable);
    //setFlag(ItemIsSelectable);
    //setCacheMode( DeviceCoordinateCache);
    setZValue(2);
}

QRectF 
NodeItem::boundingRect() const
{
    if ( isNullP( node_p))
        return QRectF();

    if ( node()->isEdgeControl())
    {
        qreal adjust = 2;
        return QRectF( -EdgeControlSize - adjust, -EdgeControlSize - adjust,
              2*( EdgeControlSize + adjust), 2*( EdgeControlSize + adjust));
    } else
    {
        qreal adjust = 5;
        return QGraphicsTextItem::boundingRect()
            .adjusted( -adjust, -adjust, adjust, adjust);
    }
}

QPainterPath 
NodeItem::shape() const
{
    if ( isNullP( node_p))
        return QPainterPath();

    if ( node()->isEdgeControl())
    {
        QPainterPath path;
        path.addEllipse( -EdgeControlSize, -EdgeControlSize, 2*EdgeControlSize, 2*EdgeControlSize);
        return path; 
    } else
    {
        QPainterPath path;
        path.addRect( boundingRect());
        return path;
    }
}

void 
NodeItem::paint( QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget)
{
    if ( isNullP( node_p))
        return;

    if ( node()->isSimple())
    {
        qreal adjust = 3;
        if (option->state & QStyle::State_Sunken)
        {
            painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        } else
        {
            painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        }
        painter->drawRect( boundingRect());
        QGraphicsTextItem::paint( painter, option, widget);
    } else if ( node()->isEdgeControl())
    {
        if ( node()->firstPred()->item()->isSelected()
             || node()->firstSucc()->item()->isSelected())
        {
            if (option->state & QStyle::State_Sunken) 
            {
                painter->setBrush( Qt::gray);
                painter->setPen( QPen( Qt::black, 0));
            } else
            {
                painter->setBrush( Qt::lightGray);
                painter->setPen( QPen(Qt::darkGray, 0));
            }
            painter->drawEllipse(-EdgeControlSize, -EdgeControlSize,
                                  2*EdgeControlSize, 2*EdgeControlSize);
        }
    }
}

void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if( event->button() & Qt::RightButton && !node()->isEdgeControl())
    {
        node()->graph()->SetCreateEdge( true);
        node()->graph()->SetTmpSrc( node());
    }
    update();
    QGraphicsItem::mousePressEvent(event);
}

void NodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void NodeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if ( event->button() & Qt::LeftButton && !node()->isEdgeControl())
    {
        if ( textInteractionFlags() == Qt::NoTextInteraction)
            setTextInteractionFlags(Qt::TextEditorInteraction);
        QGraphicsTextItem::mouseDoubleClickEvent(event);
    }
}

void NodeItem::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    //emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}
void  NodeItem::keyPressEvent(QKeyEvent *event)
{
    QGraphicsTextItem::keyPressEvent(event);
    GEdge *edge = NULL;

    for ( edge = node()->firstSucc(); isNotNullP( edge); edge = edge->nextSucc())
    {
        edge->item()->adjust();
    }
    for ( edge = node()->firstPred(); isNotNullP( edge); edge = edge->nextPred())
    {
        edge->item()->adjust();
    }
}

QVariant NodeItem::itemChange( GraphicsItemChange change, const QVariant &value)
{
    GEdge *edge = NULL;

    if ( change != QGraphicsItem::ItemSceneChange 
         || change != QGraphicsItem::ItemSceneHasChanged)
    {
        for ( edge = node()->firstSucc(); isNotNullP( edge); edge = edge->nextSucc())
        {
            edge->item()->adjust();
            GNode* succ = edge->succ();

            if ( succ->isEdgeControl())
            {
                assert( isNotNullP( succ->firstSucc()));
                succ->firstSucc()->item()->adjust();
            }
        }
        for ( edge = node()->firstPred(); isNotNullP( edge); edge = edge->nextPred())
        {
            edge->item()->adjust();
            GNode* pred = edge->pred();

            if ( pred->isEdgeControl())
            {
                assert( isNotNullP( pred->firstPred()));
                pred->firstPred()->item()->adjust();
            }
        }
    }
    return QGraphicsTextItem::itemChange(change, value);
}