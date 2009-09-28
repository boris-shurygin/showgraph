/**
 * File: text_view.h - TextView ( IR dump browser) class definition.
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef TEXT_VIEW_H
#define TEXT_VIEW_H

class TextView: public QTextBrowser
{
public:
    /**
     * Open document in browser
     */
    void openFile( QString fileName);
};


#endif /* TEXT_VIEW_H */
