#include "stdafx.h"
#include "imageWindow.h"
#include "mainWindow.h"

inline float ClampF( float v, float minV, float maxV )
{
	return std::min( std::max( v, minV ), maxV );
}

inline void HalfToFloat( float* dst, void const* src, unsigned num )
{
	DirectX::PackedVector::XMConvertHalfToFloatStream( dst, 4, (DirectX::PackedVector::HALF const*) src, 2, num );
}

struct SFormatName
{
	DXGI_FORMAT	m_format;
	QString		m_name;
};

#define MAKE_FMT_DESC( x ) { DXGI_FORMAT_##x, #x }

SFormatName GFormatNameArr[] = 
{
	MAKE_FMT_DESC( UNKNOWN						), 
	MAKE_FMT_DESC( R32G32B32A32_TYPELESS		),
	MAKE_FMT_DESC( R32G32B32A32_FLOAT			),
	MAKE_FMT_DESC( R32G32B32A32_UINT			),
	MAKE_FMT_DESC( R32G32B32A32_SINT			),
	MAKE_FMT_DESC( R32G32B32_TYPELESS			),
	MAKE_FMT_DESC( R32G32B32_FLOAT				),
	MAKE_FMT_DESC( R32G32B32_UINT				),
	MAKE_FMT_DESC( R32G32B32_SINT				),
	MAKE_FMT_DESC( R16G16B16A16_TYPELESS		),
	MAKE_FMT_DESC( R16G16B16A16_FLOAT			),
	MAKE_FMT_DESC( R16G16B16A16_UNORM			),
	MAKE_FMT_DESC( R16G16B16A16_UINT			),
	MAKE_FMT_DESC( R16G16B16A16_SNORM			),
	MAKE_FMT_DESC( R16G16B16A16_SINT			),
	MAKE_FMT_DESC( R32G32_TYPELESS				),
	MAKE_FMT_DESC( R32G32_FLOAT					),
	MAKE_FMT_DESC( R32G32_UINT					),
	MAKE_FMT_DESC( R32G32_SINT					),
	MAKE_FMT_DESC( R32G8X24_TYPELESS			),
	MAKE_FMT_DESC( D32_FLOAT_S8X24_UINT			),
	MAKE_FMT_DESC( R32_FLOAT_X8X24_TYPELESS		),
	MAKE_FMT_DESC( X32_TYPELESS_G8X24_UINT		),
	MAKE_FMT_DESC( R10G10B10A2_TYPELESS			),
	MAKE_FMT_DESC( R10G10B10A2_UNORM			),
	MAKE_FMT_DESC( R10G10B10A2_UINT				),
	MAKE_FMT_DESC( R11G11B10_FLOAT				),
	MAKE_FMT_DESC( R8G8B8A8_TYPELESS			),
	MAKE_FMT_DESC( R8G8B8A8_UNORM				),
	MAKE_FMT_DESC( R8G8B8A8_UNORM_SRGB			),
	MAKE_FMT_DESC( R8G8B8A8_UINT				),
	MAKE_FMT_DESC( R8G8B8A8_SNORM				),
	MAKE_FMT_DESC( R8G8B8A8_SINT				),
	MAKE_FMT_DESC( R16G16_TYPELESS				),
	MAKE_FMT_DESC( R16G16_FLOAT					),
	MAKE_FMT_DESC( R16G16_UNORM					),
	MAKE_FMT_DESC( R16G16_UINT					),
	MAKE_FMT_DESC( R16G16_SNORM					),
	MAKE_FMT_DESC( R16G16_SINT					),
	MAKE_FMT_DESC( R32_TYPELESS					),
	MAKE_FMT_DESC( D32_FLOAT					),
	MAKE_FMT_DESC( R32_FLOAT					),
	MAKE_FMT_DESC( R32_UINT						),
	MAKE_FMT_DESC( R32_SINT						),
	MAKE_FMT_DESC( R24G8_TYPELESS				),
	MAKE_FMT_DESC( D24_UNORM_S8_UINT			),
	MAKE_FMT_DESC( R24_UNORM_X8_TYPELESS		),
	MAKE_FMT_DESC( X24_TYPELESS_G8_UINT			),
	MAKE_FMT_DESC( R8G8_TYPELESS				),
	MAKE_FMT_DESC( R8G8_UNORM					),
	MAKE_FMT_DESC( R8G8_UINT					),
	MAKE_FMT_DESC( R8G8_SNORM					),
	MAKE_FMT_DESC( R8G8_SINT					),
	MAKE_FMT_DESC( R16_TYPELESS					),
	MAKE_FMT_DESC( R16_FLOAT					),
	MAKE_FMT_DESC( D16_UNORM					),
	MAKE_FMT_DESC( R16_UNORM					),
	MAKE_FMT_DESC( R16_UINT						),
	MAKE_FMT_DESC( R16_SNORM					),
	MAKE_FMT_DESC( R16_SINT						),
	MAKE_FMT_DESC( R8_TYPELESS					),
	MAKE_FMT_DESC( R8_UNORM						),
	MAKE_FMT_DESC( R8_UINT						),
	MAKE_FMT_DESC( R8_SNORM						),
	MAKE_FMT_DESC( R8_SINT						),
	MAKE_FMT_DESC( A8_UNORM						),
	MAKE_FMT_DESC( R1_UNORM						),
	MAKE_FMT_DESC( R9G9B9E5_SHAREDEXP			),
	MAKE_FMT_DESC( R8G8_B8G8_UNORM				),
	MAKE_FMT_DESC( G8R8_G8B8_UNORM				),
	MAKE_FMT_DESC( BC1_TYPELESS					),
	MAKE_FMT_DESC( BC1_UNORM					),
	MAKE_FMT_DESC( BC1_UNORM_SRGB				),
	MAKE_FMT_DESC( BC2_TYPELESS					),
	MAKE_FMT_DESC( BC2_UNORM					),
	MAKE_FMT_DESC( BC2_UNORM_SRGB				),
	MAKE_FMT_DESC( BC3_TYPELESS					),
	MAKE_FMT_DESC( BC3_UNORM					),
	MAKE_FMT_DESC( BC3_UNORM_SRGB				),
	MAKE_FMT_DESC( BC4_TYPELESS					),
	MAKE_FMT_DESC( BC4_UNORM					),
	MAKE_FMT_DESC( BC4_SNORM					),
	MAKE_FMT_DESC( BC5_TYPELESS					),
	MAKE_FMT_DESC( BC5_UNORM					),
	MAKE_FMT_DESC( BC5_SNORM					),
	MAKE_FMT_DESC( B5G6R5_UNORM					),
	MAKE_FMT_DESC( B5G5R5A1_UNORM				),
	MAKE_FMT_DESC( B8G8R8A8_UNORM				),
	MAKE_FMT_DESC( B8G8R8X8_UNORM				),
	MAKE_FMT_DESC( R10G10B10_XR_BIAS_A2_UNORM	),
	MAKE_FMT_DESC( B8G8R8A8_TYPELESS			),
	MAKE_FMT_DESC( B8G8R8A8_UNORM_SRGB			),
	MAKE_FMT_DESC( B8G8R8X8_TYPELESS			),
	MAKE_FMT_DESC( B8G8R8X8_UNORM_SRGB			),
	MAKE_FMT_DESC( BC6H_TYPELESS				),
	MAKE_FMT_DESC( BC6H_UF16					),
	MAKE_FMT_DESC( BC6H_SF16					),
	MAKE_FMT_DESC( BC7_TYPELESS					),
	MAKE_FMT_DESC( BC7_UNORM					),
	MAKE_FMT_DESC( BC7_UNORM_SRGB				),
	MAKE_FMT_DESC( FORCE_UINT					),
};

