/**
 * @file: fe_llvm.cpp
 * Implementation of LLVM Frontend
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

/** Check if this line starts a new node text section */
bool
TestParser::nodeStartLLVM( QString line)
{
    return line.indexOf(":") != -1;
}
/** Check whether the given line should be treated as a next line */
bool
TestParser::nextLineLLVM( QString line)
{
    return true;
}

void
TestParser::parseLineLLVM( QString line)
{   
    /** Node recognition */
    QRegExp node_rx("^([^:]+):");
    QRegExp preds_rx("; preds = ");
    QTextStream stream( stdout);

            
    /** Expression recognition */
    int pos = 0;
    
    if ( node_rx.indexIn( line) != -1 )
    {
		QString name = node_rx.cap(1);
        
        /** Add node to symtab */
        if ( symtab.find( name ) == symtab.end())
        {
            SymNode* node = new SymNode( name);
            curr_node = static_cast<CFNode *>( graph->graph()->newNode());
            curr_node->setDoc( new QTextDocument());
            node->setNode( curr_node);
            node->node()->item()->setPlainText( name);
	        symtab[ name] = node;
#ifdef _DEBUG
            //stream << name << endl;
#endif
        } else
        {
            curr_node = static_cast< SymNode *>( symtab[ name])->node();
        }
    } 

    if ( preds_rx.indexIn( line) != -1)
    {
        int index = preds_rx.matchedLength();
        QRegExp name_rx("%([^,]+),?");
        while ( ( index = name_rx.indexIn( line, index) )!= -1)
        {
            index += name_rx.matchedLength();
            
            assert( isNotNullP( curr_node));
            QString pred_name = name_rx.cap(1);
            QString succ_name = curr_node->item()->toPlainText();
            QString name = QString("%1->%2")
                           .arg( pred_name)
                           .arg( succ_name);
            
            if ( symtab.find( pred_name) == symtab.end())
            {
                SymNode* node = new SymNode( pred_name);
                CFNode * pred_node = static_cast<CFNode *>( graph->graph()->newNode());
                pred_node->setDoc( new QTextDocument());
                node->setNode( pred_node);
                node->node()->item()->setPlainText( pred_name);
	            symtab[ pred_name] = node;
            }

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
            }
        }
    } 
    
    if ( !isStateNode())
        setStateDefault();
    
    if ( isStateNode())
    {
        node_text.append( line).append( "\n");
    }
}

/** Parse LLVM dump */
void TestParser::parseLLVMUnit( DumpUnitInfo *unit)
{
    total_lines_num = 0;
    DumpPos beg = unit->pos();
    DumpPos end = unit->end();
     if ( !file.isOpen() && !file.open( QIODevice::ReadOnly))
    {
        return;
    }
    file.reset();
    /** Read file line by line */
    QTextStream in( &file);
    QString line;
    unit_beg = 0;
    unit_id = 0;
    do
    {
        line = in.readLine();
        if ( total_lines_num >= beg)
        {
            break;  
        }
        total_lines_num++;
    } while ( !line.isNull());
    cur_line_num = 0;

    /** Init state */
    setStateInit();
#ifdef _DEBUG
        out( "Started parsing");
#endif
    do
    {
        curr_line = in.readLine();
        cur_line_num++;
        if ( !nextLineLLVM( curr_line))
        {
            line.append( curr_line);
        } else
        {
            if ( !line.isNull())
            {
                if ( nodeStopLLVM( line))
                {
                    endNode();
                    setStateDefault();
                }
                if ( nodeStartLLVM( line))
                {
                    setStateNode();
                    startNode();
                }             
                parseLineLLVM( line);
            }
            line = curr_line;
        }
        if ( total_lines_num >= end)
            break;
        total_lines_num++;
    } while ( !curr_line.isNull());
    if ( isStateNode())
    {
        endNode();
    }
#ifdef _DEBUG
        out( "Finished parsing");
#endif
}

void 
TestParser::highlightTextLLVM( QTextDocument * doc)
{
    QTextCursor cursor( doc);
    QString text = doc->toPlainText();
    QRegExp exp("^([^:]+):");
    int index = text.indexOf( exp);
    QTextCharFormat plain;
        
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
        cursor.insertText( QString( "%1").arg( exp.cap( 1)), link_fmt);
        cursor.insertText( QString( ":"), plain);
        index = text.indexOf( exp, index + length);
    }

    QRegExp edge_rx("CF EDGE (\\d+) \\[(\\d+)->(\\d+)\\]");
    index = text.indexOf( edge_rx);
    while (index >= 0)
    {
        int length = edge_rx.matchedLength();
        
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
