#pragma once

enum class EViewChannel : uint8_t
{
	RGB,
	R,
	G,
	B,
	A
};

class CBaseWindow
{
public:
	virtual void			Reload() = 0;
	virtual void			ZoomIn() = 0;
	virtual void			ZoomOut() = 0;
	virtual void			SetViewChannel( EViewChannel channel ) = 0;
	virtual void			SetViewFace( unsigned face ) = 0;
	virtual void			SetViewMipMap( unsigned mipMap ) = 0;
	virtual void			SetViewMin( float min ) = 0;
	virtual void			SetViewMax( float max ) = 0;
	virtual void			SetViewGamma( float gamma ) = 0;
	virtual void			SetViewDiffMult( float mult ) = 0;
	virtual QSize			GetInitialSize() const = 0;
	virtual QString const&	GetTitle() const = 0;
	virtual EViewChannel	GetViewChannel() const = 0;
	virtual unsigned		GetMipNum()	const = 0;
	virtual unsigned		GetFaceNum() const = 0;
	virtual unsigned		GetViewFace() const = 0;
	virtual unsigned		GetViewMipMap() const = 0;
	virtual float			GetViewMin() const = 0;
	virtual float			GetViewMax() const = 0;
	virtual float			GetViewGamma() const = 0;
	virtual float			GetViewDiffMult() const = 0;
};