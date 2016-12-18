#pragma once

bool UtilLoadFile( DirectX::ScratchImage& scratchImage, DirectX::TexMetadata& info, QString& formatName, QString& fileName, unsigned& texelSizeInBytes, QString const& path );

inline float ClampF( float v, float minV, float maxV )
{
	return std::min( std::max( v, minV ), maxV );
}

inline void HalfToFloat( float* dst, void const* src, unsigned num )
{
	DirectX::PackedVector::XMConvertHalfToFloatStream( dst, 4, (DirectX::PackedVector::HALF const*) src, 2, num );
}

inline void ReadR8_UNorm( float texel[ 4 ], uint8_t *& srcPtr )
{
	texel[ 0 ] = srcPtr[ 0 ] / 255.0f;
	texel[ 1 ] = srcPtr[ 0 ] / 255.0f;
	texel[ 2 ] = srcPtr[ 0 ] / 255.0f;
	texel[ 3 ] = srcPtr[ 0 ] / 255.0f;
	srcPtr += 1;
}

inline void ReadR8G8_UNorm( float texel[ 4 ], uint8_t *& srcPtr )
{
	int8_t valXSInt = *( (int8_t*) srcPtr + 0 );
	int8_t valYSInt = *( (int8_t*) srcPtr + 1 );
	float sintMax = INT8_MAX;
	float sintMin = INT8_MIN;

	texel[ 0 ] = valXSInt > 0 ? ( valXSInt / sintMax ) : ( valXSInt / sintMax );
	texel[ 1 ] = valYSInt > 0 ? ( valYSInt / sintMin ) : ( valYSInt / sintMin );
	texel[ 2 ] = 0.0f;
	texel[ 3 ] = 0.0f;
	srcPtr += 2;
}

inline void ReadB8G8R8A8_UNorm( float texel[ 4 ], uint8_t *& srcPtr )
{
	texel[ 0 ] = srcPtr[ 2 ] / 255.0f;
	texel[ 1 ] = srcPtr[ 1 ] / 255.0f;
	texel[ 2 ] = srcPtr[ 0 ] / 255.0f;
	texel[ 3 ] = srcPtr[ 3 ] / 255.0f;
	srcPtr += 4;	
}

inline void ReadB8G8R8X8_UNorm( float texel[ 4 ], uint8_t *& srcPtr )
{
	texel[ 0 ] = srcPtr[ 2 ] / 255.0f;
	texel[ 1 ] = srcPtr[ 1 ] / 255.0f;
	texel[ 2 ] = srcPtr[ 0 ] / 255.0f;
	texel[ 3 ] = 1.0f;
	srcPtr += 4;	
}

inline void ReadR8G8B8A8_UNorm( float texel[ 4 ], uint8_t *& srcPtr )
{
	texel[ 0 ] = srcPtr[ 0 ] / 255.0f;
	texel[ 1 ] = srcPtr[ 1 ] / 255.0f;
	texel[ 2 ] = srcPtr[ 2 ] / 255.0f;
	texel[ 3 ] = srcPtr[ 3 ] / 255.0f;
	srcPtr += 4;
}

inline void ReadD16_UNorm( float texel[ 4 ], uint8_t *& srcPtr )
{
	uint16_t val;
	memcpy( &val, srcPtr, sizeof( val ) );

	texel[ 0 ] = val / float( UINT16_MAX );
	texel[ 1 ] = val / float( UINT16_MAX );
	texel[ 2 ] = val / float( UINT16_MAX );
	texel[ 3 ] = val / float( UINT16_MAX );

	srcPtr += 2;
}

inline void ReadR10G10B10A2_UNorm( float texel[ 4 ], uint8_t *& srcPtr )
{
	unsigned val;
	memcpy( &val, srcPtr, sizeof( val ) );
	unsigned const valRUint = val & 0x000003FF;
	unsigned const valGUint = ( val >> 10 ) & 0x000003FF;
	unsigned const valBUint = ( val >> 20 ) & 0x000003FF;
	unsigned const valAUint = val >> 30;

	texel[ 0 ] = valRUint / 1023.0f;
	texel[ 1 ] = valGUint / 1023.0f;
	texel[ 2 ] = valBUint / 1023.0f;
	texel[ 3 ] = valAUint / 3.0f;

	srcPtr += 4;
}