CImageWindow::CImageWindow()
	: m_scratchImage( nullptr )
	, m_zoom( 1.0f )
	, m_viewChannel( EViewChannel::RGB )
	, m_viewFace( 0 )
	, m_viewMipMap( 0 )
	, m_imageWidth( 0 )
	, m_imageHeight( 0 )
	, m_viewMin( 0.0f )
	, m_viewMax( 1.0f )
	, m_viewGamma( 1.0f )
{
	setAttribute( Qt::WA_DeleteOnClose );
    setWidget( &m_imageLabel );
}

bool CImageWindow::LoadFile( QString const& path )
{
	wchar_t pathW[ MAX_PATH ];
	path.toWCharArray( pathW );
	pathW[ path.length() ] = 0;

	DirectX::ScratchImage* scratchImage = new DirectX::ScratchImage;
	DirectX::TexMetadata info;
	HRESULT hr = DirectX::LoadFromDDSFile( pathW, 0, &info, *scratchImage );
	if ( hr != S_OK )
	{
		hr = DirectX::LoadFromTGAFile( pathW, &info, *scratchImage );
	}
	if ( hr != S_OK )
	{
		hr = DirectX::LoadFromWICFile( pathW, 0, &info, *scratchImage );
	}
	if ( hr != S_OK )
	{
		delete scratchImage;
		return false;
	}

	m_formatName = "???";
	for ( unsigned i = 0; i < ARRAYSIZE( GFormatNameArr ); ++i )
	{
		if ( GFormatNameArr[ i ].m_format == info.format )
		{
			m_formatName = GFormatNameArr[ i ].m_name;
			break;
		}
	}

	if ( DirectX::IsCompressed( info.format ) )
	{
		DirectX::ScratchImage* decompressedImage = new DirectX::ScratchImage;
		hr = DirectX::Decompress( scratchImage->GetImages(), scratchImage->GetImageCount(), info, DXGI_FORMAT_UNKNOWN, *decompressedImage );
		assert( hr == S_OK );
		delete scratchImage;
		scratchImage = decompressedImage;
		info = scratchImage->GetMetadata();
	}

	info.arraySize = std::max<size_t>( 1, info.arraySize );
	info.mipLevels = std::max<size_t>( 1, info.mipLevels );

	delete m_scratchImage;
	m_path				= path;
	m_info				= info;
	m_scratchImage		= scratchImage;
	m_fileName			= path.mid( path.lastIndexOf( '/' ) + 1 );
	m_texelSizeInBytes	= DirectX::BitsPerPixel( m_info.format ) / 8;

	( (QMdiSubWindow*) parentWidget() )->setWindowTitle( path );
	UpdateTitle();
	UpdateImage();
	return true;
}

