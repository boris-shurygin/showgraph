/**
 * File: node_w.h - Node Widget class definition.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef NODE_W_H
#define NODE_W_H

class NodeItem: public QGraphicsTextItem, public NodeT< GraphView, NodeItem, EdgeItem>
{    
    QList< AuxNode *> aux_nodes;
    AuxNode *aux_node;

    /** Initialization */
    inline void SetInitFlags()
    {
        QString text = QString("Node %1").arg( id());
        setPlainText( text);
        setFlag( ItemIsMovable);
        //setFlag(ItemIsSelectable);
        setCacheMode( DeviceCoordinateCache);
        setZValue(1);
    }

    /** We can't create nodes separately, do it through newNode method of graph */
    NodeItem( GraphView *graph_p, int _id):
        NodeT< GraphView, NodeItem, EdgeItem>( graph_p, _id)
    {
        SetInitFlags();
    }

    /** Contructor of node with specified position */
    NodeItem( GraphView *graph_p, int _id, QPointF _pos):
        NodeT< GraphView, NodeItem, EdgeItem>( graph_p, _id)
    {
        SetInitFlags();
        setPos( _pos);
    }

    friend class GraphT< GraphView, NodeItem, EdgeItem>;
    friend class GraphView;
public:
    ~NodeItem();

    enum {Type = TypeNode};

    int type() const
    {
        return Type;
    }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void keyPressEvent(QKeyEvent *event);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    
    /** 
     * Update DOM element
     */
    virtual void updateElement();

    /**
     * Read properties from XML
     */
    virtual void readFromElement( QDomElement elem);

    /** Set/Get aux node that represents node's start in aux graph */
    inline void setAuxNode( AuxNode *n)
    {
        aux_node = n;
    }
    inline AuxNode *auxNode() const
    {
        return aux_node;
    }
};

#endif