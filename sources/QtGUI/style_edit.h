/**
 * @file: style_edit.h
 * Interface of style-manipulation widgets
 */
/*
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#pragma once

#ifndef STYLE_EDIT_H
#define STYLE_EDIT_H

#include "gview_impl.h"
/**
 * Colored button
 */
class ColorButton: public QAbstractButton
{
public:
    ColorButton( QWidget *parent = 0);
    void paintEvent( QPaintEvent *event);
    QSize sizeHint() const;
    void setColor( QColor &cl)
    {
        color = cl;
    }
private:
    QColor color;
};

/**
 * Widget for creating/editing styles
 */
class StyleEdit: public QWidget
{
    Q_OBJECT;
public:
    /** Constructor */
    StyleEdit( QWidget *parent = 0);
   
public slots:
    /** Change name of the current style */
    void changeStyleName();
    /** Invoke color selection for line */
    void selectLineColor();
    /** Change line style */
    void changeLineStyle();
    /** Change line width */
    void changeLineWidth( double width);
    /** Invoke color selection for fill */
    void selectFillColor();
private:
    //QHash< QString, GStyle *> styles;
    QLabel *name_label;
    QLabel *line_color_label;
    QLabel *line_style_label;
    QLabel *line_width_label;
    QLabel *fill_color_label;

    QComboBox *name_combo;
    ColorButton *line_color_button;
    QComboBox *line_style_combo;
    QDoubleSpinBox *line_width_spin;
    ColorButton *fill_color_button;
};

#endif