void CImageWindow::Reload()
{
	LoadFile( m_path );
}

void CImageWindow::UpdateTitle() const
{
	QString title = QString( "%1 %2x%3 %4 mip:%5/%6 face:%7/%8 zoom:%9%" )
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
	GMainWindow->SetStatusLeft( title );
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

	DirectX::Image const* img = m_scratchImage->GetImage( m_viewMipMap, m_viewFace, 0 );
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
					float texel[ 4 ];
					texel[ 0 ] = srcPtr[ 0 ] / 255.0f;
					texel[ 1 ] = srcPtr[ 0 ] / 255.0f;
					texel[ 2 ] = srcPtr[ 0 ] / 255.0f;
					texel[ 3 ] = srcPtr[ 0 ] / 255.0f;

					AddTexel( texel );

					srcPtr += 1;
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
					int8_t valXSInt = *( (int8_t*) srcPtr + 0 );
					int8_t valYSInt = *( (int8_t*) srcPtr + 1 );
					float sintMax = INT8_MAX;
					float sintMin = INT8_MIN;
		
					float texel[ 4 ];
					texel[ 0 ] = valXSInt > 0 ? ( valXSInt / sintMax ) : ( valXSInt / sintMax );
					texel[ 1 ] = valYSInt > 0 ? ( valYSInt / sintMin ) : ( valYSInt / sintMin );
					texel[ 2 ] = 0.0f;
					texel[ 3 ] = 0.0f;

					AddTexel( texel );

					srcPtr += 2;
				}
			}
			break;

		case DXGI_FORMAT_B8G8R8A8_UNORM:
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					float texel[ 4 ];
					texel[ 0 ] = srcPtr[ 2 ] / 255.0f;
					texel[ 1 ] = srcPtr[ 1 ] / 255.0f;
					texel[ 2 ] = srcPtr[ 0 ] / 255.0f;
					texel[ 3 ] = srcPtr[ 3 ] / 255.0f;

					AddTexel( texel );

					srcPtr += 4;
				}
			}
			break;

		case DXGI_FORMAT_B8G8R8X8_UNORM:
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					float texel[ 4 ];
					texel[ 0 ] = srcPtr[ 2 ] / 255.0f;
					texel[ 1 ] = srcPtr[ 1 ] / 255.0f;
					texel[ 2 ] = srcPtr[ 0 ] / 255.0f;
					texel[ 3 ] = 1.0f;

					AddTexel( texel );

					srcPtr += 4;
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
					float texel[ 4 ];
					texel[ 0 ] = srcPtr[ 0 ] / 255.0f;
					texel[ 1 ] = srcPtr[ 1 ] / 255.0f;
					texel[ 2 ] = srcPtr[ 2 ] / 255.0f;
					texel[ 3 ] = srcPtr[ 3 ] / 255.0f;

					AddTexel( texel );

					srcPtr += 4;
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
					uint16_t val;
					memcpy( &val, srcPtr, sizeof( val ) );

					float texel[ 4 ];
					texel[ 0 ] = val / float( UINT16_MAX );
					texel[ 1 ] = val / float( UINT16_MAX );
					texel[ 2 ] = val / float( UINT16_MAX );
					texel[ 3 ] = val / float( UINT16_MAX );

					AddTexel( texel );

					srcPtr += 2;
				}
			}
			break;
			
		case DXGI_FORMAT_R10G10B10A2_TYPELESS:
		case DXGI_FORMAT_R10G10B10A2_UINT:
		case DXGI_FORMAT_R10G10B10A2_UNORM:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					unsigned val;
					memcpy( &val, srcPtr, sizeof( val ) );
					unsigned const valRUint = val & 0x000003FF;
					unsigned const valGUint = ( val >> 10 ) & 0x000003FF;
					unsigned const valBUint = ( val >> 20 ) & 0x000003FF;
					unsigned const valAUint = val >> 30;

					float texel[ 4 ];
					texel[ 0 ] = valRUint / 1023.0f;
					texel[ 1 ] = valGUint / 1023.0f;
					texel[ 2 ] = valBUint / 1023.0f;
					texel[ 3 ] = valAUint / 3.0f;

					AddTexel( texel );

					srcPtr += 4;
				}
			}
			break;

		case DXGI_FORMAT_R11G11B10_FLOAT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					float texel[ 4 ];
					DirectX::PackedVector::XMFLOAT3PK const tmp3PK( *( (uint32_t*) srcPtr ) );
					DirectX::XMVECTOR tmpV = DirectX::PackedVector::XMLoadFloat3PK( &tmp3PK );
					DirectX::XMStoreFloat4( (DirectX::XMFLOAT4*) texel, tmpV );

					AddTexel( texel );

					srcPtr += 4;
				}
			}
			break;
			
		case DXGI_FORMAT_R16G16B16A16_FLOAT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					float texel[ 4 ];
					HalfToFloat( texel, srcPtr, 4 );

					AddTexel( texel );

					srcPtr += 8;
				}
			}
			break;

		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					float texel[ 4 ];
					memcpy( texel, srcPtr, 4 * sizeof( float ) );

					AddTexel( texel );

					srcPtr += 16;
				}
			}
			break;

		case DXGI_FORMAT_R32G32B32A32_UINT:
		case DXGI_FORMAT_R32G32B32A32_TYPELESS:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					float texel[ 4 ];
					texel[ 0 ] = *( (uint32_t*) srcPtr + 0 );
					texel[ 1 ] = *( (uint32_t*) srcPtr + 1 );
					texel[ 2 ] = *( (uint32_t*) srcPtr + 2 );
					texel[ 3 ] = *( (uint32_t*) srcPtr + 3 );

					AddTexel( texel );

					srcPtr += 16;
				}
			}
			break;

		case DXGI_FORMAT_R32G32B32A32_SINT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					float texel[ 4 ];
					texel[ 0 ] = *( (int32_t*) srcPtr + 0 );
					texel[ 1 ] = *( (int32_t*) srcPtr + 1 );
					texel[ 2 ] = *( (int32_t*) srcPtr + 2 );
					texel[ 3 ] = *( (int32_t*) srcPtr + 3 );

					AddTexel( texel );

					srcPtr += 16;
				}
			}
			break;

		case DXGI_FORMAT_R32_UINT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					float texel[ 4 ];
					texel[ 0 ] = *( (uint32_t*) srcPtr );
					texel[ 1 ] = *( (uint32_t*) srcPtr );
					texel[ 2 ] = *( (uint32_t*) srcPtr );
					texel[ 3 ] = *( (uint32_t*) srcPtr );

					AddTexel( texel );

					srcPtr += 4;
				}
			}
			break;

		case DXGI_FORMAT_R32_SINT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					float texel[ 4 ];
					texel[ 0 ] = *( (int32_t*) srcPtr );
					texel[ 1 ] = *( (int32_t*) srcPtr );
					texel[ 2 ] = *( (int32_t*) srcPtr );
					texel[ 3 ] = *( (int32_t*) srcPtr );

					AddTexel( texel );

					srcPtr += 4;
				}
			}
			break;

		case DXGI_FORMAT_D32_FLOAT:
		case DXGI_FORMAT_R32_FLOAT:
		case DXGI_FORMAT_R32_TYPELESS:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					float texel[ 4 ];
					memcpy( texel, srcPtr, sizeof( float ) );
					texel[ 1 ] = texel[ 0 ];
					texel[ 2 ] = texel[ 0 ];
					texel[ 3 ] = texel[ 0 ];

					AddTexel( texel );

					srcPtr += 4;
				}
			}
			break;

		case DXGI_FORMAT_R16_FLOAT:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					float texel[ 4 ];
					HalfToFloat( texel, srcPtr, 1 );
					texel[ 1 ] = texel[ 0 ];
					texel[ 2 ] = texel[ 0 ];
					texel[ 3 ] = texel[ 0 ];

					AddTexel( texel );

					srcPtr += 2;
				}
			}
			break;

		case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
		case DXGI_FORMAT_R24G8_TYPELESS:
			{
				for ( unsigned iTexel = 0; iTexel < m_imageWidth * m_imageHeight; ++iTexel )
				{
					unsigned const valRUint	= srcPtr[ 0 ] + ( srcPtr[ 1 ] << 8 ) + ( srcPtr[ 2 ] << 16 );

					float texel[ 4 ];
					texel[ 0 ] = valRUint / 16777216.0f;
					texel[ 1 ] = srcPtr[ 3 ] / 255.0f;
					texel[ 2 ] = 0.0f;
					texel[ 3 ] = 0.0f;

					AddTexel( texel );

					srcPtr += 4;
				}
			}
			break;
	}

	QImage image = QImage( dataU8, m_imageWidth, m_imageHeight, QImage::Format_RGB888 );
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
		PickTexel( event->x(), event->y() );
	}
}

