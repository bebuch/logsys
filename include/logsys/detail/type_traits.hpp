//-----------------------------------------------------------------------------
// Copyright (c) 2017-2018 Benjamin Buch
//
// https://github.com/bebuch/logsys
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _logsys__detail__type_traits__hpp_INCLUDED_
#define _logsys__detail__type_traits__hpp_INCLUDED_

#include <type_traits>


namespace logsys::detail{


	/// \brief Helper to form void by a numeric template argument
	template < auto > using number2void = void;

	/// \brief By default types are not defined
	template < typename T, typename = void >
	constexpr bool is_defined_v = false;

	/// \brief If a type is defined this specialization is used
	template < typename T >
	constexpr bool is_defined_v< T, number2void< sizeof(T) > > = true;


	/// \brief Checks whether `expr_fn` is is invocable with `Param&`
	template < typename Param >
	constexpr auto is_valid = [](auto&& expr_fn){
			return std::is_invocable< decltype(expr_fn), Param& >();
		};


	/// \brief T has no unique function operator
	template < typename T, typename Void = void >
	constexpr bool has_unique_call_operator_v = false;

	/// \brief T has a unique function operator
	template < typename T >
	constexpr bool has_unique_call_operator_v< T,
		std::void_t< decltype(&T::operator()) > > = true;


	template < typename T, typename Void = void >
	struct extract_callable{
		using type = T;
	};

	template < typename T >
	struct extract_callable< T, std::void_t< decltype(&T::operator()) > >{
		using type = decltype(&T::operator());
	};


	/// \brief Type of function operator if there is one unique, T otherwise
	template < typename T >
	using extract_callable_t = typename extract_callable< T >::type;



}


#endif
