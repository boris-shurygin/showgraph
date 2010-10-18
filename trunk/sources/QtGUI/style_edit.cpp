/**
 * @file: style_edit.cpp 
 * Widgets for style manipulation
 */
/* 
 * GUI for ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */
#include "gview_impl.h"

ColorButton::ColorButton( QWidget *parent):
    QAbstractButton( parent)
{

}

void ColorButton::paintEvent( QPaintEvent *event)
{
    QPainter painter( this);
    painter.setPen( palette().foreground().color());
    painter.setBrush( color);
    painter.drawRect( rect());
}

QSize ColorButton::sizeHint() const
{
    return QSize( 20,20);
}

StyleEdit::StyleEdit( QWidget *parent)
{
    shape_combo = new QComboBox( this);
    shape_combo->addItem(tr("Box"), NODE_SHAPE_BOX);
    shape_combo->addItem(tr("Rounded box"), NODE_SHAPE_ROUNDED_BOX);
    shape_combo->addItem(tr("Ellipse"), NODE_SHAPE_ELLIPSE);
    shape_combo->addItem(tr("Circle"), NODE_SHAPE_CIRCLE);
    shape_combo->addItem(tr("Diamond"), NODE_SHAPE_DIAMOND);

    shape_label = new QLabel( "Shape:", this);
    shape_label->setBuddy( shape_combo);
    
    line_color_button = new ColorButton( this);
    line_color_button->setColor( QColor(palette().foreground().color()));
    
    line_color_label = new QLabel( "Line color:", this);
    line_color_label->setBuddy( line_color_button);

    line_style_combo = new QComboBox( this);
    line_style_combo->addItem(tr("Solid"), Qt::SolidLine);
    line_style_combo->addItem(tr("Dash"), Qt::DashLine);
    line_style_combo->addItem(tr("Dot"), Qt::DotLine);
    line_style_combo->addItem(tr("Dash Dot"), Qt::DashDotLine);
    line_style_combo->addItem(tr("Dash Dot Dot"), Qt::DashDotDotLine);
    line_style_combo->addItem(tr("None"), Qt::NoPen);

    line_style_label = new QLabel( "Line style:", this);
    line_style_label->setBuddy( line_style_combo);
    
    line_width_spin = new QDoubleSpinBox( this);
    line_width_spin->setMaximum( 12);
    
    line_width_label = new QLabel( "Line width:", this);
    line_width_label->setBuddy( line_width_spin);

    fill_color_button = new ColorButton( this);
    fill_color_button->setColor( QColor(palette().base().color()));
    
    fill_color_label = new QLabel( "Fill color:", this);
    fill_color_label->setBuddy( fill_color_button);


    ok = new QPushButton( "&OK", this);
    cancel = new QPushButton( "&Cancel", this);

    QGridLayout *mainLayout = new QGridLayout;
    //mainLayout->setColumnStretch(1, 3);
    
    //Row 0 name
    mainLayout->addWidget( shape_label, 0, 0, Qt::AlignRight);
    mainLayout->addWidget( shape_combo, 0, 1);
    //Row 1 line color
    mainLayout->addWidget( line_color_label, 1, 0, Qt::AlignRight);
    mainLayout->addWidget( line_color_button, 1, 1);
    //Row 2 line style
    mainLayout->addWidget( line_style_label, 2, 0, Qt::AlignRight);
    mainLayout->addWidget( line_style_combo, 2, 1);
     //Row 3 line width
    mainLayout->addWidget( line_width_label, 3, 0, Qt::AlignRight);
    mainLayout->addWidget( line_width_spin, 3, 1);
    //Row 4 fill color
    mainLayout->addWidget( fill_color_label, 4, 0, Qt::AlignRight);
    mainLayout->addWidget( fill_color_button, 4, 1);
    // Row 5 ok & cancel
    mainLayout->addWidget( ok, 5, 0);
    mainLayout->addWidget( cancel, 5, 1);
    
    setLayout( mainLayout);

    connect( shape_combo, SIGNAL( currentIndexChanged( int)), this, SLOT( changeShape()) );
    connect( line_color_button, SIGNAL( clicked()), this, SLOT( selectLineColor()) );
    connect( line_style_combo, SIGNAL( currentIndexChanged( int)), this, SLOT( changeLineStyle()) );
    connect( line_width_spin, SIGNAL( valueChanged( double)), this, SLOT( changeLineWidth( double)) );
    connect( fill_color_button, SIGNAL( clicked()), this, SLOT( selectFillColor()) );

    connect( ok, SIGNAL( clicked()), this, SLOT( accept()));
    connect( cancel, SIGNAL( clicked()), this, SLOT( reject()));

    setWindowTitle( "Edit style");
    setWindowFlags( windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
}

/** Change name of the current style */
void StyleEdit::changeStyleName()
{

}

/** Invoke color selection for line */
void StyleEdit::selectLineColor()
{
    QColor color = QColorDialog::getColor();
    line_color_button->setColor( color);
    gstyle->setPenColor( color);
    gstyle->setState();
    emit styleChanged( gstyle);
}

/** Change line style */
void StyleEdit::changeLineStyle()
{
    gstyle->setPenStyle((Qt::PenStyle)line_style_combo->itemData( line_style_combo->currentIndex()).toInt());
    gstyle->setState();
    emit styleChanged( gstyle);
}

/** Change line style */
void StyleEdit::changeShape()
{
    gstyle->setShape((NodeShape)shape_combo->itemData( shape_combo->currentIndex()).toInt());
    gstyle->setState();
    emit styleChanged( gstyle);
}

/** Change line width */
void StyleEdit::changeLineWidth( double width)
{
    gstyle->setPenWidth(line_width_spin->value());
    gstyle->setState();
    emit styleChanged( gstyle);
}

/** Invoke color selection for fill */
void StyleEdit::selectFillColor()
{
    QColor color = QColorDialog::getColor();
    fill_color_button->setColor( color);
    gstyle->setBrush( QBrush( color));
    gstyle->setState();
    emit styleChanged( gstyle);
}

/** Set style */
void StyleEdit::setGStyle( GStyle *st)
{
    gstyle = st;
    line_color_button->setColor( QColor( st->pen().color()));
    shape_combo->setCurrentIndex( shape_combo->findData( gstyle->shape()));
    line_style_combo->setCurrentIndex( line_style_combo->findData( gstyle->pen().style()));
    line_width_spin->setValue( gstyle->pen().widthF());
    fill_color_button->setColor( QColor( st->brush().color()));
}