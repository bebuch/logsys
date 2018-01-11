//-----------------------------------------------------------------------------
// Copyright (c) 2017-2018 Benjamin Buch
//
// https://github.com/bebuch/logsys
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _logsys__is_valid__hpp_INCLUDED_
#define _logsys__is_valid__hpp_INCLUDED_

#include <utility>


namespace logsys{


	/// \brief Implementation of is_valid_t
	template < typename F, typename Log, typename =
		decltype(std::declval< F&& >()(std::declval< Log& >())) >
	constexpr bool is_valid_impl(int){ return true; }

	/// \brief Implementation of is_valid_t
	template < typename F, typename Log >
	constexpr bool is_valid_impl(...){ return false; }

	/// \brief SFINAE expression evaluartor, use lambda return type definition
	template < typename Log >
	struct is_valid_t{
		template < typename F >
		constexpr bool operator()(F&&)const
		{ return is_valid_impl< F, Log& >(int{}); }
	};

	/// \copydoc is_valid_t
	template < typename Log >
	constexpr is_valid_t< Log > is_valid{};


}


#endif
