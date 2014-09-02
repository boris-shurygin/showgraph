/**
 * @file: cfedge.cpp
 * Implementation of CFEdge
 * @author Boris Shurygin
 */
/**
 * IR implementation
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

CFEdge::CFEdge( CFG *graph_p, int _id, CFNode *_pred, CFNode* _succ):
    GEdge( (GGraph *)graph_p, _id, (GNode *)_pred, (GNode *)_succ)
{

}
   
CFEdge::~CFEdge()
{

}

CFNode * 
CFEdge::node( GraphDir dir) const 
{
    return static_cast< CFNode *>(AuxEdge::node( dir));
}

CFNode * 
CFEdge::realNode( GraphDir dir) const 
{
    return static_cast< CFNode *>(AuxEdge::realNode( dir));
}

CFG *
CFEdge::graph() const
{
    return static_cast< CFG *>( AuxEdge::graph());
}