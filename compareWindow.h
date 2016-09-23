#pragma once

#include "baseWindow.h"

class CCompareImageLabel : public QLabel
{
	Q_OBJECT

public:
    CCompareImageLabel();
	void SetZoom( float zoom );
	void SetImage( QImage const& image );
	void SetCrossPos( QPoint const& pos );


private:
	QPixmap	m_pixmap;
	QPixmap	m_crossPixmap;
	QPoint	m_crossPos;
	float	m_zoom;

	void	paintEvent( QPaintEvent* event ) Q_DECL_OVERRIDE;
};

class CCompareWindow : public QWidget, public CBaseWindow
{
    Q_OBJECT

public:
    CCompareWindow();
	~CCompareWindow();
	bool					LoadFiles( QString const& path0, QString const& path1 );
	virtual void			Reload();
	virtual void			SetViewChannel( EViewChannel channel );
	virtual void			SetViewFace( unsigned face );
	virtual void			SetViewMipMap( unsigned mipMap );
	virtual void			SetViewMin( float min );
	virtual void			SetViewMax( float max );
	virtual void			SetViewGamma( float gamma );
	virtual void			SetViewDiffMult( float mult );
	virtual QSize			GetInitialSize() const		{ return QSize( m_imageWidth, m_imageHeight ); }
	virtual QString const&	GetTitle() const			{ return m_title; }
	virtual EViewChannel	GetViewChannel() const		{ return m_viewChannel; }
	virtual unsigned		GetMipNum()	const			{ return m_info[ 0 ].mipLevels; }
	virtual unsigned		GetFaceNum() const			{ return m_info[ 0 ].arraySize; }
	virtual unsigned		GetViewFace() const			{ return m_viewFace; }
	virtual unsigned		GetViewMipMap() const		{ return m_viewMipMap; }
	virtual float			GetViewMin() const			{ return m_viewMin; }
	virtual float			GetViewMax() const			{ return m_viewMax; }
	virtual float			GetViewGamma() const		{ return m_viewGamma; }
	virtual float			GetViewDiffMult() const		{ return m_viewDiffMult; }

private slots:
	void ScrollBarHRangeChanged( int min, int max );
	void ScrollBarVRangeChanged( int min, int max );
	void ScrollBarHValueChanged( int value );
	void ScrollBarVValueChanged( int value );

private:
	QString					m_title;
	DirectX::ScratchImage	m_scratchImage[ 2 ];
	DirectX::TexMetadata	m_info[ 2 ];
	QString					m_path[ 2 ];
	QString					m_fileName[ 2 ];
	QString					m_formatName[ 2 ];
	QGridLayout				m_gridLayout;
	QScrollArea				m_scrollArea0;
	QScrollArea				m_scrollArea1;
	QScrollArea				m_scrollArea2;
	CCompareImageLabel		m_imageLabel0;
	CCompareImageLabel		m_imageLabel1;
	CCompareImageLabel		m_imageLabel2;
	QScrollBar				m_scrollBarH;
	QScrollBar				m_scrollBarV;
	float					m_rmse;
	float					m_zoom;
	bool					m_dragEnabled;
	QPoint					m_dragStart;
	EViewChannel			m_viewChannel;
	unsigned				m_texelSizeInBytes;
	unsigned				m_viewFace;
	unsigned				m_viewMipMap;
	float					m_viewMin;
	float					m_viewMax;
	float					m_viewGamma;
	float					m_viewDiffMult;
	unsigned				m_imageWidth;
	unsigned				m_imageHeight;

	void					mousePressEvent( QMouseEvent* event ) Q_DECL_OVERRIDE;
	void					mouseReleaseEvent( QMouseEvent* event ) Q_DECL_OVERRIDE;
	void					mouseMoveEvent( QMouseEvent* event ) Q_DECL_OVERRIDE;
	void					wheelEvent( QWheelEvent* event ) Q_DECL_OVERRIDE;
	bool					eventFilter( QObject* object, QEvent* event ) Q_DECL_OVERRIDE;
	void					UpdateImage();	
	void					PickTexel( QPoint const& pos );
	void					UpdateCrossCursor( QPoint const& cursorPos );
	void					UpdateTitle();
};