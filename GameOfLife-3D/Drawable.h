#pragma once

#include <vector>
#include <memory>
#include "Bindable.h"

class Drawable
{
public:
	void Draw( class Graphics& gfx) noexcept( !IS_DEBUG );
protected:
	void AddBind( std::unique_ptr<Bindable> bind ) noexcept;
private:
	class IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> bindPtrs;
};