#include "stdafx.h"
#include "imageWindow.h"
#include "mainWindow.h"
#include "util.h"

CImageWindow::CImageWindow()
	: m_zoom( 1.0f )
	, m_viewChannel( EViewChannel::RGB )
	, m_viewFace( 0 )
	, m_viewMipMap( 0 )
	, m_imageWidth( 0 )
	, m_imageHeight( 0 )
	, m_viewMin( 0.0f )
	, m_viewMax( 1.0f )
	, m_viewGamma( 1.0f )
{
	memset( &m_info, 0, sizeof( m_info ) );

	setAttribute( Qt::WA_DeleteOnClose );
    setWidget( &m_imageLabel );
}

bool CImageWindow::LoadFile( QString const& path )
{
	m_path = path;
	if ( !UtilLoadFile( m_scratchImage, m_info, m_formatName, m_fileName, m_texelSizeInBytes, path ) )
	{
		memset( &m_info, 0, sizeof( m_info ) );
		return false;
	}

	( (QMdiSubWindow*) parentWidget() )->setWindowTitle( path );
	UpdateTitle();
	UpdateImage();
	return true;
}

void CImageWindow::Reload()
{
	LoadFile( m_path );
}

void CImageWindow::UpdateTitle()
{
	m_title = QString( "%1 %2x%3 %4 mip:%5/%6 face:%7/%8 zoom:%9%" )
		.arg( m_fileName )
		.arg( m_info.width )
		.arg( m_info.height )
		.arg( m_formatName )
		.arg( m_viewMipMap )
		.arg( m_info.mipLevels )
		.arg( m_viewFace )
		.arg( m_info.arraySize )
		.arg( m_zoom * 100.0f );

	extern CMainWindow* GMainWindow;
	GMainWindow->SetStatusLeft( m_title );
}

void CImageWindow::SetViewChannel( EViewChannel channel )
{
	if ( m_viewChannel != channel ) 
	{
		m_viewChannel = channel;
		UpdateImage();
		UpdateTitle();
	}
}

void CImageWindow::SetViewFace( unsigned face )
{
	if ( m_viewFace != face )
	{
		m_viewFace = face;
		UpdateImage();
		UpdateTitle();
	}
}

void CImageWindow::SetViewMipMap( unsigned mipMap )
{
	if ( m_viewMipMap != mipMap )
	{
		m_viewMipMap = mipMap;
		UpdateImage();
		UpdateTitle();
	}
}

void CImageWindow::SetViewMin( float min )
{
	if ( m_viewMin != min )
	{
		m_viewMin = min;
		UpdateImage();
	}
}

void CImageWindow::SetViewMax( float max )
{
	if ( m_viewMax != max )
	{
		m_viewMax = max;
		UpdateImage();
	}
}

void CImageWindow::SetViewGamma( float gamma )
{
	if ( m_viewGamma != gamma )
	{
		m_viewGamma = gamma;
		UpdateImage();
	}
}

