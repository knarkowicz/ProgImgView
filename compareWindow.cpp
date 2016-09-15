#include "stdafx.h"
#include "compareWindow.h"
#include "mainWindow.h"
#include "util.h"

CCompareImageLabel::CCompareImageLabel()
	: m_zoom( 1.0f )
{
	
}

void CCompareImageLabel::SetZoom( float zoom )
{
	m_zoom = zoom;
}

void CCompareImageLabel::SetImage( QImage const& image )
{
	m_pixmap = QPixmap::fromImage( image );
}

void CCompareImageLabel::paintEvent( QPaintEvent* event )
{
	QPainter paint( this );

	QRect dstRect( 0, 0, m_pixmap.width() * m_zoom, m_pixmap.height() * m_zoom );
	QRect srcRect( 0, 0, m_pixmap.width(), m_pixmap.height() );

    paint.drawPixmap( dstRect, m_pixmap, srcRect );
}

CCompareWindow::CCompareWindow()
	: m_zoom( 1.0f )
	, m_viewChannel( EViewChannel::RGB )
	, m_viewFace( 0 )
	, m_viewMipMap( 0 )
	, m_imageWidth( 0 )
	, m_imageHeight( 0 )
	, m_viewMin( 0.0f )
	, m_viewMax( 1.0f )
	, m_viewGamma( 1.0f )
	, m_viewDiffMult( 1.0f )
	, m_gridLayout( this )
	, m_scrollBarH( Qt::Orientation::Horizontal )
	, m_scrollBarV( Qt::Orientation::Vertical )
{
	memset( &m_info, 0, sizeof( m_info ) );
	setAttribute( Qt::WA_DeleteOnClose );

	connect( m_scrollArea0.verticalScrollBar(), &QScrollBar::rangeChanged, this, &CCompareWindow::ScrollBarVRangeChanged );
	connect( m_scrollArea0.horizontalScrollBar(), &QScrollBar::rangeChanged, this, &CCompareWindow::ScrollBarHRangeChanged );

	m_scrollArea0.setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	m_scrollArea0.setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	m_scrollArea1.setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	m_scrollArea1.setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	m_scrollArea2.setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	m_scrollArea2.setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	m_scrollArea0.setWidget( &m_imageLabel0 );
	m_scrollArea1.setWidget( &m_imageLabel1 );
	m_scrollArea2.setWidget( &m_imageLabel2 );

	m_gridLayout.setMargin( 0 );
	m_gridLayout.setSpacing( 0 );
	m_gridLayout.addWidget( &m_scrollArea0, 0, 0 );
	m_gridLayout.addWidget( &m_scrollArea1, 0, 1 );
	m_gridLayout.addWidget( &m_scrollArea2, 0, 2 );
	m_gridLayout.addWidget( &m_scrollBarV, 0, 3 );
	m_gridLayout.addWidget( &m_scrollBarH, 1, 0, 1, 3 );

	m_scrollArea0.viewport()->installEventFilter( this );
	m_scrollArea1.viewport()->installEventFilter( this );
	m_scrollArea2.viewport()->installEventFilter( this );
}

CCompareWindow::~CCompareWindow()
{
	m_scrollArea0.setWidget( nullptr );
	m_scrollArea1.setWidget( nullptr );
	m_scrollArea2.setWidget( nullptr );
}

bool CCompareWindow::LoadFiles( QString const& path0, QString const& path1 )
{
	m_path[ 0 ]	= path0;
	m_path[ 1 ] = path1;
	if ( !UtilLoadFile( m_scratchImage[ 0 ], m_info[ 0 ], m_formatName, m_fileName[ 0 ], m_texelSizeInBytes, path0 ) )
	{
		memset( &m_info, 0, sizeof( m_info ) );
		return false;
	}

	if ( !UtilLoadFile( m_scratchImage[ 1 ], m_info[ 1 ], m_formatName, m_fileName[ 1 ], m_texelSizeInBytes, path1 ) )
	{
		memset( &m_info, 0, sizeof( m_info ) );
		return false;
	}

	if ( m_info[ 0 ].format != m_info[ 1 ].format 
			|| m_info[ 0 ].width != m_info[ 1 ].width 
			|| m_info[ 0 ].height != m_info[ 1 ].height 
			|| m_info[ 0 ].mipLevels != m_info[ 1 ].mipLevels
			|| m_info[ 0 ].arraySize != m_info[ 1 ].arraySize )
	{
		memset( &m_info, 0, sizeof( m_info ) );
		return false;
	}

	( (QMdiSubWindow*) parentWidget() )->setWindowTitle( m_path[ 0 ] + " | " + m_path[ 1 ] );
	UpdateTitle();
	UpdateImage();
	return true;
}

