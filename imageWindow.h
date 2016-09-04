#pragma once

enum class EViewChannel : uint8_t
{
	RGB,
	R,
	G,
	B,
	A
};

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

	void	paintEvent( QPaintEvent* event );
};

class CImageWindow : public QScrollArea
{
    Q_OBJECT

public:
    CImageWindow();
	bool			LoadFile( QString const& path );
	void			Reload();
	void			SetViewChannel( EViewChannel channel );
	void			SetViewFace( unsigned face );
	void			SetViewMipMap( unsigned mipMap );
	void			SetViewMin( float min );
	void			SetViewMax( float max );
	void			SetViewGamma( float gamma );
	unsigned		GetImageWidth() const		{ return m_imageWidth; }
	unsigned		GetImageHeight() const		{ return m_imageHeight; }
	EViewChannel	GetViewChannel() const		{ return m_viewChannel; }
	unsigned		GetMipNum()	const			{ return m_info.mipLevels; }
	unsigned		GetFaceNum() const			{ return m_info.arraySize; }
	unsigned		GetViewFace() const			{ return m_viewFace; }
	unsigned		GetViewMipMap() const		{ return m_viewMipMap; }
	float			GetViewMin() const			{ return m_viewMin; }
	float			GetViewMax() const			{ return m_viewMax; }
	float			GetViewGamma() const		{ return m_viewGamma; }


private:
	DirectX::ScratchImage*	m_scratchImage;
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
	void					PickTexel( unsigned tx, unsigned ty );
	void					UpdateImage();	
	void					UpdateTitle() const;
};