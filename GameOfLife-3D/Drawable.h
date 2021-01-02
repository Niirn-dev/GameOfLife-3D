#pragma once

#include <vector>
#include <memory>
#include <type_traits>
#include "Bindable.h"
#include <DirectXMath.h>

class Drawable
{
public:
	virtual ~Drawable() = default;
	void Draw( class Graphics& gfx ) noexcept( !IS_DEBUG );
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
protected:
	void AddBind( std::shared_ptr<Bindable> bind ) noexcept;

	template<class T>
	T* QueryBindable() noexcept
	{
		static_assert( std::is_base_of_v<Bindable,T>,"Template type T should be derived from Bindable" );
		for ( auto& b : bindPtrs )
		{
			if ( auto pBind = dynamic_cast<T*>( b.get() ) )
			{
				return pBind;
			}
		}
		return nullptr;
	}
private:
	class IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::shared_ptr<Bindable>> bindPtrs;
};