void CCompareWindow::Reload()
{
	LoadFiles( m_path[ 0 ], m_path[ 1 ] );
}

void CCompareWindow::SetViewChannel( EViewChannel channel )
{
	if ( m_viewChannel != channel ) 
	{
		m_viewChannel = channel;
		UpdateImage();
		UpdateTitle();
	}
}

void CCompareWindow::SetViewFace( unsigned face )
{
	if ( m_viewFace != face )
	{
		m_viewFace = face;
		UpdateImage();
		UpdateTitle();
	}
}

void CCompareWindow::SetViewMipMap( unsigned mipMap )
{
	if ( m_viewMipMap != mipMap )
	{
		m_viewMipMap = mipMap;
		UpdateImage();
		UpdateTitle();
	}
}

void CCompareWindow::SetViewMin( float min )
{
	if ( m_viewMin != min )
	{
		m_viewMin = min;
		UpdateImage();
	}
}

void CCompareWindow::SetViewMax( float max )
{
	if ( m_viewMax != max )
	{
		m_viewMax = max;
		UpdateImage();
	}
}

void CCompareWindow::SetViewGamma( float gamma )
{
	if ( m_viewGamma != gamma )
	{
		m_viewGamma = gamma;
		UpdateImage();
	}
}

void CCompareWindow::SetViewDiffMult( float mult )
{
	if ( m_viewDiffMult != mult )
	{
		m_viewDiffMult = mult;
		UpdateImage();
	}
}