void CImageWindow::UpdateImage()
{
	m_viewFace		= std::min<unsigned>( m_viewFace,	m_info.arraySize - 1 );
	m_viewMipMap	= std::min<unsigned>( m_viewMipMap,	m_info.mipLevels - 1 );

	DirectX::Image const* img = m_scratchImage.GetImage( m_viewMipMap, m_viewFace, 0 );
	uint8_t* srcPtr = img->pixels;
	m_imageWidth	= img->width;
	m_imageHeight	= img->height;

	unsigned redIdx		= 0;
	unsigned greenIdx	= 1;
	unsigned blueIdx	= 2;
	switch ( m_viewChannel )
	{
		case EViewChannel::R:	redIdx = greenIdx = blueIdx = 0; break;
		case EViewChannel::G:	redIdx = greenIdx = blueIdx = 1; break;
		case EViewChannel::B:	redIdx = greenIdx = blueIdx = 2; break;
		case EViewChannel::A:	redIdx = greenIdx = blueIdx = 3; break;
	}

	float const viewScale = 1.0f / ( m_viewMax - m_viewMin );
	uint8_t* dataU8 = new uint8_t[ m_imageWidth * m_imageHeight * 4 ];
	uint8_t* dataPtr = dataU8;

	auto AddTexel = [&]( float texel[ 4 ] )
	{
		texel[ 0 ] = powf( ( texel[ 0 ] - m_viewMin ) * viewScale, m_viewGamma );
		texel[ 1 ] = powf( ( texel[ 1 ] - m_viewMin ) * viewScale, m_viewGamma );
		texel[ 2 ] = powf( ( texel[ 2 ] - m_viewMin ) * viewScale, m_viewGamma );
		texel[ 3 ] = powf( ( texel[ 3 ] - m_viewMin ) * viewScale, m_viewGamma );

		dataPtr[ 0 ] = (uint8_t) ClampF( texel[ redIdx ]	* 255.0f + 0.5f, 0.0f, 255.0f );
		dataPtr[ 1 ] = (uint8_t) ClampF( texel[ greenIdx ]	* 255.0f + 0.5f, 0.0f, 255.0f );
		dataPtr[ 2 ] = (uint8_t) ClampF( texel[ blueIdx ]	* 255.0f + 0.5f, 0.0f, 255.0f );
		dataPtr += 3;
	};

	float texel[ 4 ];
	switch ( m_info.format )
	{
		case DXGI_FORMAT_R8_TYPELESS:
		case DXGI_FORMAT_R8_UNORM:
		case DXGI_FORMAT_R8_UINT:
		case DXGI_FORMAT_R8_SNORM:
		case DXGI_FORMAT_R8_SINT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR8_UNorm( texel, srcPtr );
					AddTexel( texel );
				}
			}
			break;
			
		case DXGI_FORMAT_R8G8_SNORM:
		case DXGI_FORMAT_R8G8_UNORM:
		case DXGI_FORMAT_R8G8_UINT:
		case DXGI_FORMAT_R8G8_SINT:
		case DXGI_FORMAT_R8G8_TYPELESS:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR8G8_UNorm( texel, srcPtr );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_B8G8R8A8_UNORM:
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadB8G8R8A8_UNorm( texel, srcPtr );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_B8G8R8X8_UNORM:
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadB8G8R8X8_UNorm( texel, srcPtr );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_R8G8B8A8_TYPELESS:
		case DXGI_FORMAT_R8G8B8A8_UINT:
		case DXGI_FORMAT_R8G8B8A8_UNORM:
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR8G8B8A8_UNorm( texel, srcPtr );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_D16_UNORM:
		case DXGI_FORMAT_R16_UINT:
		case DXGI_FORMAT_R16_TYPELESS:
		case DXGI_FORMAT_R16_SNORM:
		case DXGI_FORMAT_R16_SINT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadD16_UNorm( texel, srcPtr );
					AddTexel( texel );
				}
			}
			break;
			
		case DXGI_FORMAT_R10G10B10A2_TYPELESS:
		case DXGI_FORMAT_R10G10B10A2_UINT:
		case DXGI_FORMAT_R10G10B10A2_UNORM:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR10G10B10A2_UNorm( texel, srcPtr );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_R11G11B10_FLOAT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR11G11B10_Float( texel, srcPtr );
					AddTexel( texel );
				}
			}
			break;
			
		case DXGI_FORMAT_R16G16B16A16_FLOAT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR16G16B16A16_Float( texel, srcPtr );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR32G32B32A32_Float( texel, srcPtr );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_R32G32B32A32_UINT:
		case DXGI_FORMAT_R32G32B32A32_TYPELESS:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR32G32B32A32_UInt( texel, srcPtr );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_R32G32B32A32_SINT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR32G32B32A32_SInt( texel, srcPtr );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_R32_UINT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR32_UInt( texel, srcPtr );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_R32_SINT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR32_SInt( texel, srcPtr );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_D32_FLOAT:
		case DXGI_FORMAT_R32_FLOAT:
		case DXGI_FORMAT_R32_TYPELESS:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR32_Float( texel, srcPtr );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_R16_FLOAT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR16_Float( texel, srcPtr );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		case DXGI_FORMAT_R24G8_TYPELESS:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR24G8_UInt( texel, srcPtr );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_R32G8X24_TYPELESS:
		case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
		case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR32S8( texel, srcPtr );
					AddTexel( texel );
				}
			}
			break;
	}

	QImage image = QImage( dataU8, m_imageWidth, m_imageHeight, m_imageWidth * 3, QImage::Format_RGB888 );
	m_imageLabel.SetImage( image );
	delete[] dataU8;

	m_imageLabel.setFixedSize( m_imageWidth * m_zoom, m_imageHeight * m_zoom );
	m_imageLabel.update();
}

