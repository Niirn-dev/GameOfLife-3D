#pragma once

#include <concepts>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include "Bindable.h"

template<class B>
concept Bind = requires {
	std::is_base_of_v<Bindable,B>;
};

class BindableCodex
{
public:
	template<Bind B,typename...Params>
	static std::shared_ptr<Bindable> Resolve( Graphics& gfx,Params&&... params ) noexcept( !IS_DEBUG )
	{
		return Get_().Resolve_<B>( gfx,std::forward<Params>( params )... );
	}

private:
	template<Bind B,typename...Params>
	std::shared_ptr<Bindable> Resolve_( Graphics& gfx,Params&&... params ) noexcept( !IS_DEBUG )
	{
		const auto key = B::GenerateUID( std::forward<Params>( params )... );
		const auto bindIt = binds.find( key );
		if ( bindIt != binds.end() )
		{
			return bindIt->second;
		}
		else
		{
			auto bind = std::make_shared<B>( gfx,std::forward<Params>( params )... );
			binds[key] = bind;
			return bind;
		}
	}

	BindableCodex() = default;
	static BindableCodex& Get_() noexcept
	{
		static BindableCodex codex{};
		return codex;
	}

private:
	std::unordered_map<std::string,std::shared_ptr<Bindable>> binds;
};