inline void ReadR11G11B10_Float( float texel[ 4 ], uint8_t *& srcPtr )
{
	DirectX::PackedVector::XMFLOAT3PK const tmp3PK( *( (uint32_t*) srcPtr ) );
	DirectX::XMVECTOR tmpV = DirectX::PackedVector::XMLoadFloat3PK( &tmp3PK );
	DirectX::XMStoreFloat4( (DirectX::XMFLOAT4*) texel, tmpV );

	srcPtr += 4;
}

inline void ReadR16G16B16A16_Float( float texel[ 4 ], uint8_t *& srcPtr )
{
	HalfToFloat( texel, srcPtr, 4 );
	srcPtr += 8;
}

inline void ReadR32G32B32_Float( float texel[ 4 ], uint8_t *& srcPtr )
{
	memcpy( texel, srcPtr, 3 * sizeof( float ) );
	texel[ 3 ] = 1.0f;
	srcPtr += 12;
}

inline void ReadR32G32B32A32_Float( float texel[ 4 ], uint8_t *& srcPtr )
{
	memcpy( texel, srcPtr, 4 * sizeof( float ) );
	srcPtr += 16;
}

inline void ReadR32G32B32A32_UInt( float texel[ 4 ], uint8_t *& srcPtr )
{
	texel[ 0 ] = *( (uint32_t*) srcPtr + 0 );
	texel[ 1 ] = *( (uint32_t*) srcPtr + 1 );
	texel[ 2 ] = *( (uint32_t*) srcPtr + 2 );
	texel[ 3 ] = *( (uint32_t*) srcPtr + 3 );

	srcPtr += 16;
}

inline void ReadR32G32B32A32_SInt( float texel[ 4 ], uint8_t *& srcPtr )
{
	texel[ 0 ] = *( (int32_t*) srcPtr + 0 );
	texel[ 1 ] = *( (int32_t*) srcPtr + 1 );
	texel[ 2 ] = *( (int32_t*) srcPtr + 2 );
	texel[ 3 ] = *( (int32_t*) srcPtr + 3 );

	srcPtr += 16;
}

inline void ReadR32_UInt( float texel[ 4 ], uint8_t *& srcPtr )
{
	texel[ 0 ] = *( (uint32_t*) srcPtr );
	texel[ 1 ] = *( (uint32_t*) srcPtr );
	texel[ 2 ] = *( (uint32_t*) srcPtr );
	texel[ 3 ] = *( (uint32_t*) srcPtr );

	srcPtr += 4;
}

inline void ReadR32_SInt( float texel[ 4 ], uint8_t *& srcPtr )
{
	texel[ 0 ] = *( (uint32_t*) srcPtr );
	texel[ 1 ] = *( (uint32_t*) srcPtr );
	texel[ 2 ] = *( (uint32_t*) srcPtr );
	texel[ 3 ] = *( (uint32_t*) srcPtr );

	srcPtr += 4;
}

inline void ReadR32_Float( float texel[ 4 ], uint8_t *& srcPtr )
{
	memcpy( texel, srcPtr, sizeof( float ) );
	texel[ 1 ] = texel[ 0 ];
	texel[ 2 ] = texel[ 0 ];
	texel[ 3 ] = texel[ 0 ];

	srcPtr += 4;
}

inline void ReadR16_Float( float texel[ 4 ], uint8_t *& srcPtr )
{
	HalfToFloat( texel, srcPtr, 1 );
	texel[ 1 ] = texel[ 0 ];
	texel[ 2 ] = texel[ 0 ];
	texel[ 3 ] = texel[ 0 ];

	srcPtr += 2;
}

inline void ReadR24G8_UInt( float texel[ 4 ], uint8_t *& srcPtr )
{
	unsigned const valRUint	= srcPtr[ 0 ] + ( srcPtr[ 1 ] << 8 ) + ( srcPtr[ 2 ] << 16 );

	texel[ 0 ] = valRUint / 16777216.0f;
	texel[ 1 ] = srcPtr[ 3 ] / 255.0f;
	texel[ 2 ] = 0.0f;
	texel[ 3 ] = 0.0f;

	srcPtr += 4;
}

