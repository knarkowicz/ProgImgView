#pragma once

#include "baseWindow.h"

class CImageLabel : public QLabel
{
	Q_OBJECT

public:
    CImageLabel();
	void SetZoom( float zoom );
	void SetImage( QImage const& image );


private:
	QPixmap	m_pixmap;
	float	m_zoom;

	void	paintEvent( QPaintEvent* event ) Q_DECL_OVERRIDE;
};

class CImageWindow : public QScrollArea, public CBaseWindow
{
    Q_OBJECT

public:
    CImageWindow();
	bool					LoadFile( QString const& path );
	virtual void			Reload();
	virtual void			ZoomIn();
	virtual void			ZoomOut();
	virtual void			SetViewChannel( EViewChannel channel );
	virtual void			SetViewFace( unsigned face );
	virtual void			SetViewMipMap( unsigned mipMap );
	virtual void			SetViewMin( float min );
	virtual void			SetViewMax( float max );
	virtual void			SetViewGamma( float gamma );
	virtual void			SetViewDiffMult( float mult ) {}
	virtual QSize			GetInitialSize() const		{ return QSize( m_imageWidth, m_imageHeight ); }
	virtual QString const&	GetTitle() const			{ return m_title; }
	virtual EViewChannel	GetViewChannel() const		{ return m_viewChannel; }
	virtual unsigned		GetMipNum()	const			{ return m_info.mipLevels; }
	virtual unsigned		GetFaceNum() const			{ return m_info.arraySize; }
	virtual unsigned		GetViewFace() const			{ return m_viewFace; }
	virtual unsigned		GetViewMipMap() const		{ return m_viewMipMap; }
	virtual float			GetViewMin() const			{ return m_viewMin; }
	virtual float			GetViewMax() const			{ return m_viewMax; }
	virtual float			GetViewGamma() const		{ return m_viewGamma; }
	virtual float			GetViewDiffMult() const		{ return 1.0f; }


private:
	QString					m_title;
	DirectX::ScratchImage	m_scratchImage;
	DirectX::TexMetadata	m_info;
	QString					m_path;
	QString					m_fileName;
	QString					m_formatName;
	CImageLabel				m_imageLabel;
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
	unsigned				m_imageWidth;
	unsigned				m_imageHeight;

	void					mousePressEvent( QMouseEvent* event ) Q_DECL_OVERRIDE;
	void					mouseReleaseEvent( QMouseEvent* event ) Q_DECL_OVERRIDE;
	void					mouseMoveEvent( QMouseEvent* event ) Q_DECL_OVERRIDE;
	void					wheelEvent( QWheelEvent* event ) Q_DECL_OVERRIDE;
	void					PickTexel( QPoint const& pos );
	void					UpdateImage();	
	void					UpdateTitle();
};