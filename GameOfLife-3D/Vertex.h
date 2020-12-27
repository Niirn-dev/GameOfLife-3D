#pragma once

#include <vector>
#include <type_traits>
#include "Graphics.h"

class VertexLayout
{
public:
	enum class ElementType
	{
		Position2D,
		Position3D,
		Normal,
		Float3Color,
		Float4Color,
		Count,
	};
private:
	using ElemType = VertexLayout::ElementType;
public:
	template<ElementType> struct Map;
	template<> struct Map<ElemType::Position2D>
	{
		using SysType = DirectX::XMFLOAT2;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
		static constexpr const char* semantic = "Position";
		static constexpr const char* code = "P2";
	};
	template<> struct Map<ElemType::Position3D>
	{
		using SysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* semantic = "Position";
		static constexpr const char* code = "P3";
	};
	template<> struct Map<ElemType::Normal>
	{
		using SysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* semantic = "Normal";
		static constexpr const char* code = "N3";
	};
	template<> struct Map<ElemType::Float3Color>
	{
		using SysType = DirectX::XMFLOAT3;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
		static constexpr const char* semantic = "Color";
		static constexpr const char* code = "C3";
	};
	template<> struct Map<ElemType::Float4Color>
	{
		using SysType = DirectX::XMFLOAT4;
		static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
		static constexpr const char* semantic = "Color";
		static constexpr const char* code = "C4";
	};

	class Element
	{
	public:
		Element( ElementType type,size_t offset );
		size_t GetOffsetAfter() const noexcept( !IS_DEBUG );
		size_t GetOffset() const;
		size_t SizeBytes() const noexcept( !IS_DEBUG );
		ElementType GetType() const noexcept;
		D3D11_INPUT_ELEMENT_DESC GetDesc() const noexcept( !IS_DEBUG );
		std::string GetCode() const noexcept;

		static constexpr size_t SizeOf( ElementType type ) noexcept( !IS_DEBUG );
	private:
		template<ElementType Type>
		static constexpr D3D11_INPUT_ELEMENT_DESC GenerateDesc( size_t offset ) noexcept( !IS_DEBUG )
		{
			return { Map<Type>::semantic,0u,Map<Type>::dxgiFormat,0u,(UINT)offset,D3D11_INPUT_PER_VERTEX_DATA,0u };
		}
	private:
		ElementType type;
		size_t offset;
	};

public:
	template<ElementType Type>
	const Element& Resolve() const noexcept( !IS_DEBUG )
	{
		for ( auto& e : elements )
		{
			if ( e.GetType() == Type )
			{
				return e;
			}
		}
		assert( "Could not resolve element type" && false );
		return elements.front();
	}
	const Element& ResolveByIndex( size_t i ) const noexcept( !IS_DEBUG );
	VertexLayout& Append( ElementType type ) noexcept( !IS_DEBUG );
	size_t SizeBytes() const noexcept( !IS_DEBUG );
	size_t GetElementCount() const noexcept;
	std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DLayout() const noexcept( !IS_DEBUG );
	std::string GetCode() const noexcept;
private:
	std::vector<Element> elements;
};

class Vertex
{
	friend class VertexData;
public:
	template<VertexLayout::ElementType Type>
	auto& Attr() noexcept( !IS_DEBUG )
	{
		auto pAttribute = pData + layout.Resolve<Type>().GetOffset();
		return *reinterpret_cast<typename VertexLayout::Map<Type>::SysType*>( pAttribute );
	}
	template<typename T>
	void SetAttributeByIndex( size_t i,T&& val ) noexcept( !IS_DEBUG )
	{
		using ElType = VertexLayout::ElementType;

		const auto& element = layout.ResolveByIndex( i );
		auto pAttribute = pData + element.GetOffset();
		switch ( element.GetType() )
		{
		case ElType::Position2D:
			SetAttribute<ElType::Position2D>( pAttribute,std::forward<T>( val ) );
			break;
		case ElType::Position3D:
			SetAttribute<ElType::Position3D>( pAttribute,std::forward<T>( val ) );
			break;
		case ElType::Normal:
			SetAttribute<ElType::Normal>( pAttribute,std::forward<T>( val ) );
			break;
		case ElType::Float3Color:
			SetAttribute<ElType::Float3Color>( pAttribute,std::forward<T>( val ) );
			break;
		case ElType::Float4Color:
			SetAttribute<ElType::Float4Color>( pAttribute,std::forward<T>( val ) );
			break;
		default:
			assert( "Bad element type" && false );
		}
	}
protected:
	Vertex( char* pData,const VertexLayout& layout ) noexcept( !IS_DEBUG );
private:
	// enables parameter pack setting of multiple parameters by element index
	template<typename First,typename ...Rest>
	void SetAttributeByIndex( size_t i,First&& first,Rest&&... rest )
	{
		SetAttributeByIndex( i,std::forward<First>( first ) );
		SetAttributeByIndex( i + 1,std::forward<Rest>( rest )... );
	}
	// helper to reduce code duplication in SetAttributeByIndex
	template<VertexLayout::ElementType DestLayoutType,typename SrcType>
	void SetAttribute( char* pAttribute,SrcType&& val ) noexcept( !IS_DEBUG )
	{
		using Dest = typename VertexLayout::Map<DestLayoutType>::SysType;
		if constexpr( std::is_assignable_v<Dest,SrcType> )
		{
			*reinterpret_cast<Dest*>( pAttribute ) = val;
		}
		else
		{
			assert( "Parameter attribute type mismatch" && false );
		}
	}
private:
	char* pData = nullptr;
	const VertexLayout& layout;
};

class ConstVertex
{
public:
	ConstVertex( const Vertex& v ) noexcept( !IS_DEBUG );
	template<VertexLayout::ElementType Type>
	const auto& Attr() const noexcept( !IS_DEBUG )
	{
		return const_cast<Vertex&>( vertex ).Attr<Type>();
	}
private:
	Vertex vertex;
};

class VertexData
{
public:
	VertexData( VertexLayout layout ) noexcept( !IS_DEBUG );
	const char* GetData() const noexcept( !IS_DEBUG );
	const VertexLayout& GetLayout() const noexcept;
	size_t Size() const noexcept( !IS_DEBUG );
	size_t SizeBytes() const noexcept( !IS_DEBUG );
	template<typename ...Params>
	void EmplaceBack( Params&&... params ) noexcept( !IS_DEBUG )
	{
		assert( sizeof...( params ) == layout.GetElementCount()  && "Param count doesn't match number of vertex elements" );
		buffer.resize( buffer.size() + layout.SizeBytes() );
		Back().SetAttributeByIndex( 0u,std::forward<Params>( params )... );
	}
	Vertex Back() noexcept( !IS_DEBUG );
	Vertex Front() noexcept( !IS_DEBUG );
	Vertex operator[]( size_t i ) noexcept( !IS_DEBUG );
	ConstVertex Back() const noexcept( !IS_DEBUG );
	ConstVertex Front() const noexcept( !IS_DEBUG );
	ConstVertex operator[]( size_t i ) const noexcept( !IS_DEBUG );
private:
	std::vector<char> buffer;
	VertexLayout layout;
};