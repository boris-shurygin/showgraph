/**
 * @file: fe_gcc.cpp
 * Implementation of sample frontend
 */
/* 
 * Frontend in ShowGraph tool.
 * Copyright (c) 2009, Boris Shurygin
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "fe_iface.h"

TestParser::TestParser( QString str):
    Parser( str), curr_node( NULL)
{
    graph = new GraphView();
    graph->setGraph( new CFG( graph, true));
    graph->scene()->setItemIndexMethod( QGraphicsScene::NoIndex);
    static_cast< CFG *>( graph->graph())->setParser( this);
    setType( DUMP_TYPE_UNKNOWN);
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
    return line.indexOf("Node") != -1;
}

void
TestParser::startNode()
{
    node_text.clear();
}
void
TestParser::endNode()
{
    ASSERTD( isNotNullP( curr_node));
    curr_node->doc()->setPlainText( node_text);
}

bool TestParser::nextLine( QString line)
{
    return true;
}

void
TestParser::parseLine( QString line)
{
    QString n_str("Node ");
    QString e_str("Edge ");
    /** Node recognition */
    QRegExp node_rx("CF ?(Enter)? ?(\\w*) Node (\\d+)");
    
    /** Edge recognition */
    QRegExp edge_rx("CF EDGE (\\d+) \\[(\\d+)->(\\d+)\\]");
    QTextStream stream( stdout);
            
    /** Expression recognition */
    int pos = 0;

    if (  edge_rx.indexIn( line) != -1)
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
    } else if ( node_rx.indexIn( line) != -1 )
    {
		bool good_id = false;
		int ir_id = node_rx.cap(3).toInt( &good_id);
		QString text = QString("Node ").append( node_rx.cap(3));
        QString name = n_str.append( node_rx.cap(3));
        if ( !node_rx.cap( 1).isEmpty())
        {
            text.append("\n").append( node_rx.cap(1));
        }
        if ( !node_rx.cap( 2).isEmpty())
        {
            text.append("\n").append( node_rx.cap(2));
        }
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
/**
 * Select apropriate highlighter or run the default one
 */
void TestParser::highlightText( QTextDocument * doc)
{
    switch( type())
    {
    case DUMP_TYPE_GCC_CF:
        highlightTextGCC( doc);
        return;
    case DUMP_TYPE_ICC_IL0:
        highlightTextIL0( doc);
        return;
    case DUMP_TYPE_LLVM:
        highlightTextLLVM( doc);
        return;
    case DUMP_TYPE_UNKNOWN:
    default:
        break;
    }
    QTextCursor cursor( doc);
    QString text = doc->toPlainText();
    QRegExp exp("Node (\\d+)");
    int index = text.indexOf( exp);
    while (index >= 0)
    {
        int length = exp.matchedLength();

        QTextCharFormat link_fmt;
        //link_fmt.setFontWeight(QFont::Bold);
        link_fmt.setForeground(Qt::blue);
        link_fmt.setFontUnderline( true);
        link_fmt.setAnchor( true);
        link_fmt.setAnchorHref( exp.cap( 1));
        cursor.setPosition( index);
        cursor.setPosition( index + length, QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
        cursor.insertText( QString( "Node %1").arg( exp.cap( 1)), link_fmt);
        index = text.indexOf( exp, index + length);
    }

    QRegExp edge_rx("CF EDGE (\\d+) \\[(\\d+)->(\\d+)\\]");
    index = text.indexOf( edge_rx);
    while (index >= 0)
    {
        int length = edge_rx.matchedLength();
        
        QTextCharFormat plain;
        QTextCharFormat link_fmt;
        //link_fmt.setFontWeight(QFont::Bold);
        link_fmt.setForeground(Qt::blue);
        link_fmt.setFontUnderline( true);
        link_fmt.setAnchor( true);
        link_fmt.setAnchorHref( edge_rx.cap( 2));
        cursor.setPosition( index);
        cursor.setPosition( index + length, QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
        cursor.insertText( QString( "CF EDGE %1 [").arg( edge_rx.cap( 1)), plain);
        cursor.insertText( edge_rx.cap( 2), link_fmt);
        cursor.insertText( QString( "->"), plain);
        link_fmt.setAnchorHref( edge_rx.cap( 3));
        cursor.insertText( edge_rx.cap( 3), link_fmt);
        cursor.insertText( QString( "]"), plain);
         
        index = text.indexOf( edge_rx, index + length);
    }
}
