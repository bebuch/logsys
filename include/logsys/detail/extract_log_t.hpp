//-----------------------------------------------------------------------------
// Copyright (c) 2017-2018 Benjamin Buch
//
// https://github.com/bebuch/logsys
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _logsys__detail__extract_log_t__hpp_INCLUDED_
#define _logsys__detail__extract_log_t__hpp_INCLUDED_

#include "type_traits.hpp"

#include "../optional.hpp"


namespace logsys::detail{


	/// \brief Extract type of LogF parameter
	template < typename LogF >
	struct extract_log_impl;


	/// \brief Implementation of extract_log_t
	template < typename Log, typename R >
	struct extract_log_impl< R(Log) >{
		using type = Log;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename R >
	struct extract_log_impl< R(*)(Log) >{
		using type = Log;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename R, typename F >
	struct extract_log_impl< R(F::*)(Log) >{
		using type = Log;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename R, typename F >
	struct extract_log_impl< R(F::*)(Log)const >{
		using type = Log;
	};


	/// \brief Implementation of extract_log_t
	template < typename Log, typename R >
	struct extract_log_impl< R(Log)noexcept >{
		using type = Log;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename R >
	struct extract_log_impl< R(*)(Log)noexcept >{
		using type = Log;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename R, typename F >
	struct extract_log_impl< R(F::*)(Log)noexcept >{
		using type = Log;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename R, typename F >
	struct extract_log_impl< R(F::*)(Log)const noexcept >{
		using type = Log;
	};


	/// \brief Implementation of extract_log_t
	template < typename LogF, typename Void = void >
	struct extract_log{
		using type = std::remove_reference_t<
			typename extract_log_impl< LogF >::type >;

		static_assert(
			std::is_lvalue_reference_v<
				typename extract_log_impl< LogF >::type >,
			"First parameter of a LogMessageProvider must be a lvalue-reference "
			"to your log-object.");
	};

	/// \brief Implementation of extract_log_t
	template < typename LogF >
	struct extract_log< LogF, std::void_t< decltype(&LogF::operator()) > >{
		using type = typename extract_log< decltype(&LogF::operator()) >::type;
	};


	/// \brief Extract type of first parameter from log function
	template < typename LogF >
	using extract_log_t =
		typename extract_log< std::remove_reference_t< LogF > >::type;


	/// \brief Checks whether extract_log_t< Log > is a valid expression
	template < typename LogF >
	constexpr bool is_extract_log_valid_v =
		[]{
			if constexpr(std::is_reference_v< LogF >){
				return is_extract_log_valid_v<
					std::remove_reference_t< LogF > >;
			}else if constexpr(has_unique_call_operator_v<LogF>){
				return is_extract_log_valid_v< decltype(&LogF::operator()) >;
			}else if constexpr(is_defined_v< extract_log_impl< LogF > >){
				return std::is_lvalue_reference_v<
					typename extract_log_impl< LogF >::type >;
			}else{
				return false;
			}
		}();


}


#endif
