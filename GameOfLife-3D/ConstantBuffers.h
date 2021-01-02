#pragma once

#include "Bindable.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"

template<class C>
class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer( Graphics& gfx,UINT slot = 0u )
		:
		slot( slot )
	{
		INFOMAN( gfx );

		D3D11_BUFFER_DESC bDesc = {};
		bDesc.ByteWidth = sizeof( C );
		bDesc.Usage = D3D11_USAGE_DYNAMIC;
		bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bDesc.MiscFlags = 0u;
		bDesc.StructureByteStride = 0u;

		GFX_THROW_INFO( GetDevice( gfx )->CreateBuffer( &bDesc,nullptr,&pConstantBuffer ) );
	}
	ConstantBuffer( Graphics& gfx,const C& consts,UINT slot = 0u )
		:
		slot( slot )
	{
		INFOMAN( gfx );

		D3D11_BUFFER_DESC bDesc = {};
		bDesc.ByteWidth = sizeof( C );
		bDesc.Usage = D3D11_USAGE_DYNAMIC;
		bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bDesc.MiscFlags = 0u;
		bDesc.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA srd = {};
		srd.pSysMem = &consts;

		GFX_THROW_INFO( GetDevice( gfx )->CreateBuffer( &bDesc,&srd,&pConstantBuffer ) );
	}

	void Update( Graphics& gfx,const C& consts )
	{
		INFOMAN( gfx );

		D3D11_MAPPED_SUBRESOURCE msr;
		GFX_THROW_INFO( GetContext( gfx )->Map(
			pConstantBuffer.Get(),0u,
			D3D11_MAP_WRITE_DISCARD,0u,
			&msr
		) );
		memcpy( msr.pData,&consts,sizeof( consts ) );
		GetContext( gfx )->Unmap( pConstantBuffer.Get(),0u );
	}

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	UINT slot;
};

template<class C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using ConstantBuffer<C>::slot;
	using Bindable::GetContext;
	using Bindable::GetInfoManager;
public:
	using ConstantBuffer<C>::ConstantBuffer;

	void Bind( Graphics& gfx ) noexcept( !IS_DEBUG ) override
	{
		INFOMAN_ONLY( gfx );

		GFX_THROW_INFO_ONLY( GetContext( gfx )->VSSetConstantBuffers( slot,1u,pConstantBuffer.GetAddressOf() ) );
	}

	static std::shared_ptr<Bindable> Resolve( Graphics& gfx,UINT slot = 0u ) noexcept( !IS_DEBUG )
	{
		return BindableCodex::Resolve<VertexConstantBuffer<C>>( gfx,slot );
	}
	static std::shared_ptr<Bindable> Resolve( Graphics& gfx,const C& consts,UINT slot = 0u ) noexcept( !IS_DEBUG )
	{
		return BindableCodex::Resolve<VertexConstantBuffer<C>>( gfx,consts,slot );
	}
	static std::string GenerateUID( UINT slot ) noexcept
	{
		using namespace std::string_literals;
		return typeid( VertexConstantBuffer<C> ).name() + "#"s + std::to_string( slot );
	}
	static std::string GenerateUID( const C& consts,UINT slot ) noexcept
	{
		return GenerateUID( slot );
	}
	std::string GetUID() const noexcept override
	{
		return GenerateUID( slot );
	}
};

template<class C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using ConstantBuffer<C>::slot;
	using Bindable::GetContext;
	using Bindable::GetInfoManager;
public:
	using ConstantBuffer<C>::ConstantBuffer;

	void Bind( Graphics& gfx ) noexcept( !IS_DEBUG ) override
	{
		INFOMAN_ONLY( gfx );

		GFX_THROW_INFO_ONLY( GetContext( gfx )->PSSetConstantBuffers( slot,1u,pConstantBuffer.GetAddressOf() ) );
	}

	static std::shared_ptr<Bindable> Resolve( Graphics& gfx,UINT slot = 0u ) noexcept( !IS_DEBUG )
	{
		return BindableCodex::Resolve<PixelConstantBuffer<C>>( gfx,slot );
	}
	static std::shared_ptr<Bindable> Resolve( Graphics& gfx,const C& consts,UINT slot = 0u ) noexcept( !IS_DEBUG )
	{
		return BindableCodex::Resolve<PixelConstantBuffer<C>>( gfx,consts,slot );
	}
	static std::string GenerateUID( UINT slot ) noexcept
	{
		using namespace std::string_literals;
		return typeid( PixelConstantBuffer<C> ).name() + "#"s + std::to_string( slot );
	}
	static std::string GenerateUID( const C& consts,UINT slot ) noexcept
	{
		return GenerateUID( slot );
	}
	std::string GetUID() const noexcept override
	{
		return GenerateUID( slot );
	}
};