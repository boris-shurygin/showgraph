/**
 * File: fe_gcc.c - Implementation of GCC Frontend in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "fe_iface.h"

TestParser::TestParser( QString str):
    Parser( str)
{
    graph = new GraphView();
}

TestParser::~TestParser()
{
    delete graph;
}

void
TestParser::convert2XML( QString xmlname)
{
    Parser::convert2XML( xmlname);
    graph->writeToXML( xmlname);
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
        QString text = QString("Node ").append( node_rx.cap(1));
        QString name = n_str.append( node_rx.cap(1));
        /** Add node to symtab */
        if ( symtab.find( name ) == symtab.end())
        {
            SymNode* node = new SymNode( name);
            node->setNode( graph->newNode());
            node->node()->item()->setPlainText( text);
        
            symtab[ name] = node;
            stream << name << endl;
        }
    } else if (  edge_rx.indexIn( line) != -1 )
    {
        QString name = e_str.append( edge_rx.cap(1));
        QString pred_name("Node ");
        pred_name.append( edge_rx.cap( 2));
        QString succ_name("Node ");
        succ_name.append( edge_rx.cap( 3));
        
        /** Add edge to symtab */
        if ( symtab.find( name) == symtab.end())
        {
            SymEdge *edge = new SymEdge( name);
            edge->setPred( pred_name);
            edge->setSucc( succ_name);
            symtab[ name] = edge;
            stream << name << ": " << pred_name << "->" << succ_name << endl;
        }

        /** Add edge to graph */
        if ( symtab.find( pred_name) != symtab.end() 
             && symtab.find( succ_name) != symtab.end())
        {
            GNode* pred = static_cast< SymNode *>( symtab[ pred_name])->node();
            GNode* succ = static_cast< SymNode *>( symtab[ succ_name])->node();
            graph->newEdge( pred, succ);
        }
    }
}