void CCompareWindow::UpdateImage()
{
	m_viewFace		= std::min<unsigned>( m_viewFace,	m_info[ 0 ].arraySize - 1 );
	m_viewMipMap	= std::min<unsigned>( m_viewMipMap,	m_info[ 0 ].mipLevels - 1 );

	DirectX::Image const* img[ 2 ];
	img[ 0 ] = m_scratchImage[ 0 ].GetImage( m_viewMipMap, m_viewFace, 0 );
	img[ 1 ] = m_scratchImage[ 1 ].GetImage( m_viewMipMap, m_viewFace, 0 );

	uint8_t* srcPtr[ 2 ];
	srcPtr[ 0 ] = img[ 0 ]->pixels;
	srcPtr[ 1 ] = img[ 1 ]->pixels;

	m_imageWidth	= img[ 0 ]->width;
	m_imageHeight	= img[ 0 ]->height;

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
	uint8_t* dataU8[ 3 ];
	dataU8[ 0 ] = new uint8_t[ m_imageWidth * m_imageHeight * 3 ];
	dataU8[ 1 ] = new uint8_t[ m_imageWidth * m_imageHeight * 3 ];
	dataU8[ 2 ] = new uint8_t[ m_imageWidth * m_imageHeight * 3 ];

	uint8_t* dataPtr[ 3 ];
	dataPtr[ 0 ] = dataU8[ 0 ];
	dataPtr[ 1 ] = dataU8[ 1 ];
	dataPtr[ 2 ] = dataU8[ 2 ];

	auto AddTexel = [&]( float texel[ 2 ][ 4 ] )
	{
		float const diffR = fabs( texel[ 1 ][ redIdx ]		- texel[ 0 ][ redIdx ] )	* m_viewDiffMult;
		float const diffG = fabs( texel[ 1 ][ greenIdx ]	- texel[ 0 ][ greenIdx ] )	* m_viewDiffMult;
		float const diffB = fabs( texel[ 1 ][ blueIdx ]		- texel[ 0 ][ blueIdx ] )	* m_viewDiffMult;

		for ( unsigned i = 0; i < 2; ++i )
		{
			texel[ i ][ 0 ] = powf( ( texel[ i ][ 0 ] - m_viewMin ) * viewScale, m_viewGamma );
			texel[ i ][ 1 ] = powf( ( texel[ i ][ 1 ] - m_viewMin ) * viewScale, m_viewGamma );
			texel[ i ][ 2 ] = powf( ( texel[ i ][ 2 ] - m_viewMin ) * viewScale, m_viewGamma );
			texel[ i ][ 3 ] = powf( ( texel[ i ][ 3 ] - m_viewMin ) * viewScale, m_viewGamma );

			dataPtr[ i ][ 0 ] = (uint8_t) ClampF( texel[ i ][ redIdx ]		* 255.0f + 0.5f, 0.0f, 255.0f );
			dataPtr[ i ][ 1 ] = (uint8_t) ClampF( texel[ i ][ greenIdx ]	* 255.0f + 0.5f, 0.0f, 255.0f );
			dataPtr[ i ][ 2 ] = (uint8_t) ClampF( texel[ i ][ blueIdx ]		* 255.0f + 0.5f, 0.0f, 255.0f );

			dataPtr[ i ] += 3;
		}

		dataPtr[ 2 ][ 0 ] = (uint8_t) ClampF( diffR * 255.0f + 0.5f, 0.0f, 255.0f );
		dataPtr[ 2 ][ 1 ] = (uint8_t) ClampF( diffG * 255.0f + 0.5f, 0.0f, 255.0f );
		dataPtr[ 2 ][ 2 ] = (uint8_t) ClampF( diffB * 255.0f + 0.5f, 0.0f, 255.0f );
		dataPtr[ 2 ] += 3;
	};
	
	float texel[ 2 ][ 4 ];
	switch ( m_info[ 0 ].format )
	{
		case DXGI_FORMAT_R8_TYPELESS:
		case DXGI_FORMAT_R8_UNORM:
		case DXGI_FORMAT_R8_UINT:
		case DXGI_FORMAT_R8_SNORM:
		case DXGI_FORMAT_R8_SINT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR8_UNorm( texel[ 0 ], srcPtr[ 0 ] );
					ReadR8_UNorm( texel[ 1 ], srcPtr[ 1 ] );
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
					ReadR8G8_UNorm( texel[ 0 ], srcPtr[ 0 ] );
					ReadR8G8_UNorm( texel[ 1 ], srcPtr[ 1 ] );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_B8G8R8A8_UNORM:
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadB8G8R8A8_UNorm( texel[ 0 ], srcPtr[ 0 ] );
					ReadB8G8R8A8_UNorm( texel[ 1 ], srcPtr[ 1 ] );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_B8G8R8X8_UNORM:
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadB8G8R8X8_UNorm( texel[ 0 ], srcPtr[ 0 ] );
					ReadB8G8R8X8_UNorm( texel[ 1 ], srcPtr[ 1 ] );
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
					ReadR8G8B8A8_UNorm( texel[ 0 ], srcPtr[ 0 ] );
					ReadR8G8B8A8_UNorm( texel[ 1 ], srcPtr[ 1 ] );
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
					ReadD16_UNorm( texel[ 0 ], srcPtr[ 0 ] );
					ReadD16_UNorm( texel[ 1 ], srcPtr[ 1 ] );
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
					ReadR10G10B10A2_UNorm( texel[ 0 ], srcPtr[ 0 ] );
					ReadR10G10B10A2_UNorm( texel[ 1 ], srcPtr[ 1 ] );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_R11G11B10_FLOAT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR11G11B10_Float( texel[ 0 ], srcPtr[ 0 ] );
					ReadR11G11B10_Float( texel[ 1 ], srcPtr[ 1 ] );
					AddTexel( texel );
				}
			}
			break;
			
		case DXGI_FORMAT_R16G16B16A16_FLOAT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR16G16B16A16_Float( texel[ 0 ], srcPtr[ 0 ] );
					ReadR16G16B16A16_Float( texel[ 1 ], srcPtr[ 1 ] );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR32G32B32A32_Float( texel[ 0 ], srcPtr[ 0 ] );
					ReadR32G32B32A32_Float( texel[ 1 ], srcPtr[ 1 ] );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_R32G32B32A32_UINT:
		case DXGI_FORMAT_R32G32B32A32_TYPELESS:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR32G32B32A32_UInt( texel[ 0 ], srcPtr[ 0 ] );
					ReadR32G32B32A32_UInt( texel[ 1 ], srcPtr[ 1 ] );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_R32G32B32A32_SINT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR32G32B32A32_SInt( texel[ 0 ], srcPtr[ 0 ] );
					ReadR32G32B32A32_SInt( texel[ 1 ], srcPtr[ 1 ] );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_R32_UINT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR32_UInt( texel[ 0 ], srcPtr[ 0 ] );
					ReadR32_UInt( texel[ 1 ], srcPtr[ 1 ] );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_R32_SINT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR32_SInt( texel[ 0 ], srcPtr[ 0 ] );
					ReadR32_SInt( texel[ 1 ], srcPtr[ 1 ] );
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
					ReadR32_Float( texel[ 0 ], srcPtr[ 0 ] );
					ReadR32_Float( texel[ 1 ], srcPtr[ 1 ] );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_R16_FLOAT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR16_Float( texel[ 0 ], srcPtr[ 0 ] );
					ReadR16_Float( texel[ 1 ], srcPtr[ 1 ] );
					AddTexel( texel );
				}
			}
			break;

		case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		case DXGI_FORMAT_R24G8_TYPELESS:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					ReadR24G8_UInt( texel[ 0 ], srcPtr[ 0 ] );
					ReadR24G8_UInt( texel[ 1 ], srcPtr[ 1 ] );
					AddTexel( texel );
				}
			}
			break;
	}
	
	QImage image0 = QImage( dataU8[ 0 ], m_imageWidth, m_imageHeight, QImage::Format_RGB888 );
	QImage image1 = QImage( dataU8[ 1 ], m_imageWidth, m_imageHeight, QImage::Format_RGB888 );
	QImage image2 = QImage( dataU8[ 2 ], m_imageWidth, m_imageHeight, QImage::Format_RGB888 );

	m_imageLabel0.SetImage( image0 );
	m_imageLabel1.SetImage( image2 );
	m_imageLabel2.SetImage( image1 );

	delete[] dataU8[ 0 ];
	delete[] dataU8[ 1 ];
	delete[] dataU8[ 2 ];

	m_imageLabel0.setFixedSize( m_imageWidth * m_zoom, m_imageHeight * m_zoom );
	m_imageLabel1.setFixedSize( m_imageWidth * m_zoom, m_imageHeight * m_zoom );
	m_imageLabel2.setFixedSize( m_imageWidth * m_zoom, m_imageHeight * m_zoom );

	m_imageLabel0.update();
	m_imageLabel1.update();
	m_imageLabel2.update();
	m_gridLayout.update();
}