inline void ReadR32S8( float texel[ 4 ], uint8_t *& srcPtr )
{
	float d32f;
	uint8_t stencil;
	memcpy( &d32f,		srcPtr + 0,	sizeof( float ) );
	memcpy( &stencil,	srcPtr + 4, sizeof( uint8_t ) );

	texel[ 0 ] = d32f;
	texel[ 1 ] = stencil / 255.0f;
	texel[ 2 ] = 0.0f;
	texel[ 3 ] = 0.0f;

	srcPtr += 8;
}

inline void TexelInfoR8_UNorm( QString info[ 4 ], uint8_t const* srcPtr )
{
	unsigned const valUInt	= srcPtr[ 3 ];
	float const valFloat	= valUInt / 255.0f;
	info[ 0 ] = QString( "%1(%2)" ).arg( valUInt ).arg( valFloat );
}

inline void TexelInfoR8G8_UNorm( QString info[ 4 ], uint8_t const* srcPtr )
{
	int8_t valXSInt = *( (int8_t*) srcPtr + 0 );
	int8_t valYSInt = *( (int8_t*) srcPtr + 1 );
				
	float sintMax = +INT8_MAX;
	float sintMin = -INT8_MIN;

	float valXFloat = valXSInt > 0 ? ( valXSInt / sintMax ) : ( valXSInt / sintMax );
	float valYFloat = valYSInt > 0 ? ( valYSInt / sintMin ) : ( valYSInt / sintMin );
	info[ 0 ] = QString( "%1(%2)" ).arg( valXSInt ).arg( valXFloat );
	info[ 1 ] = QString( "%1(%2)" ).arg( valYSInt ).arg( valYFloat );
}

inline void TexelInfoB8G8R8A8_UNorm( QString info[ 4 ], uint8_t const* srcPtr )
{
	unsigned const valRUint		= srcPtr[ 2 ];
	unsigned const valGUint		= srcPtr[ 1 ];
	unsigned const valBUint		= srcPtr[ 0 ];
	unsigned const valAUint		= srcPtr[ 3 ];
	float    const valRFloat	= valRUint / 255.0f;
	float    const valGFloat	= valGUint / 255.0f;
	float    const valBFloat	= valBUint / 255.0f;
	float    const valAFloat	= valAUint / 255.0f;

	info[ 0 ] = QString( "%1(%2)" ).arg( valRUint ).arg( valRFloat );
	info[ 1 ] = QString( "%1(%2)" ).arg( valGUint ).arg( valGFloat );
	info[ 2 ] = QString( "%1(%2)" ).arg( valBUint ).arg( valBFloat );
	info[ 3 ] = QString( "%1(%2)" ).arg( valAUint ).arg( valAFloat );
}

inline void TexelInfoB8G8R8X8_UNorm( QString info[ 4 ], uint8_t const* srcPtr )
{
	unsigned const valRUint		= srcPtr[ 2 ];
	unsigned const valGUint		= srcPtr[ 1 ];
	unsigned const valBUint		= srcPtr[ 0 ];
	float    const valRFloat	= valRUint / 255.0f;
	float    const valGFloat	= valGUint / 255.0f;
	float    const valBFloat	= valBUint / 255.0f;

	info[ 0 ] = QString( "%1(%2)" ).arg( valRUint ).arg( valRFloat );
	info[ 1 ] = QString( "%1(%2)" ).arg( valGUint ).arg( valGFloat );
	info[ 2 ] = QString( "%1(%2)" ).arg( valBUint ).arg( valBFloat );
}