void CImageWindow::mouseReleaseEvent( QMouseEvent* event )
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
		PickTexel( event->x(), event->y() );
	}
}

void CImageWindow::PickTexel( unsigned tx, unsigned ty )
{
	unsigned texelX = ClampF( ( tx + horizontalScrollBar()->value() - 0.5f ) / m_zoom, 0.0f, m_imageWidth  - 1.0f );
	unsigned texelY = ClampF( ( ty + verticalScrollBar()->value() - 0.5f )	 / m_zoom, 0.0f, m_imageHeight - 1.0f );

	QString texelInfo;
	if ( texelX >= 0 && texelX < m_imageWidth && texelY >= 0 && texelY < m_imageHeight )
	{
		DirectX::Image const* img = m_scratchImage->GetImage( m_viewMipMap, m_viewFace, 0 );
		uint8_t* srcPtr = img->pixels + m_texelSizeInBytes * ( texelX + texelY * m_imageWidth );

		switch ( m_info.format )
		{
			case DXGI_FORMAT_R8_TYPELESS:
			case DXGI_FORMAT_R8_UNORM:
			case DXGI_FORMAT_R8_UINT:
			case DXGI_FORMAT_R8_SNORM:
			case DXGI_FORMAT_R8_SINT:
				{
					unsigned const valUInt	= srcPtr[ 3 ];
					float const valFloat	= valUInt / 255.0f;
					texelInfo = QString( "R:%1(%2)" ).arg( valUInt ).arg( valFloat );
				}
				break;

			case DXGI_FORMAT_R8G8_SNORM:
			case DXGI_FORMAT_R8G8_UNORM:
			case DXGI_FORMAT_R8G8_UINT:
			case DXGI_FORMAT_R8G8_SINT:
			case DXGI_FORMAT_R8G8_TYPELESS:
				{
					int8_t valXSInt = *( (int8_t*) srcPtr + 0 );
					int8_t valYSInt = *( (int8_t*) srcPtr + 1 );
				
					float sintMax = +INT8_MAX;
					float sintMin = -INT8_MIN;

					float valXFloat = valXSInt > 0 ? ( valXSInt / sintMax ) : ( valXSInt / sintMax );
					float valYFloat = valYSInt > 0 ? ( valYSInt / sintMin ) : ( valYSInt / sintMin );
					texelInfo = QString( "R:%1(%2) G:%3(%4)" ).arg( valXSInt ).arg( valXFloat ).arg( valYSInt ).arg( valYFloat );
				}
				break;

			case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
			case DXGI_FORMAT_R24G8_TYPELESS:
				{
					unsigned const valRUint		= srcPtr[ 0 ] + ( srcPtr[ 1 ] << 8 ) + ( srcPtr[ 2 ] << 16 );
					unsigned const valGUint		= srcPtr[ 3 ];
					float    const valRFloat	= valRUint / 16777216.0f;
					float    const valGFloat	= valGUint / 255.0f;
					texelInfo = QString( "R:%1(%2) G:%3(%4)" ).arg( valRUint ).arg( valRFloat ).arg( valGUint ).arg( valGFloat );
				}
				break;

			case DXGI_FORMAT_B8G8R8X8_UNORM:
			case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
				{
					unsigned const valRUint		= srcPtr[ 2 ];
					unsigned const valGUint		= srcPtr[ 1 ];
					unsigned const valBUint		= srcPtr[ 0 ];
					float    const valRFloat	= valRUint / 255.0f;
					float    const valGFloat	= valGUint / 255.0f;
					float    const valBFloat	= valBUint / 255.0f;
					texelInfo = QString( "R:%1(%2) G:%3(%4) B:%5(%6)" ).arg( valRUint ).arg( valRFloat ).arg( valGUint ).arg( valGFloat ).arg( valBUint ).arg( valBFloat );
				}
				break;

			case DXGI_FORMAT_B8G8R8A8_UNORM:
			case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
				{
					unsigned const valRUint		= srcPtr[ 2 ];
					unsigned const valGUint		= srcPtr[ 1 ];
					unsigned const valBUint		= srcPtr[ 0 ];
					unsigned const valAUint		= srcPtr[ 3 ];
					float    const valRFloat	= valRUint / 255.0f;
					float    const valGFloat	= valGUint / 255.0f;
					float    const valBFloat	= valBUint / 255.0f;
					float    const valAFloat	= valAUint / 255.0f;
					texelInfo = QString( "R:%1(%2) G:%3(%4) B:%5(%6) A:%7(%8)" ).arg( valRUint ).arg( valRFloat ).arg( valGUint ).arg( valGFloat ).arg( valBUint ).arg( valBFloat ).arg( valAUint ).arg( valAFloat );
				}
				break;

			case DXGI_FORMAT_R10G10B10A2_TYPELESS:
			case DXGI_FORMAT_R10G10B10A2_UINT:
			case DXGI_FORMAT_R10G10B10A2_UNORM:
				{
					unsigned texel;
					memcpy( &texel, srcPtr, sizeof( texel ) );
					unsigned const valRUint		= texel & 0x000003FF;
					unsigned const valGUint		= ( texel >> 10 ) & 0x000003FF;
					unsigned const valBUint		= ( texel >> 20 ) & 0x000003FF;
					unsigned const valAUint		= texel >> 30;
					float    const valRFloat	= valRUint / 1023.0f;
					float    const valGFloat	= valGUint / 1023.0f;
					float    const valBFloat	= valBUint / 1023.0f;
					float    const valAFloat	= valAUint / 3.0f;
					texelInfo = QString( "R:%1(%2) G:%3(%4) B:%5(%6) A:%7(%8)" ).arg( valRUint ).arg( valRFloat ).arg( valGUint ).arg( valGFloat ).arg( valBUint ).arg( valBFloat ).arg( valAUint ).arg( valAFloat );
				}
				break;

			case DXGI_FORMAT_R8G8B8A8_TYPELESS:
			case DXGI_FORMAT_R8G8B8A8_UINT:
			case DXGI_FORMAT_R8G8B8A8_UNORM:
			case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
				{
					unsigned const valRUint		= srcPtr[ 0 ];
					unsigned const valGUint		= srcPtr[ 1 ];
					unsigned const valBUint		= srcPtr[ 2 ];
					unsigned const valAUint		= srcPtr[ 3 ];
					float    const valRFloat	= valRUint / 255.0f;
					float    const valGFloat	= valGUint / 255.0f;
					float    const valBFloat	= valBUint / 255.0f;
					float    const valAFloat	= valAUint / 255.0f;
					texelInfo = QString( "R:%1(%2) G:%3(%4) B:%5(%6) A:%7(%8)" ).arg( valRUint ).arg( valRFloat ).arg( valGUint ).arg( valGFloat ).arg( valBUint ).arg( valBFloat ).arg( valAUint ).arg( valAFloat );
				}
				break;

			case DXGI_FORMAT_R11G11B10_FLOAT:
				{
					DirectX::XMFLOAT4 val;

					DirectX::PackedVector::XMFLOAT3PK const tmp3PK( *( (uint32_t*) srcPtr ) );
					DirectX::XMVECTOR tmpV = DirectX::PackedVector::XMLoadFloat3PK( &tmp3PK );
					DirectX::XMStoreFloat4( &val, tmpV );

					texelInfo = QString( "R:%1 G:%2 B:%3" ).arg( val.x ).arg( val.y ).arg( val.z );
				}
				break;

			case DXGI_FORMAT_R16G16B16A16_FLOAT:
				{
					float val[ 4 ];
					HalfToFloat( val, srcPtr, ARRAYSIZE( val ) );
					texelInfo = QString( "R:%1 G:%2 B:%3 A:%4" ).arg( val[ 0 ] ).arg( val[ 1 ] ).arg( val[ 2 ] ).arg( val[ 3 ] );
				}
				break;

			case DXGI_FORMAT_R32G32B32A32_FLOAT:
				{
					float val[ 4 ];
					memcpy( &val, srcPtr, sizeof( val ) );
					texelInfo = QString( "R:%1 G:%2 B:%3 A:%4" ).arg( val[ 0 ] ).arg( val[ 1 ] ).arg( val[ 2 ] ).arg( val[ 3 ] );
				}
				break;

			case DXGI_FORMAT_R32G32B32A32_UINT:
			case DXGI_FORMAT_R32G32B32A32_TYPELESS:
				{
					uint32_t val[ 4 ];
					val[ 0 ] = *( (uint32_t*) srcPtr + 0 );
					val[ 1 ] = *( (uint32_t*) srcPtr + 1 );
					val[ 2 ] = *( (uint32_t*) srcPtr + 2 );
					val[ 3 ] = *( (uint32_t*) srcPtr + 3 );

					texelInfo = QString( "R:%1(0x%2) G:%3(0x%4) B:%5(0x%6) A:%7(0x%8)" ).arg( val[ 0 ] ).arg( val[ 0 ], 0, 16 ).arg( val[ 1 ] ).arg( val[ 1 ], 0, 16 ).arg( val[ 2 ] ).arg( val[ 2 ], 0, 16 ).arg( val[ 3 ] ).arg( val[ 3 ], 0, 16 );
				}
				break;

			case DXGI_FORMAT_R32G32B32A32_SINT:
				{
					int32_t val[ 4 ];
					val[ 0 ] = *( (int32_t*) srcPtr + 0 );
					val[ 1 ] = *( (int32_t*) srcPtr + 1 );
					val[ 2 ] = *( (int32_t*) srcPtr + 2 );
					val[ 3 ] = *( (int32_t*) srcPtr + 3 );

					texelInfo = QString( "R:%1(0x%2) G:%3(0x%4) B:%5(0x%6) A:%7(0x%8)" ).arg( val[ 0 ] ).arg( val[ 0 ], 0, 16 ).arg( val[ 1 ] ).arg( val[ 1 ], 0, 16 ).arg( val[ 2 ] ).arg( val[ 2 ], 0, 16 ).arg( val[ 3 ] ).arg( val[ 3 ], 0, 16 );
				}
				break;

			case DXGI_FORMAT_R32_UINT:
				{
					uint32_t val = *( (uint32_t*) srcPtr );
					texelInfo = QString( "R:%1(0x%2)" ).arg( val ).arg( val, 0, 16 );
				}
				break;

			case DXGI_FORMAT_R32_SINT:
				{
					int32_t val = *( (int32_t*) srcPtr );
					texelInfo = QString( "R:%1(0x%2)" ).arg( val ).arg( val, 0, 16 );
				}
				break;

			case DXGI_FORMAT_D32_FLOAT:
			case DXGI_FORMAT_R32_FLOAT:
			case DXGI_FORMAT_R32_TYPELESS:
				{
					float val;
					memcpy( &val, srcPtr, sizeof( float ) );
					texelInfo = QString( "R:%1" ).arg( val );
				}
				break;

			case DXGI_FORMAT_R16_FLOAT:
				{
					float val;
					HalfToFloat( &val, srcPtr, 1 );
					texelInfo = QString( "R:%1" ).arg( val );
				}
				break;

			case DXGI_FORMAT_D16_UNORM:
			case DXGI_FORMAT_R16_UINT:
			case DXGI_FORMAT_R16_TYPELESS:
			case DXGI_FORMAT_R16_SNORM:
			case DXGI_FORMAT_R16_SINT:
				{
					uint16_t	valU16;
					float		valF;
					memcpy( &valU16, srcPtr, sizeof( valU16 ) );
					valF = valU16 / float( UINT16_MAX );
					texelInfo = QString( "R:%1(%2)" ).arg( valF ).arg( valU16 );
				}
				break;
		}
	}

	QString texelDesc = QString( "%1 %2 %3" ).arg( texelX ).arg( texelY ).arg( texelInfo );
	extern CMainWindow* GMainWindow;
	GMainWindow->SetStatusRight( texelDesc );
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