void CImageWindow::mousePressEvent( QMouseEvent* event )
{
	if ( event->buttons() & Qt::LeftButton )
	{
		m_dragEnabled	= true;
		m_dragStart		= QPoint( horizontalScrollBar()->value(), verticalScrollBar()->value() ) + event->pos();	
	}
	
	if ( event->buttons() & Qt::RightButton )
	{
		PickTexel( event->pos() );
	}
}

void CImageWindow::mouseReleaseEvent( QMouseEvent* event )
{
	if ( event->buttons() & Qt::LeftButton )
	{
		m_dragEnabled = false;
	}
}

void CImageWindow::mouseMoveEvent( QMouseEvent* event )
{
	if ( event->buttons() & Qt::LeftButton && m_dragEnabled )
	{
		QPoint offset = m_dragStart - event->pos();
		horizontalScrollBar()->setValue( offset.x() );
		verticalScrollBar()->setValue( offset.y() );
	}

	if ( event->buttons() & Qt::RightButton )
	{
		PickTexel( event->pos() );
	}
}

void CImageWindow::PickTexel( QPoint const& pos )
{
	unsigned texelX = ClampF( ( pos.x() + horizontalScrollBar()->value() - 0.5f ) / m_zoom, 0.0f, m_imageWidth  - 1.0f );
	unsigned texelY = ClampF( ( pos.y() + verticalScrollBar()->value() - 0.5f )	 / m_zoom, 0.0f, m_imageHeight - 1.0f );

	QString texelInfo[ 4 ];
	if ( texelX >= 0 && texelX < m_imageWidth && texelY >= 0 && texelY < m_imageHeight )
	{
		DirectX::Image const* img = m_scratchImage.GetImage( m_viewMipMap, m_viewFace, 0 );
		uint8_t* srcPtr = img->pixels + m_texelSizeInBytes * ( texelX + texelY * m_imageWidth );

		switch ( m_info.format )
		{
			case DXGI_FORMAT_R8_TYPELESS:
			case DXGI_FORMAT_R8_UNORM:
			case DXGI_FORMAT_R8_UINT:
			case DXGI_FORMAT_R8_SNORM:
			case DXGI_FORMAT_R8_SINT:
				{
					TexelInfoR8_UNorm( texelInfo, srcPtr );
				}
				break;

			case DXGI_FORMAT_R8G8_SNORM:
			case DXGI_FORMAT_R8G8_UNORM:
			case DXGI_FORMAT_R8G8_UINT:
			case DXGI_FORMAT_R8G8_SINT:
			case DXGI_FORMAT_R8G8_TYPELESS:
				{
					TexelInfoR8G8_UNorm( texelInfo, srcPtr );
				}
				break;

			case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
			case DXGI_FORMAT_R24G8_TYPELESS:
				{
					TexelInfoR24G8_UInt( texelInfo, srcPtr );
				}
				break;

			case DXGI_FORMAT_B8G8R8X8_UNORM:
			case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
				{
					TexelInfoB8G8R8X8_UNorm( texelInfo, srcPtr );
				}
				break;

			case DXGI_FORMAT_B8G8R8A8_UNORM:
			case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
				{
					TexelInfoB8G8R8A8_UNorm( texelInfo, srcPtr );
				}
				break;

			case DXGI_FORMAT_R10G10B10A2_TYPELESS:
			case DXGI_FORMAT_R10G10B10A2_UINT:
			case DXGI_FORMAT_R10G10B10A2_UNORM:
				{
					TexelInfoR10G10B10A2_UNorm( texelInfo, srcPtr );
				}
				break;

			case DXGI_FORMAT_R8G8B8A8_TYPELESS:
			case DXGI_FORMAT_R8G8B8A8_UINT:
			case DXGI_FORMAT_R8G8B8A8_UNORM:
			case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
				{
					TexelInfoR8G8B8A8_UNorm( texelInfo, srcPtr );
				}
				break;

			case DXGI_FORMAT_R11G11B10_FLOAT:
				{
					TexelInfoR11G11B10_Float( texelInfo, srcPtr );
				}
				break;

			case DXGI_FORMAT_R16G16B16A16_FLOAT:
				{
					TexelInfoR16G16B16A16_Float( texelInfo, srcPtr );
				}
				break;

			case DXGI_FORMAT_R32G32B32A32_FLOAT:
				{
					TexelInfoR32G32B32A32_Float( texelInfo, srcPtr );
				}
				break;

			case DXGI_FORMAT_R32G32B32A32_UINT:
			case DXGI_FORMAT_R32G32B32A32_TYPELESS:
				{
					TexelInfoR32G32B32A32_UInt( texelInfo, srcPtr );
				}
				break;

			case DXGI_FORMAT_R32G32B32A32_SINT:
				{
					TexelInfoR32G32B32A32_SInt( texelInfo, srcPtr );
				}
				break;

			case DXGI_FORMAT_R32_UINT:
				{
					TexelInfoR32_UInt( texelInfo, srcPtr );
				}
				break;

			case DXGI_FORMAT_R32_SINT:
				{
					TexelInfoR32_SInt( texelInfo, srcPtr );
				}
				break;

			case DXGI_FORMAT_D32_FLOAT:
			case DXGI_FORMAT_R32_FLOAT:
			case DXGI_FORMAT_R32_TYPELESS:
				{
					TexelInfoR32_Float( texelInfo, srcPtr );
				}
				break;

			case DXGI_FORMAT_R16_FLOAT:
				{
					TexelInfoR16_Float( texelInfo, srcPtr );
				}
				break;

			case DXGI_FORMAT_D16_UNORM:
			case DXGI_FORMAT_R16_UINT:
			case DXGI_FORMAT_R16_TYPELESS:
			case DXGI_FORMAT_R16_SNORM:
			case DXGI_FORMAT_R16_SINT:
				{
					TexelInfoD16_UNorm( texelInfo, srcPtr );
				}
				break;

			case DXGI_FORMAT_R32G8X24_TYPELESS:
			case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
			case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
				{
					TexelInfoR32S8( texelInfo, srcPtr );
				}
				break;
		}
	}


	QString texelDesc = QString::number( texelX ) + "x" + QString::number( texelY );
	QString tooltip	= texelDesc;

	QString const channelNames[ 4 ] = { "R:", "G:", "B:", "A:" };
	for ( unsigned i = 0; i < 4; ++i )
	{
		if ( !texelInfo[ i ].isEmpty() )
		{
			texelDesc	+= " " + channelNames[ i ] + texelInfo[ i ];
			tooltip		+= "\n" + channelNames[ i ] + texelInfo[ i ];
		}
	}

	extern CMainWindow* GMainWindow;
	GMainWindow->SetStatusRight( texelDesc );
	QToolTip::showText( QCursor::pos(), tooltip );
}