void CCompareWindow::UpdateTitle()
{
	m_title = QString( "%1 %2 %3x%4 %5 mip:%6/%7 face:%8/%9 zoom:%10%" )
		.arg( m_fileName[ 0 ] )
		.arg( m_fileName[ 1 ] )
		.arg( m_info[ 0 ].width )
		.arg( m_info[ 1 ].height )
		.arg( m_formatName )
		.arg( m_viewMipMap )
		.arg( m_info[ 1 ].mipLevels )
		.arg( m_viewFace )
		.arg( m_info[ 1 ].arraySize )
		.arg( m_zoom * 100.0f );

	extern CMainWindow* GMainWindow;
	GMainWindow->SetStatusLeft( m_title );
}

bool CCompareWindow::eventFilter( QObject* object, QEvent* event )
{
	if ( event->type() == QEvent::Wheel )
	{
		wheelEvent( (QWheelEvent*) event );
		return true;
	}
	else if ( event->type() == QEvent::MouseMove )
	{
		mouseMoveEvent( (QMouseEvent*) event );
		return true;
	}
	else if ( event->type() == QEvent::MouseButtonPress )
	{
		mousePressEvent( (QMouseEvent*) event );
		return true;
	}
	else if ( event->type() == QEvent::MouseButtonRelease )
	{
		mouseReleaseEvent( (QMouseEvent*) event );
		return true;
	}

	return false;
}

