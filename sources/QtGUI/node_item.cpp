/**
 * @file: node_item.cpp 
 * Drawable node implementation
 */
/* 
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gui_impl.h"

/** We can't create nodes separately, do it through newNode method of graph */
GNode::GNode( GGraph *graph_p, int _id):
    AuxNode( ( AuxGraph *)graph_p, _id),
	_doc( NULL),
	ir_id( -1),
	text_shown( false)
{
    item_p = new NodeItem( this);
	setIRId( id());
    graph()->invalidateRanking();
}

/** Contructor of node with specified position */
GNode::GNode( GGraph *graph_p, int _id, QPointF _pos):
    AuxNode( ( AuxGraph *)graph_p, _id),
	_doc( NULL),
	ir_id( -1),
	text_shown( false)
{
    item_p = new NodeItem( this);
    item_p->setPos( _pos);
	setIRId( id());
    graph()->invalidateRanking();
}

/**
 * Destructor for node - removes edge controls on incidient edges and disconnects item from scene
 */
GNode::~GNode()
{
    graph()->invalidateRanking();
    if ( ( isEdgeControl() || isEdgeLabel())
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
		Marker m = graph()->newMarker();
        for ( edge = firstSucc(); isNotNullP( edge); edge = edge->nextSucc())
        {
            edge->item()->adjust();
            GNode* succ = edge->succ();

            while ( succ->isEdgeControl() || succ->isEdgeLabel())
            {
                assert( isNotNullP( succ->firstSucc()));
                if ( succ->mark( m))
				{
					nodes << succ;
				}
                succ = succ->firstSucc()->succ();
            }
        }
        for ( edge = firstPred(); isNotNullP( edge); edge = edge->nextPred())
        {
            if ( edge->isSelf()) // We've already processed this one in previous loop
				continue;

			edge->item()->adjust();
            GNode* pred = edge->pred();

            while ( pred->isEdgeControl() || pred->isEdgeLabel())
            {
                assert( isNotNullP( pred->firstPred()));
                if ( pred->mark( m))
				{
					nodes << pred;
				}
                pred = pred->firstPred()->pred();
            }
        }
        
        foreach ( GNode *n, nodes)
        {
            delete n;
        }
		graph()->freeMarker( m);
    }
    if ( isNodeInFocus())
        graph()->setNodeInFocus( NULL);
    graph()->view()->viewHistory()->eraseNode( this);
    item()->remove();
    graph()->view()->deleteLaterNodeItem( item());
}
/**
 * Get the pointer to graph
 */
GGraph* GNode::graph() const
{
    return static_cast< GGraph *>( AuxNode::graph());
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
    if ( isSimple())
    {  
       // e.setAttribute( "type", "simple");
    } else if ( isEdgeControl())
    {
        e.setAttribute( "type", "edge_control");
    } else if ( isEdgeLabel())
    {
        e.setAttribute( "type", "edge_label");
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
        QString str = e.attribute( "label");
        item()->setPlainText( str);
        QRegExp rx("(\\d+)");
        if ( rx.indexIn( str) != -1)
        {
            setIRId( rx.cap( 1).toInt());
        }
    }
    if ( e.hasAttribute("type"))
    {
        QString type_str = e.attribute( "type");
        if ( type_str == QString( "edge_control"))
        {
            setTypeEdgeControl();
        } else if (type_str == QString( "edge_label"))
        {
            setTypeEdgeLabel();
        } 
    }

    AuxNode::readFromElement( e); // Base class method
}
bool GNode::isNodeInFocus() const
{
    return areEqP( this, graph()->nodeInFocus());
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
    setFlag( ItemIsFocusable, false);
    setCacheMode( DeviceCoordinateCache);
    setZValue(2);
    QGraphicsItem::setCursor( Qt::ArrowCursor);
}

/**
 * Rectangle that marks border of node
 */
