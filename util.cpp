#include "stdafx.h"
#include "util.h"

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

bool UtilLoadFile( DirectX::ScratchImage& scratchImage, DirectX::TexMetadata& info, QString& formatName, QString& fileName, unsigned& texelSizeInBytes, QString const& path )
{
	wchar_t pathW[ MAX_PATH ];
	path.toWCharArray( pathW );
	pathW[ path.length() ] = 0;
	
	HRESULT hr = DirectX::LoadFromDDSFile( pathW, 0, &info, scratchImage );
	if ( hr != S_OK )
	{
		hr = DirectX::LoadFromTGAFile( pathW, &info, scratchImage );
	}
	if ( hr != S_OK )
	{
		hr = DirectX::LoadFromWICFile( pathW, 0, &info, scratchImage );
	}
	if ( hr != S_OK )
	{
		hr = DirectX::LoadFromHDRFile( pathW, &info, scratchImage );
	}
	if ( hr != S_OK )
	{
		FIBITMAP* dib = FreeImage_Load( FIF_HDR, path.toLatin1(), HDR_DEFAULT );
		if ( !dib )
		{
			dib = FreeImage_Load( FIF_EXR, path.toLatin1(), EXR_DEFAULT );
		}

		if ( dib )
		{
			info.arraySize	= 1;
			info.depth		= 1;
			info.dimension	= DirectX::TEX_DIMENSION_TEXTURE2D;
			info.format		= DXGI_FORMAT_UNKNOWN;
			info.width		= FreeImage_GetWidth( dib );
			info.height		= FreeImage_GetHeight( dib );
			info.mipLevels	= 1;

			unsigned const bpp = FreeImage_GetBPP( dib );
			if ( bpp == 128 )
			{
				info.format	= DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
			else if ( bpp == 96 )
			{
				info.format	= DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if ( bpp == 64 )
			{
				info.format	= DXGI_FORMAT_R16G16B16A16_FLOAT;
			}

			if ( info.format != DXGI_FORMAT_UNKNOWN )
			{
				FreeImage_FlipVertical( dib );
				scratchImage.Initialize2D( info.format, info.width, info.height, 1, 1, 0 );

				uint8_t* srcPixels = FreeImage_GetBits( dib );
				texelSizeInBytes = DirectX::BitsPerPixel( info.format ) / 8;
				memcpy( scratchImage.GetPixels(), srcPixels, info.width * info.height * texelSizeInBytes );

				hr = S_OK;
			}
			FreeImage_Unload( dib );
		}
	}

	if ( hr != S_OK )
	{
		QMessageBox::critical( nullptr, "Error", "Can't load image: " + path );
		return false;
	}

	formatName = "???";
	for ( unsigned i = 0; i < ARRAYSIZE( GFormatNameArr ); ++i )
	{
		if ( GFormatNameArr[ i ].m_format == info.format )
		{
			formatName = GFormatNameArr[ i ].m_name;
			break;
		}
	}

	if ( DirectX::IsCompressed( info.format ) )
	{
		DirectX::ScratchImage decompressedImage;
		hr = DirectX::Decompress( scratchImage.GetImages(), scratchImage.GetImageCount(), info, DXGI_FORMAT_UNKNOWN, decompressedImage );
		assert( hr == S_OK );
		scratchImage = std::move( decompressedImage );
		info = scratchImage.GetMetadata();
	}

	info.arraySize = std::max<size_t>( 1, info.arraySize );
	info.mipLevels = std::max<size_t>( 1, info.mipLevels );

	fileName			= path.mid( path.lastIndexOf( '/' ) + 1 );
	texelSizeInBytes	= DirectX::BitsPerPixel( info.format ) / 8;
	return true;
}