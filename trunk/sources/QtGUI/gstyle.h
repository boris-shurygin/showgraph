/**
 * @file: gstyle.h 
 * Node/Edge styles in graph
 */
/*
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef GSTYLE_H
#define GSTYLE_H

/** Style id type */
typedef quint32 StyleId;

/**
 * Graphic style for nodes and edges
 */ 
class GStyle
{
public:
    /** Constructor */
    inline GStyle();
    /** Copy constructor */
    inline GStyle( const GStyle&);
    /** Copy constructor with name specification */
    inline GStyle( QString nm, const GStyle&);
    /** Assignment */
    inline GStyle& operator = ( const GStyle&);
    /** Destructor */
    virtual ~GStyle(){};
    /** Get name of style */
    inline QString name() const;
    /** Get pen */
    inline QPen pen() const;
    /** Get brush */
    inline QBrush brush() const;
    /** Check if style uses default pen */
    inline bool defaultPen() const;
    /** Check if style uses default brush */
    inline bool defaultBrush() const;

private:
    QString name_priv;
    QPen pen_priv;
    QBrush brush_priv;
    bool dflt_pen;
    bool dflt_brush;
    //StyleId id;
};

/** Constructor */
GStyle::GStyle(): 
    dflt_pen( true),
    dflt_brush( true)
{}

/** Copy constructor */
GStyle::GStyle( const GStyle& st)
{
    name_priv = st.name_priv;
    name_priv.append("_copy");
    pen_priv = st.pen_priv;
    brush_priv = st.brush_priv;
    dflt_pen = st.dflt_pen;
    dflt_brush = st.dflt_brush;
}
/** Copy constructor with name specification */
GStyle::GStyle( QString nm, const GStyle& st)
{
    name_priv = nm;
    pen_priv = st.pen_priv;
    brush_priv = st.brush_priv;
    dflt_pen = st.dflt_pen;
    dflt_brush = st.dflt_brush;
}

/** Assignment */
GStyle& GStyle::operator = ( const GStyle& st)
{
    name_priv = st.name_priv;
    name_priv.append("_copy");
    pen_priv = st.pen_priv;
    brush_priv = st.brush_priv;
    dflt_pen = st.dflt_pen;
    dflt_brush = st.dflt_brush;
}

/** Get name of style */
inline QString GStyle::name() const
{
    return name_priv;
}
/** Get pen */
inline QPen GStyle::pen() const
{
    return pen_priv;
}
/** Get brush */
inline QBrush GStyle::brush() const
{
    return brush_priv;
}
/** Check if style uses default pen */
inline bool GStyle::defaultPen() const
{
    return dflt_pen;
}
/** Check if style uses default brush */
inline bool GStyle::defaultBrush() const
{
    return dflt_brush;
}
#endif /* GSTYLE_H */