/**
 * @file: ir.h 
 * Description of expressions' graph
 * @author Boris Shurygin
 */
/**
 * IR interface
 * Copyright (C) 2009  Boris Shurygin
 */
#pragma once
#ifndef IR_H
#define IR_H

/**
 * Intermediate representation
 */
class IR: public GGraph
{
public:
    IR();
    ~IR();
};

class IRView: public GraphView
{

};

#endif /* IR_H */