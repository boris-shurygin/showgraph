/**
 * @file: node_item.h 
 * Drawable Node class definition.
 */
/*
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef NODE_W_H
#define NODE_W_H

/**
 * Subclass of QGraphicsItem for representing a node in scene
 * @ingroup GUIGraph
 */
class NodeItem: public QGraphicsTextItem
{    
    GNode *node_p;
	bool bold_border;
    /** Initialization */
    void SetInitFlags();
public:
    /** Type of graphics item that corresponds to node */
    enum {Type = TypeNode};

    /** Constructor */
	inline NodeItem( GNode *n_p): bold_border( false)
    {
        node_p = n_p;
        SetInitFlags();
    }
    /** Get pointer to model node */
    inline GNode *node() const
    {
        return node_p;
    }
    /** Get the node type */
    inline int type() const
    {
        return Type;
    }
    /** Get the inner border rectangle */
    QRectF borderRect() const;
    /** Get the bounding rectangle */
    QRectF boundingRect() const;
    /** Get node shape */
    QPainterPath shape() const;
    /** Paint procedure */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    /** Reimplementation of mouse press event handler */
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    /** Reimplementation of mouse release event handler */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    /** Reimplementation of double click event handler */
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    /** Reimplementation of focus out event */
    void focusOutEvent(QFocusEvent *event);
    /** Reimplementation of key press event */
    void keyPressEvent(QKeyEvent *event);
    /** Item change event handler */
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
	/** Remove from scene */
    inline void remove()
    {
        setVisible( false);
        removeFromIndex();
        scene()->removeItem( this);
        node_p = NULL;
    }
};
/**
 * Representation of model graph node
 * @ingroup GUIGraph
 */
class GNode: public AuxNode
{
    /** Representation of node in graph view */
    NodeItem *item_p; 

    /** Representation of node as text */
    QTextDocument* _doc;
    
    /** We can't create nodes separately, do it through newNode method of graph */
    GNode( GGraph *graph_p, int _id);
    /** Contructor of node with specified position */
    GNode( GGraph *graph_p, int _id, QPointF _pos);

    friend class GraphT< GGraph, GNode, GEdge>;
    friend class GGraph;
    
public:
    /** Destructor */
    virtual ~GNode();
    /** Get the pointer to item */
    inline NodeItem* item() const
    {
        return item_p;
    }
    /** Get the corresponding text */
    inline QTextDocument *doc() const
    {
        return _doc;
    }
    /** Set node's text document */
    inline void setDoc( QTextDocument* doc)
    {
        _doc = doc;
    }
    
    /** 
     * Update DOM element
     */
    virtual void updateElement();

    /**
     * Read properties from XML
     */
    virtual void readFromElement( QDomElement elem);

    GGraph * graph() const;
    /** Get next graph's node */
    inline GNode* nextNode()
    {
        return static_cast< GNode*>( AuxNode::nextNode());
    }
    /** Edge connection reimplementation */
    inline void AddEdgeInDir( GEdge *edge, GraphDir dir)
    {
        AuxNode::AddEdgeInDir( (AuxEdge *)edge, dir);
    }
    /** Add predecessor */
    inline void AddPred( GEdge *edge)
    {
        AddEdgeInDir( edge, GRAPH_DIR_UP);
    }
    /** Add successor */
    inline void AddSucc( GEdge *edge) 
    {
        AddEdgeInDir( edge, GRAPH_DIR_DOWN);
    }
    /** Get first edge in given dircetion */
    inline GEdge* firstEdgeInDir( GraphDir dir)
    {
        return static_cast< GEdge*>( AuxNode::first_edge[ dir]);
    }
    /** Get first successor */
    inline GEdge* firstSucc()
    {
        return firstEdgeInDir( GRAPH_DIR_DOWN);
    }
    /** Get first predecessor */
    inline GEdge* firstPred()
    {
        return firstEdgeInDir( GRAPH_DIR_UP);
    }
    /** Get node's width */
    virtual inline double width() const
    {
        return item()->boundingRect().width();
    }
    /** Get node's nodes height */
    virtual inline double height() const
    {
        return item()->boundingRect().height();
    }
};
#endif