inline void TexelInfoR8G8B8A8_UNorm( QString info[ 4 ], uint8_t const* srcPtr )
{
	unsigned const valRUint		= srcPtr[ 0 ];
	unsigned const valGUint		= srcPtr[ 1 ];
	unsigned const valBUint		= srcPtr[ 2 ];
	unsigned const valAUint		= srcPtr[ 3 ];
	float    const valRFloat	= valRUint / 255.0f;
	float    const valGFloat	= valGUint / 255.0f;
	float    const valBFloat	= valBUint / 255.0f;
	float    const valAFloat	= valAUint / 255.0f;

	info[ 0 ] = QString( "%1(%2)" ).arg( valRUint ).arg( valRFloat );
	info[ 1 ] = QString( "%1(%2)" ).arg( valGUint ).arg( valGFloat );
	info[ 2 ] = QString( "%1(%2)" ).arg( valBUint ).arg( valBFloat );
	info[ 3 ] = QString( "%1(%2)" ).arg( valAUint ).arg( valAFloat );
}

inline void TexelInfoD16_UNorm( QString info[ 4 ], uint8_t const* srcPtr )
{
	uint16_t valU16;
	float valF;
	memcpy( &valU16, srcPtr, sizeof( valU16 ) );
	valF = valU16 / float( UINT16_MAX );

	info[ 0 ] = QString( "%1(%2)" ).arg( valF ).arg( valU16 );
}

inline void TexelInfoR10G10B10A2_UNorm( QString info[ 4 ], uint8_t const* srcPtr )
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

	info[ 0 ] = QString( "%1(%2)" ).arg( valRUint ).arg( valRFloat );
	info[ 1 ] = QString( "%1(%2)" ).arg( valGUint ).arg( valGFloat );
	info[ 2 ] = QString( "%1(%2)" ).arg( valBUint ).arg( valBFloat );
	info[ 3 ] = QString( "%1(%2)" ).arg( valAUint ).arg( valAFloat );
}

inline void TexelInfoR11G11B10_Float( QString info[ 4 ], uint8_t const* srcPtr )
{
	DirectX::XMFLOAT4 val;

	DirectX::PackedVector::XMFLOAT3PK const tmp3PK( *( (uint32_t*) srcPtr ) );
	DirectX::XMVECTOR tmpV = DirectX::PackedVector::XMLoadFloat3PK( &tmp3PK );
	DirectX::XMStoreFloat4( &val, tmpV );

	info[ 0 ] = QString( "%1" ).arg( val.x );
	info[ 1 ] = QString( "%1" ).arg( val.y );
	info[ 2 ] = QString( "%1" ).arg( val.z );
}

inline void TexelInfoR16G16B16A16_Float( QString info[ 4 ], uint8_t const* srcPtr )
{
	float val[ 4 ];
	HalfToFloat( val, srcPtr, ARRAYSIZE( val ) );

	info[ 0 ] = QString( "%1" ).arg( val[ 0 ] );
	info[ 1 ] = QString( "%1" ).arg( val[ 1 ] );
	info[ 2 ] = QString( "%1" ).arg( val[ 2 ] );
	info[ 3 ] = QString( "%1" ).arg( val[ 3 ] );
}

inline void TexelInfoR32G32B32_Float( QString info[ 4 ], uint8_t const* srcPtr )
{
	float val[ 3 ];
	memcpy( &val, srcPtr, sizeof( val ) );

	info[ 0 ] = QString( "%1" ).arg( val[ 0 ] );
	info[ 1 ] = QString( "%1" ).arg( val[ 1 ] );
	info[ 2 ] = QString( "%1" ).arg( val[ 2 ] );
}

inline void TexelInfoR32G32B32A32_Float( QString info[ 4 ], uint8_t const* srcPtr )
{
	float val[ 4 ];
	memcpy( &val, srcPtr, sizeof( val ) );

	info[ 0 ] = QString( "%1" ).arg( val[ 0 ] );
	info[ 1 ] = QString( "%1" ).arg( val[ 1 ] );
	info[ 2 ] = QString( "%1" ).arg( val[ 2 ] );
	info[ 3 ] = QString( "%1" ).arg( val[ 3 ] );
}

