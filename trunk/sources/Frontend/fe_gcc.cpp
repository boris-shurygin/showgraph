/**
 * @file: fe_gcc.cpp
 * Implementation of GCC Frontend
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
TestParser::nodeStartGCC( QString line)
{
    return line.indexOf(";; Start of basic block") != -1;
}
/** Check whether the given line should be treated as a next line */
bool
TestParser::nextLineGCC( QString line)
{
    return true;
}

void
TestParser::parseLineGCC( QString line)
{   
    QString n_str("Node ");
    QString e_str("Edge ");
    /** Node recognition */
    QRegExp node_rx("^;; Start of basic block");
    QRegExp preds_rx("^;; Pred edge ");
    QRegExp succs_rx("^;; Succ edge ");
    QTextStream stream( stdout);

            
    /** Expression recognition */
    int pos = 0;

    if ( preds_rx.indexIn( line) == 0)
    {
        int index = preds_rx.matchedLength();
        QRegExp num_rx(" (\\d+)");
        while ( ( index = num_rx.indexIn( line, index) )!= -1)
        {
            index += num_rx.matchedLength();
            
            assert( isNotNullP( curr_node));
            int pred_num =  num_rx.cap(1).toInt();
            QString name = QString("%1->%2")
                           .arg( pred_num)
                           .arg( curr_node->irId());
            QString pred_name = QString("Node %1").arg( pred_num);
            QString succ_name = QString("Node %1").arg( curr_node->irId());
            
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
    } else if ( succs_rx.indexIn( line) == 0)
    {
        int index = succs_rx.matchedLength();
        QRegExp num_rx(" (\\d+)");
        while ( ( index = num_rx.indexIn( line, index) )!= -1)
        {
            index += num_rx.matchedLength();
            assert( isNotNullP( curr_node));
            int pred_num =  num_rx.cap(1).toInt();
            QString name = QString("%1->%2")
                           .arg( curr_node->irId())
                           .arg( pred_num);
            QString pred_name = QString("Node %1").arg( curr_node->irId());
            QString succ_name = QString("Node %1").arg( pred_num);
            
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
    } else if ( node_rx.indexIn( line) != -1 )
    {
		QRegExp num_rx("-> (\\d+)");
        if ( num_rx.indexIn( line) != -1)
        {
            bool good_id = false;
		    int ir_id = num_rx.cap(1).toInt( &good_id);
		    QString text = QString("Block ").append( num_rx.cap(1));
            QString name = n_str.append( num_rx.cap(1));
            
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

/** Parse GCC dump */
void TestParser::parseGCCUnit( DumpUnitInfo *unit)
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
        if ( !nextLineGCC( curr_line))
        {
            line.append( curr_line);
        } else
        {
            if ( !line.isNull())
            {
                if ( nodeStopGCC( line))
                {
                    endNode();
                    setStateDefault();
                }
                if ( nodeStartGCC( line))
                {
                    setStateNode();
                    startNode();
                }             
                parseLineGCC( line);
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
TestParser::highlightTextGCC( QTextDocument * doc)
{
    QTextCursor cursor( doc);
    QString text = doc->toPlainText();
    QRegExp exp(";; Start of basic block ([^\\n]*)");
    int index = 0;
    if ( (index = text.indexOf( exp)) != -1)
    {
        int length = exp.matchedLength();
        int num_index = 0;
        QString line = exp.cap( 1);
        int arrow_index = line.indexOf( "->");
        if ( arrow_index != -1)
        {
            cursor.setPosition( index);
            cursor.setPosition( index + length, QTextCursor::KeepAnchor);
            cursor.removeSelectedText();
            QTextCharFormat plain;
            QTextCharFormat link_fmt;
            link_fmt.setForeground(Qt::blue);
            link_fmt.setFontUnderline( true);
            link_fmt.setAnchor( true);
            QRegExp num_rx(" (\\d+)");
            cursor.insertText( QString( ";; Start of basic block ("), plain);
            while ( (num_index = line.indexOf( num_rx, num_index)) != -1)
            {
                if ( num_index > arrow_index)
                {
                    num_index += num_rx.matchedLength();
                    cursor.insertText( QString( ") -> "), plain);
                    link_fmt.setAnchorHref( num_rx.cap( 1));
                    cursor.insertText( num_rx.cap( 1), link_fmt);
                    break;
                } else
                {
                    num_index += num_rx.matchedLength();
                    cursor.insertText( QString( " "), plain);
                    link_fmt.setAnchorHref( num_rx.cap( 1));
                    cursor.insertText( num_rx.cap( 1), link_fmt);
                }
            }
        }
    }
    QRegExp edge_rx(";; (Succ|Pred) edge(\\s+)(\\d+)");
    index = text.indexOf( edge_rx);
    while (index >= 0)
    {
        int length = edge_rx.matchedLength();
        
        QTextCharFormat plain;
        QTextCharFormat link_fmt;
        link_fmt.setForeground(Qt::blue);
        link_fmt.setFontUnderline( true);
        link_fmt.setAnchor( true);
        link_fmt.setAnchorHref( edge_rx.cap( 3));
        cursor.setPosition( index);
        cursor.setPosition( index + length, QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
        cursor.insertText( QString( ";; "), plain);
        cursor.insertText( QString( "%1 edge%2")
                           .arg( edge_rx.cap( 1))
                           .arg( edge_rx.cap( 2)),
                           link_fmt);
        cursor.insertText( edge_rx.cap( 3), link_fmt);
        index = text.indexOf( edge_rx, index + length);
    }
}
