#include "Vertex.h"
#include <sstream>

// VertexLayout::Element
VertexLayout::Element::Element( ElementType type,size_t offset )
    :
    type( type ),
    offset( offset )
{}

size_t VertexLayout::Element::GetOffsetAfter() const noexcept( !IS_DEBUG )
{
    return offset + SizeBytes();
}

size_t VertexLayout::Element::GetOffset() const
{
    return offset;
}

size_t VertexLayout::Element::SizeBytes() const noexcept( !IS_DEBUG )
{
    return SizeOf( type );
}

VertexLayout::ElementType VertexLayout::Element::GetType() const noexcept
{
    return type;
}

D3D11_INPUT_ELEMENT_DESC VertexLayout::Element::GetDesc() const noexcept( !IS_DEBUG )
{
    using ElType = VertexLayout::ElementType;
    switch ( type )
    {
    case ElType::Position2D:
        return GenerateDesc<ElType::Position2D>( GetOffset() );
    case ElType::Position3D:
        return GenerateDesc<ElType::Position3D>( GetOffset() );
    case ElType::Normal:
        return GenerateDesc<ElType::Normal>( GetOffset() );
    case ElType::Float3Color:
        return GenerateDesc<ElType::Float3Color>( GetOffset() );
    case ElType::Float4Color:
        return GenerateDesc<ElType::Float4Color>( GetOffset() );
    default:
        assert( "Invalid element type" && false );
        return { "INVALID",0u,DXGI_FORMAT_UNKNOWN,0u,0u,D3D11_INPUT_PER_VERTEX_DATA,0u };
    }
}

std::string VertexLayout::Element::GetCode() const noexcept
{
    using ElType = VertexLayout::ElementType;
    switch ( type )
    {
    case ElType::Position2D:
        return Map<ElType::Position2D>::code;
    case ElType::Position3D:
        return Map<ElType::Position3D>::code;
    case ElType::Normal:
        return Map<ElType::Normal>::code;
    case ElType::Float3Color:
        return Map<ElType::Float3Color>::code;
    case ElType::Float4Color:
        return Map<ElType::Float4Color>::code;
    default:
        assert( "Invalid element type" && false );
        return "INVALID";
    }
}

constexpr size_t VertexLayout::Element::SizeOf( ElementType type ) noexcept( !IS_DEBUG )
{
    using ElType = VertexLayout::ElementType;
    switch ( type )
    {
    case ElType::Position2D:
        return sizeof( Map<ElType::Position2D>::SysType );
    case ElType::Position3D:
        return sizeof( Map<ElType::Position3D>::SysType );
    case ElType::Normal:
        return sizeof( Map<ElType::Normal>::SysType );
    case ElType::Float3Color:
        return sizeof( Map<ElType::Float3Color>::SysType );
    case ElType::Float4Color:
        return sizeof( Map<ElType::Float4Color>::SysType );
    default:
        assert( "Invalid element type" && false );
        return 0u;
    }
}

// VertexLayout
const VertexLayout::Element& VertexLayout::ResolveByIndex( size_t i ) const noexcept( !IS_DEBUG )
{
    return elements[i];
}

VertexLayout& VertexLayout::Append( ElementType type ) noexcept( !IS_DEBUG )
{
    elements.emplace_back( type,SizeBytes() );
    return *this;
}

size_t VertexLayout::SizeBytes() const noexcept( !IS_DEBUG )
{
    return elements.empty() ? 0u : elements.back().GetOffsetAfter();
}

size_t VertexLayout::GetElementCount() const noexcept
{
    return elements.size();
}

std::vector<D3D11_INPUT_ELEMENT_DESC> VertexLayout::GetD3DLayout() const noexcept( !IS_DEBUG )
{
    std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
    desc.reserve( GetElementCount() );
    for ( const auto& e : elements )
    {
        desc.push_back( e.GetDesc() );
    }
    return std::move( desc );
}

std::string VertexLayout::GetCode() const noexcept
{
    using namespace std::string_literals;
    std::stringstream code;
    std::string delimiter = ""s;
    for ( const auto& e : elements )
    {
        code << std::exchange( delimiter,"#"s ) << e.GetCode();
    }
    return code.str();
}

// Vertex
Vertex::Vertex( char* pData,const VertexLayout& layout ) noexcept( !IS_DEBUG )
    :
    pData( pData ),
    layout( layout )
{
    assert( pData != nullptr );
}
ConstVertex::ConstVertex( const Vertex& v ) noexcept( !IS_DEBUG )
    :
    vertex( v )
{}

// VertexData
VertexData::VertexData( VertexLayout layout ) noexcept( !IS_DEBUG )
    :
    layout( std::move( layout ) )
{}

const char* VertexData::GetData() const noexcept( !IS_DEBUG )
{
    return buffer.data();
}

const VertexLayout& VertexData::GetLayout() const noexcept
{
    return layout;
}

size_t VertexData::Size() const noexcept( !IS_DEBUG )
{
    return buffer.size() / layout.SizeBytes();
}

size_t VertexData::SizeBytes() const noexcept( !IS_DEBUG )
{
    return buffer.size();
}

Vertex VertexData::Back() noexcept( !IS_DEBUG )
{
    assert( buffer.size() != 0u );
    return Vertex{ buffer.data() + buffer.size() - layout.SizeBytes(),layout };
}

Vertex VertexData::Front() noexcept( !IS_DEBUG )
{
    return Vertex{ buffer.data(),layout };
}

Vertex VertexData::operator[]( size_t i ) noexcept( !IS_DEBUG )
{
    return Vertex{ buffer.data() + layout.SizeBytes() * i,layout };
}

ConstVertex VertexData::Back() const noexcept( !IS_DEBUG )
{
    return const_cast<VertexData*>( this )->Back();
}

ConstVertex VertexData::Front() const noexcept( !IS_DEBUG )
{
    return const_cast<VertexData*>( this )->Front();
}

ConstVertex VertexData::operator[]( size_t i ) const noexcept( !IS_DEBUG )
{
    return const_cast<VertexData&>( *this )[i];
}