inline void TexelInfoR32G32B32A32_UInt( QString info[ 4 ], uint8_t const* srcPtr )
{
	uint32_t val[ 4 ];
	val[ 0 ] = *( (uint32_t*) srcPtr + 0 );
	val[ 1 ] = *( (uint32_t*) srcPtr + 1 );
	val[ 2 ] = *( (uint32_t*) srcPtr + 2 );
	val[ 3 ] = *( (uint32_t*) srcPtr + 3 );

	info[ 0 ] = QString( "%1(0x%2)" ).arg( val[ 0 ] ).arg( val[ 0 ], 0, 16 );
	info[ 1 ] = QString( "%1(0x%2)" ).arg( val[ 1 ] ).arg( val[ 1 ], 0, 16 );
	info[ 2 ] = QString( "%1(0x%2)" ).arg( val[ 2 ] ).arg( val[ 2 ], 0, 16 );
	info[ 3 ] = QString( "%1(0x%2)" ).arg( val[ 3 ] ).arg( val[ 3 ], 0, 16 );
}

inline void TexelInfoR32G32B32A32_SInt( QString info[ 4 ], uint8_t const* srcPtr )
{
	int32_t val[ 4 ];
	val[ 0 ] = *( (int32_t*) srcPtr + 0 );
	val[ 1 ] = *( (int32_t*) srcPtr + 1 );
	val[ 2 ] = *( (int32_t*) srcPtr + 2 );
	val[ 3 ] = *( (int32_t*) srcPtr + 3 );

	info[ 0 ] = QString( "%1(0x%2)" ).arg( val[ 0 ] ).arg( val[ 0 ], 0, 16 );
	info[ 1 ] = QString( "%1(0x%2)" ).arg( val[ 1 ] ).arg( val[ 1 ], 0, 16 );
	info[ 2 ] = QString( "%1(0x%2)" ).arg( val[ 2 ] ).arg( val[ 2 ], 0, 16 );
	info[ 3 ] = QString( "%1(0x%2)" ).arg( val[ 3 ] ).arg( val[ 3 ], 0, 16 );
}

inline void TexelInfoR32_UInt( QString info[ 4 ], uint8_t const* srcPtr )
{
	uint32_t val = *( (uint32_t*) srcPtr );
	info[ 0 ] = QString( "%1(0x%2)" ).arg( val ).arg( val, 0, 16 );
}

inline void TexelInfoR32_SInt( QString info[ 4 ], uint8_t const* srcPtr )
{
	int32_t val = *( (int32_t*) srcPtr );
	info[ 0 ] = QString( "%1(0x%2)" ).arg( val ).arg( val, 0, 16 );
}

inline void TexelInfoR32_Float( QString info[ 4 ], uint8_t const* srcPtr )
{
	float val;
	memcpy( &val, srcPtr, sizeof( float ) );
	info[ 0 ] = QString( "%1" ).arg( val );
}

inline void TexelInfoR16_Float( QString info[ 4 ], uint8_t const* srcPtr )
{
	float val;
	HalfToFloat( &val, srcPtr, 1 );
	info[ 0 ] = QString( "%1" ).arg( val );
}

inline void TexelInfoR24G8_UInt( QString info[ 4 ], uint8_t const* srcPtr )
{
	unsigned const valRUint		= srcPtr[ 0 ] + ( srcPtr[ 1 ] << 8 ) + ( srcPtr[ 2 ] << 16 );
	unsigned const valGUint		= srcPtr[ 3 ];
	float    const valRFloat	= valRUint / 16777216.0f;
	float    const valGFloat	= valGUint / 255.0f;
	info[ 0 ] = QString( "%1(%2)" ).arg( valRUint ).arg( valRFloat );
	info[ 1 ] = QString( "%1(%2)" ).arg( valGUint ).arg( valGFloat );
}

inline void TexelInfoR32S8( QString info[ 4 ], uint8_t const* srcPtr )
{
	float d32f;
	uint8_t stencil;
	memcpy( &d32f,		srcPtr + 0,	sizeof( float ) );
	memcpy( &stencil,	srcPtr + 4, sizeof( uint8_t ) );

	info[ 0 ] = QString( "%1" ).arg( d32f );
	info[ 1 ] = QString( "%1(%2)" ).arg( stencil ).arg( stencil / 255.0f );
}