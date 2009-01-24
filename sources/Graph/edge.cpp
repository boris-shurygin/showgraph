#include "graph_impl.h"

void Edge::DebugPrint()
{
    out("%u->%u;", GetPred()->GetId(), GetSucc()->GetId());
}