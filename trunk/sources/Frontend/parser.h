/**
 * File: parser.h - General compiler dump parser
 * Component of Frontend library in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef PARSER_H
#define PARSER_H

/**
 * Provides generic functionality for parsing files. 
 * It opens/closes files, reads them line by line and calls specific parsing functions.
 */
class Parser
{
    QFile file;
    QString curr_line;
protected: 
    /*virtual bool nextLine();
    virtual void parseLine();*/
public:
    Parser( QString filename);
    ~Parser();
    
    void convert2XML( QString xmlname);

    inline QString curr() const
    {
        return curr_line;
    }
};

#endif