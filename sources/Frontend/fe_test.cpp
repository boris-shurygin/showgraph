/**
 * @file: fe_gcc.cpp
 * Implementation of sample frontend
 */
/* 
 * Frontend in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "fe_iface.h"

TestParser::TestParser( QString str):
    Parser( str), curr_node( NULL)
{
    graph = new GraphView();
    ir = new GraphView();
    graph->setGraph( new CFG( graph));
    ir->setGraph( new IR( ir));
}

TestParser::~TestParser()
{

}

void
TestParser::convert2XML( QString xmlname)
{
    Parser::convert2XML( xmlname);
    graph->graph()->writeToXML( xmlname);
}

bool TestParser::nodeStart( QString line)
{
    /** Node recognition */
    QRegExp node_rx("Node (\\d+)");
    return  node_rx.indexIn( line) != -1;
}

void
TestParser::startNode()
{
    node_text.clear();
}
void
TestParser::endNode()
{
    assert( isNotNullP( curr_node));
    curr_node->doc()->setPlainText( node_text);
}

void
TestParser::parseLine( QString line)
{
    QString n_str("Node ");
    QString e_str("Edge ");
    /** Node recognition */
    QRegExp node_rx("Node (\\d+)");
    
    /** Edge recognition */
    QRegExp edge_rx("CF EDGE (\\d+) \\[(\\d+)->(\\d+)\\]");
    QTextStream stream( stdout);
            
    if ( node_rx.indexIn( line) != -1 )
    {
		bool good_id = false;
		int ir_id = node_rx.cap(1).toInt( &good_id);
		QString text = QString("Node ").append( node_rx.cap(1));
        QString name = n_str.append( node_rx.cap(1));
        /** Add node to symtab */
        if ( symtab.find( name ) == symtab.end())
        {
            SymNode* node = new SymNode( name);
            curr_node = static_cast<CFNode *>( graph->graph()->newNode());
            curr_node->setDoc( new QTextDocument());
            node->setNode( curr_node);
            node->node()->item()->setPlainText( text);
			if ( good_id)
			{
				node->node()->setIRId( ir_id);
			}
            symtab[ name] = node;
#ifdef _DEBUG
            //stream << name << endl;
#endif
        }
    } else if (  edge_rx.indexIn( line) != -1 )
    {
        QString name = e_str.append( edge_rx.cap(1));
        QString pred_name("Node ");
        pred_name.append( edge_rx.cap( 2));
        QString succ_name("Node ");
        succ_name.append( edge_rx.cap( 3));
        
        /** Back edge */
        QRegExp back_rx("Back");
    
        /** Add edge to symtab */
        if ( symtab.find( name) == symtab.end() 
             && symtab.find( pred_name) != symtab.end() 
             && symtab.find( succ_name) != symtab.end())
        {
            SymEdge *edge = new SymEdge( name);
            edge->setPred( pred_name);
            edge->setSucc( succ_name);
            symtab[ name] = edge;

            /** Add edge to graph */
            GNode* pred = static_cast< SymNode *>( symtab[ pred_name])->node();
            GNode* succ = static_cast< SymNode *>( symtab[ succ_name])->node();
            GEdge* e = graph->graph()->newEdge( pred, succ);
#ifdef _DEBUG            
            //stream << name << ": " << pred_name << "->" << succ_name << endl;
#endif      
            if (  back_rx.indexIn( line) != -1 && !e->isSelf())
            {
                GNode* label = e->insertLabelNode( QPointF( 0,0));                
                label->item()->setPlainText( "Back");
            }
        }
    } else
    {
        if ( !isStateNode())
            setStateDefault();
    }
    if ( isStateNode())
    {
        node_text.append( line).append( "\n");
    }
}

