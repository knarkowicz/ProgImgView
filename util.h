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