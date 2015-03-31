# Basic Edition #
Basic editor operations are node/edge creation and edge routing.

### Creating nodes ###
Nodes are created by simply double-clicking anywhere on background. Newly created node has box shape, thin solid border and a label with its number.

### Node movement ###
Nodes can be dragged by pressing left mouse button on node and holding it while moving the node anywhere you like. Node is dropped when the mouse button is released.

### Creating edges ###
Nodes can be connected with edges by pressing right mouse button on predecessor node and holding it while moving mouse pointer to the successor node. Releasing mouse button on successor node creates new edge.

Most edges are created this way. However self-edges are not. To create a self edge you can use context menu of node (right click on it).

### Edge routing ###
Edges are routed by creating special nodes called _edge controls_ or _control nodes_. They are created by double-clicking on edge. When created they can be moved around like any other node.

Edge controls divide an edge into segments. Every edge is automatically routed so that it goes through controls and segments form a spline.

When you do not touch an edge its controls are hidden. They can be seen if you click on an edge. When you click on edge only the clicked segment is selected and its controls become visible.