void CImageWindow::wheelEvent( QWheelEvent* event )
{
	float const zoomMin = 0.125f;
	float const zoomMax = 32.0f;

	float newZoom = m_zoom;
	if ( event->angleDelta().y() > 0 )
	{
		newZoom *= 0.5f;
	}
	else
	{
		newZoom *= 2.0f;
	}
	newZoom = ClampF( newZoom, zoomMin, zoomMax );

	if ( fabs( newZoom - m_zoom ) > 0.001f )
	{
		float prevTexelX = ( event->x() + horizontalScrollBar()->value() - 0.5f )	/ m_zoom;
		float prevTexelY = ( event->y() + verticalScrollBar()->value() - 0.5f )		/ m_zoom;

		m_zoom = newZoom;
		m_imageLabel.SetZoom( newZoom );	
		m_imageLabel.setFixedSize( m_imageWidth * newZoom, m_imageHeight * newZoom );

		// scroll in order to hold to the same texel under the cursor after the zoom
		horizontalScrollBar()->setValue( prevTexelX * m_zoom - event->x() + 0.5f );
		verticalScrollBar()->setValue( prevTexelY * m_zoom - event->y() + 0.5f );

		UpdateTitle();
	}

	event->accept();
}

CImageLabel::CImageLabel()
	: m_zoom( 1.0f )
{
	
}

void CImageLabel::SetZoom( float zoom )
{
	m_zoom = zoom;
}

void CImageLabel::SetImage( QImage const& image )
{
	m_pixmap = QPixmap::fromImage( image );
}

void CImageLabel::paintEvent( QPaintEvent* event )
{
	QPainter paint( this );

	QRect dstRect( 0, 0, m_pixmap.width() * m_zoom, m_pixmap.height() * m_zoom );
	QRect srcRect( 0, 0, m_pixmap.width(), m_pixmap.height() );

    paint.drawPixmap( dstRect, m_pixmap, srcRect );
}