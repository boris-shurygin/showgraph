/**
 * @file: expr.cpp 
 * IR node implementation
 */
/* 
 * IR for ShowGraph tool.
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
#include "ir_impl.h"

/** We can't create nodes separately, do it through newNode method of graph */
Expr::Expr( IR *graph_p, int _id):
    GNode( ( GGraph *)graph_p, _id), 
    _prev( NULL),
    _next( NULL),
    _node( NULL)
{
    item()->hide();
    setForPlacement( false);
    setPriority( 0);
}

/** Contructor of node with specified position */
Expr::Expr( IR *graph_p, int _id, QPointF _pos):
    GNode( ( GGraph *)graph_p, _id),
    _prev( NULL),
    _next( NULL),
    _node( NULL)
{
    item()->hide();
    setPriority( 0);
    setForPlacement( false);
}

/**
 * Destructor for node - removes edge controls on incidient edges and disconnects item from scene
 */
Expr::~Expr()
{

}
