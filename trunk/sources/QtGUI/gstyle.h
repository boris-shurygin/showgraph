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
    /** Construction from XML description */
    inline GStyle( QDomElement e);
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
    inline bool isDefault() const;
    /** Saving to element */
    inline void writeElement( QDomElement e);
private:
    QString name_priv;
    QPen pen_priv;
    bool dflt_pen;
    //StyleId id;
};

/** Constructor */
inline GStyle::GStyle(): 
    dflt_pen( true)
{}

/** Copy constructor */
inline GStyle::GStyle( const GStyle& st)
{
    name_priv = st.name_priv;
    name_priv.append("_copy");
    pen_priv = st.pen_priv;
    dflt_pen = st.dflt_pen;
}
/** Copy constructor with name specification */
inline 
GStyle::GStyle( QString nm, const GStyle& st)
{
    name_priv = nm;
    pen_priv = st.pen_priv;
    dflt_pen = st.dflt_pen;
}

/** Construction from XML description */
inline 
GStyle::GStyle( QDomElement e ): 
    dflt_pen( true)
{
    ASSERTD( !e.isNull());
    ASSERTD( e.tagName() == QString( "style"));

    QString error_msg = QString("in line %1: ").arg( e.lineNumber());
        
    /** A style must be named in XML */
    if( !e.hasAttribute( "name"))
    {
        throw GGraphError( error_msg.append("style without a name"));
    } else
    {
       name_priv = e.attribute( "name ");    
    }
    /** Parse pen color */
    if ( e.hasAttribute( "pen_color"))
    {
        dflt_pen = false;
        QColor color( e.attribute( "pen_color"));
        
        if ( color.isValid())
        {
            pen_priv.setColor( color);
        } else
        {
            throw GGraphError( error_msg.append("invalid pen color"));
        }
    }
    /** Parse pen style */
    if ( e.hasAttribute( "pen_style"))
    {
        dflt_pen = false;
        
        QString stl = e.attribute( "pen_style");
        
        if ( stl == "no_pen")
        {
            pen_priv.setStyle( Qt::NoPen);
        } if ( stl == "solid")
        {
            pen_priv.setStyle( Qt::SolidLine);
        } if ( stl == "dash")
        {
            pen_priv.setStyle( Qt::DashLine);
        } if ( stl == "dot")
        {
            pen_priv.setStyle( Qt::DotLine);
        } if ( stl == "dash_dot")
        {
            pen_priv.setStyle( Qt::DashDotLine);
        } else
        {
            throw GGraphError( error_msg.append("invalid pen style"));
        }
    }
    /** Parse pen width */
    if ( e.hasAttribute( "pen_width"))
    {
        dflt_pen = false;
        bool ok;
        qreal width = e.attribute( "pen_width").toDouble( &ok);
        
        if ( ok)
        {
            pen_priv.setWidthF( width);
        } else
        {
            throw GGraphError( error_msg.append("invalid pen width"));
        }
    }
    /** Parse fill color */
    if ( e.hasAttribute( "fill"))
    {
        dflt_pen = false;
        QString fill = e.attribute( "fill");
        QColor color( fill);
        
        if ( fill == "no_fill")
        {
            pen_priv.setBrush( Qt::NoBrush);
        } else if ( color.isValid())
        {
            pen_priv.setBrush( color);
        } else
        {
            throw GGraphError( error_msg.append("invalid fill color"));
        }
    }
}

/** Saving to element */
inline void 
GStyle::writeElement( QDomElement e)
{
    if ( dflt_pen)
        return;
    
    e.setAttribute( "name", name_priv);

    /** Save pen color */
    e.setAttribute( "pen_color", pen_priv.color().name());

    /** Save pen style */
    Qt::PenStyle pen_style = pen_priv.style();

    switch ( pen_style)
    {
        case Qt::NoPen:
            e.setAttribute( "pen_style","no_pen");
            break;
        case Qt::SolidLine:
            e.setAttribute( "pen_style","solid");
            break;
        case Qt::DashLine:
            e.setAttribute( "pen_style","dash");
            break;
        case Qt::DotLine:           
            e.setAttribute( "pen_style","dot");
            break;
        case Qt::DashDotLine:
            e.setAttribute( "pen_style","dash_dot");
            break;
        default:
            ASSERTD( 0);
            e.setAttribute( "pen_style","solid");
            break;
    }
    /** Save pen width */
    e.setAttribute( "pen_width", pen_priv.widthF());
    
    /** Save fill color */
    QBrush brush = pen_priv.brush();
    
    if ( !( Qt::NoBrush == brush.style()))
    {
        e.setAttribute( "fill", brush.color().name());
    }
}

/** Assignment */
inline GStyle&
GStyle::operator = ( const GStyle& st)
{
    name_priv = st.name_priv;
    name_priv.append("_copy");
    pen_priv = st.pen_priv;
    dflt_pen = st.dflt_pen;
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
    return pen_priv.brush();
}
/** Check if style uses default pen */
inline bool GStyle::isDefault() const
{
    return dflt_pen;
}
#endif /* GSTYLE_H */