void CCompareWindow::wheelEvent( QWheelEvent* event )
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
		float prevTexelX = ( event->x() + m_scrollBarH.value() - 0.5f )	/ m_zoom;
		float prevTexelY = ( event->y() + m_scrollBarV.value() - 0.5f ) / m_zoom;

		m_zoom = newZoom;
		m_imageLabel0.SetZoom( newZoom );
		m_imageLabel1.SetZoom( newZoom );
		m_imageLabel2.SetZoom( newZoom );
		m_imageLabel0.setFixedSize( m_imageWidth * newZoom, m_imageHeight * newZoom );
		m_imageLabel1.setFixedSize( m_imageWidth * newZoom, m_imageHeight * newZoom );
		m_imageLabel2.setFixedSize( m_imageWidth * newZoom, m_imageHeight * newZoom );

		// scroll in order to hold to the same texel under the cursor after the zoom
		m_scrollBarH.setValue( prevTexelX * m_zoom - event->x() + 0.5f );
		m_scrollBarV.setValue( prevTexelY * m_zoom - event->y() + 0.5f );

		m_scrollArea0.horizontalScrollBar()->setValue( m_scrollBarH.value() );
		m_scrollArea0.verticalScrollBar()->setValue( m_scrollBarV.value() );
		m_scrollArea1.horizontalScrollBar()->setValue( m_scrollBarH.value() );
		m_scrollArea1.verticalScrollBar()->setValue( m_scrollBarV.value() );
		m_scrollArea2.horizontalScrollBar()->setValue( m_scrollBarH.value() );
		m_scrollArea2.verticalScrollBar()->setValue( m_scrollBarV.value() );
		UpdateTitle();
	}

	event->accept();
}

void CCompareWindow::mousePressEvent( QMouseEvent* event )
{
	if ( event->buttons() & Qt::LeftButton )
	{
		m_dragEnabled	= true;
		m_dragStart		= QPoint( m_scrollBarH.value(), m_scrollBarV.value() )  + event->pos();
	}
	
	if ( event->buttons() & Qt::RightButton )
	{
		PickTexel( event->x(), event->y() );
	}
}

void CCompareWindow::mouseReleaseEvent( QMouseEvent* event )
{
	if ( event->buttons() & Qt::LeftButton )
	{
		m_dragEnabled = false;
	}
	
	if ( event->buttons() & Qt::RightButton )
	{
		PickTexel( event->x(), event->y() );
	}
}

void CCompareWindow::mouseMoveEvent( QMouseEvent* event )
{
	if ( event->buttons() & Qt::LeftButton && m_dragEnabled )
	{
		QPoint offset = m_dragStart - event->pos();
		m_scrollBarH.setValue( offset.x() );
		m_scrollBarV.setValue( offset.y() );

		m_scrollArea0.horizontalScrollBar()->setValue( m_scrollBarH.value() );
		m_scrollArea0.verticalScrollBar()->setValue( m_scrollBarV.value() );
		m_scrollArea1.horizontalScrollBar()->setValue( m_scrollBarH.value() );
		m_scrollArea1.verticalScrollBar()->setValue( m_scrollBarV.value() );
		m_scrollArea2.horizontalScrollBar()->setValue( m_scrollBarH.value() );
		m_scrollArea2.verticalScrollBar()->setValue( m_scrollBarV.value() );
	}

	if ( event->buttons() & Qt::RightButton )
	{
		PickTexel( event->x(), event->y() );
	}
}

void CCompareWindow::ScrollBarHRangeChanged( int min, int max )
{
	m_scrollBarH.setRange( m_scrollArea0.horizontalScrollBar()->minimum(), m_scrollArea0.horizontalScrollBar()->maximum() );
	m_scrollBarH.setSingleStep( m_scrollArea0.horizontalScrollBar()->singleStep() );
}

void CCompareWindow::ScrollBarVRangeChanged( int min, int max )
{
	m_scrollBarV.setRange( m_scrollArea0.verticalScrollBar()->minimum(), m_scrollArea0.verticalScrollBar()->maximum() );
	m_scrollBarV.setSingleStep( m_scrollArea0.verticalScrollBar()->singleStep() );
}

void CCompareWindow::PickTexel( unsigned tx, unsigned ty )
{
}