QRectF 
NodeItem::borderRect() const
{
    if ( isNullP( node_p))
        return QRectF();

    if ( node()->isEdgeControl())
    {
        qreal adjust = 2;
        return QRectF( -EdgeControlSize - adjust, -EdgeControlSize - adjust,
              2*( EdgeControlSize + adjust), 2*( EdgeControlSize + adjust));
    } else if ( node()->isEdgeLabel())
    {
        qreal adjust = 5;
        return QGraphicsTextItem::boundingRect()
            .adjusted( -adjust, -adjust, adjust, adjust);    
    } else
    {
        qreal adjust = 5;
        return QGraphicsTextItem::boundingRect()
            .adjusted( -adjust, -adjust, adjust, adjust);
    }
}

/**
 * Overload of QGraphicsItem::bounding rectangle
 */
QRectF 
NodeItem::boundingRect() const
{
    qreal adjust = 2;
    return borderRect().adjusted( -adjust, -adjust, adjust, adjust);
}

/**
 * Shape of NodeItem: circle for EdgeControl and rectangle for simple
 */
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
        path.addRect( borderRect());
        return path;
    }
}

/**
 * Painting procedure for NodeItem
 */
void 
NodeItem::paint( QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget *widget)
{
    if ( isNullP( node_p))
        return;
    
    if ( node()->graph()->view()->isContext())
        painter->setOpacity( this->opacity);

    if ( node()->isSimple() || node()->isEdgeLabel())
    {
        if ( option->levelOfDetail < 0.1)
        {
            painter->fillRect( borderRect(), option->palette.highlight().color());
        }
        qreal adjust = 3;
        if ( bold_border )// ( option->state & QStyle::State_Sunken))
        {
            painter->setPen( QPen(option->palette.foreground().color(), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        } else
        {
            painter->setPen( QPen(option->palette.foreground().color(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        }
        if ( alternate_background)
        {
            painter->setBrush( option->palette.highlight().color());
        }
        if ( node()->isSimple())
            painter->drawRect( borderRect());
        if ( option->levelOfDetail >= 0.2)
            QGraphicsTextItem::paint( painter, option, widget);
    } else if ( node()->isEdgeControl())
    {
        if ( option->levelOfDetail < 0.2)
            return;
        if ( node()->firstPred()->item()->isSelected()
             || node()->firstSucc()->item()->isSelected())
        {
            if ( bold_border && ( option->state & QStyle::State_Sunken)) 
            {
                painter->setBrush( option->palette.highlight().color());
                painter->setPen( QPen(option->palette.foreground().color(), 0));
            } else
            {
                painter->setBrush( option->palette.highlight().color());
                painter->setPen( QPen(option->palette.foreground().color(), 0));
            }
            painter->drawEllipse( -EdgeControlSize, -EdgeControlSize,
                                  2*EdgeControlSize, 2*EdgeControlSize);
        }
    }
    painter->setOpacity( 1);
}

/**
 * Right button press starts edge drawing process 
 */
void NodeItem::mousePressEvent( QGraphicsSceneMouseEvent *event)
{
    bold_border = true;
	if( event->button() & Qt::RightButton && !node()->isEdgeControl())
    {
        node()->graph()->view()->SetCreateEdge( true);
        node()->graph()->view()->SetTmpSrc( node());
    } else if ( node()->isEdgeControl() || node()->isEdgeLabel())
    {
        node()->firstPred()->item()->setSelected( true);
        node()->firstSucc()->item()->setSelected( true);
    }
    QGraphicsTextItem::mousePressEvent(event);
	update();
}

/**
 * On mouse release we do nothing - graph will handle it for us
 */
void NodeItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event)
{
    bold_border = false;
	/** Select this node */
	node()->graph()->emptySelection();
    if ( !node()->isNodeInFocus())
    {
        node()->graph()->view()->viewHistory()->focusEvent( node());
        node()->graph()->setNodeInFocus( node());
    }
	node()->graph()->selectNode( this->node());
	/** Show context menu */
	if ( node()->graph()->view()->isShowContextMenus()
        && ( event->button() & Qt::RightButton) )
    {
	    QMenu *menu = node()->graph()->view()->createMenuForNode( node());
        menu->exec( event->screenPos());
        delete menu;
    } else if ( event->button() & Qt::LeftButton 
                && !( node()->isEdgeControl() || node()->isEdgeLabel()))
    {
        if (  node()->graph()->view()->isContext())
        {
            node()->graph()->view()->findContext();
        } else
        {
            node()->graph()->view()->showNodeText( node());
        }
	}
	QGraphicsTextItem::mouseReleaseEvent( event);
	update();
}

/**
 * Double click enables text edition for simple nodes
 */
void NodeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if ( event->button() & Qt::LeftButton && !node()->isEdgeControl())
    {
        if ( textInteractionFlags() == Qt::NoTextInteraction)
            setTextInteractionFlags(Qt::TextEditorInteraction);
			//QGraphicsTextItem::mousePressEvent(event);
    }
}

void NodeItem::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
	setFlag( ItemIsFocusable, false);
    QGraphicsTextItem::focusOutEvent(event);
}

/**
 * We should adjust edges when entering the text as the size of item changes
 */
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

/** 
 * Perform animation step
 * Advance node's coordinates and opacity towards goal values of these parameters
 * Return true if node have advanced somehow. False if node hasn't change
 */
bool NodeItem::advance()
{
    QPointF target( node()->modelX(), node()->modelY());
    QLineF line( pos(), target); 
    qreal dist = line.length();
    bool changed = false;

    if ( !isVisible())
        return false;

    qreal target_opacity = ((qreal)node()->priority())/6;
    
    if ( target_opacity > opacityLevel() + OPACITY_STEP)
    {
        setOpacityLevel( opacityLevel() + OPACITY_STEP);
        changed = true;
    } else if ( target_opacity < opacityLevel() - OPACITY_STEP)
    {
        setOpacityLevel( opacityLevel() - OPACITY_STEP);
        changed = true;
    } else
    {
        setOpacityLevel( target_opacity);
        if ( !target_opacity)
        {
            setVisible( false);
            changed = true;
        }
    }
    if ( changed)
    {
        adjustAssociates();
        updateAssociates();
        update();
    }

    if ( dist > NODE_SPEED)
    {
        QPointF displacement( NODE_SPEED * line.dx() / dist, NODE_SPEED * line.dy() / dist);
        setPos( pos() + displacement);   
        changed = true;
    }
    return changed;
}

void NodeItem::adjustAssociates()
{
    GEdge *edge = NULL;

    for ( edge = node()->firstSucc(); isNotNullP( edge); edge = edge->nextSucc())
    {
        edge->item()->adjust();
        GNode* succ = edge->succ();

        if ( succ->isEdgeControl() || succ->isEdgeLabel())
        {
            assert( isNotNullP( succ->firstSucc()));
            succ->firstSucc()->item()->adjust();
        }
    }
    for ( edge = node()->firstPred(); isNotNullP( edge); edge = edge->nextPred())
    {
        edge->item()->adjust();
        GNode* pred = edge->pred();

        if ( pred->isEdgeControl() || pred->isEdgeLabel())
        {
            assert( isNotNullP( pred->firstPred()));
            pred->firstPred()->item()->adjust();
        }
    }
}

void NodeItem::updateAssociates()
{
    GEdge *edge = NULL;

    for ( edge = node()->firstSucc(); isNotNullP( edge); edge = edge->nextSucc())
    {
        edge->item()->adjust();
        GNode* succ = edge->succ();

        if ( succ->isEdgeControl() || succ->isEdgeLabel())
        {
            assert( isNotNullP( succ->firstSucc()));
            succ->firstSucc()->item()->update();
        }
    }
    for ( edge = node()->firstPred(); isNotNullP( edge); edge = edge->nextPred())
    {
        edge->item()->adjust();
        GNode* pred = edge->pred();

        if ( pred->isEdgeControl() || pred->isEdgeLabel())
        {
            assert( isNotNullP( pred->firstPred()));
            pred->firstPred()->item()->update();
        }
    }
}
/**
 * Adjust edges when node changes
 */
QVariant NodeItem::itemChange( GraphicsItemChange change, const QVariant &value)
{
    GEdge *edge = NULL;

    if ( change != QGraphicsItem::ItemSceneChange 
         || change != QGraphicsItem::ItemSceneHasChanged)
    {
        adjustAssociates();
    }
    return QGraphicsTextItem::itemChange(change, value);
}


