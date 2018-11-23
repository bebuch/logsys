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
		using log_type = Log;
		using rtp_type = void;
	};

	/// \brief Implementation of extract_log_t
	template < typename RTP, typename Log, typename R >
	struct extract_log_impl< R(Log, RTP) >{
		using log_type = Log;
		using rtp_type = RTP;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename R >
	struct extract_log_impl< R(*)(Log) >{
		using log_type = Log;
		using rtp_type = void;
	};

	/// \brief Implementation of extract_log_t
	template < typename RTP, typename Log, typename R >
	struct extract_log_impl< R(*)(Log, RTP) >{
		using log_type = Log;
		using rtp_type = RTP;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename R, typename F >
	struct extract_log_impl< R(F::*)(Log) >{
		using log_type = Log;
		using rtp_type = void;
	};

	/// \brief Implementation of extract_log_t
	template < typename RTP, typename Log, typename R, typename F >
	struct extract_log_impl< R(F::*)(Log, RTP) >{
		using log_type = Log;
		using rtp_type = RTP;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename R, typename F >
	struct extract_log_impl< R(F::*)(Log)const >{
		using log_type = Log;
		using rtp_type = void;
	};

	/// \brief Implementation of extract_log_t
	template < typename RTP, typename Log, typename R, typename F >
	struct extract_log_impl< R(F::*)(Log, RTP)const >{
		using log_type = Log;
		using rtp_type = RTP;
	};


	/// \brief Implementation of extract_log_t
	template < typename Log, typename R >
	struct extract_log_impl< R(Log)noexcept >{
		using log_type = Log;
		using rtp_type = void;
	};

	/// \brief Implementation of extract_log_t
	template < typename RTP, typename Log, typename R >
	struct extract_log_impl< R(Log, RTP)noexcept >{
		using log_type = Log;
		using rtp_type = RTP;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename R >
	struct extract_log_impl< R(*)(Log)noexcept >{
		using log_type = Log;
		using rtp_type = void;
	};

	/// \brief Implementation of extract_log_t
	template < typename RTP, typename Log, typename R >
	struct extract_log_impl< R(*)(Log, RTP)noexcept >{
		using log_type = Log;
		using rtp_type = RTP;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename R, typename F >
	struct extract_log_impl< R(F::*)(Log)noexcept >{
		using log_type = Log;
		using rtp_type = void;
	};

	/// \brief Implementation of extract_log_t
	template < typename RTP, typename Log, typename R, typename F >
	struct extract_log_impl< R(F::*)(Log, RTP)noexcept >{
		using log_type = Log;
		using rtp_type = RTP;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename R, typename F >
	struct extract_log_impl< R(F::*)(Log)const noexcept >{
		using log_type = Log;
		using rtp_type = void;
	};

	/// \brief Implementation of extract_log_t
	template < typename RTP, typename Log, typename R, typename F >
	struct extract_log_impl< R(F::*)(Log, RTP)const noexcept >{
		using log_type = Log;
		using rtp_type = RTP;
	};


	template < typename T >
	struct remove_lvalue_reference{
		using type = T;
	};

	template < typename T >
	struct remove_lvalue_reference< T& >{
		using type = T;
	};

	template < typename T >
	using remove_lvalue_reference_t =
		typename remove_lvalue_reference< T >::type;


	template < typename BodyRT, typename RTP >
	constexpr bool is_valid_body_return_type_parameter =
		std::is_void_v< RTP > ||
		std::is_same_v<
			std::remove_const_t< remove_lvalue_reference_t< RTP > >,
			optional< BodyRT > >;


	/// \brief Implementation of extract_log_t
	template < typename LogF, typename BodyRT, typename Void = void >
	struct extract_log{
		using type = std::remove_reference_t<
			typename extract_log_impl< LogF >::log_type >;

		static_assert(
			std::is_lvalue_reference_v<
				typename extract_log_impl< LogF >::log_type >,
			"First parameter of a LogMessageProvider must be a "
			"lvalue-reference to your log-object.");

		static_assert(
			is_valid_body_return_type_parameter< BodyRT,
				typename extract_log_impl< LogF >::rtp_type >,
			"If there is a second parameter in the LogMessageProvider, it "
			"must be logsys::optional< BodyReturnType >. "
			"(Optionally const and with lvalue references.)");
	};

	/// \brief Implementation of extract_log_t
	template < typename LogF, typename BodyRT >
	struct extract_log< LogF, BodyRT,
		std::void_t< decltype(&LogF::operator()) >
	>{
		using type = typename extract_log<
			decltype(&LogF::operator()), BodyRT >::type;
	};


	/// \brief Extract type of first parameter from log function
	template < typename LogF, typename BodyRT = void >
	using extract_log_t =
		typename extract_log< std::remove_reference_t< LogF >, BodyRT >::type;


	/// \brief Checks whether extract_log_t< Log, BodyRT > is a
	///        valid expression
	template < typename LogF, typename BodyRT = void >
	constexpr bool is_extract_log_valid_v =
		[]{
			if constexpr(std::is_reference_v< LogF >){
				return is_extract_log_valid_v<
					std::remove_reference_t< LogF >, BodyRT >;
			}else if constexpr(has_unique_call_operator_v<LogF>){
				return is_extract_log_valid_v<
					decltype(&LogF::operator()), BodyRT >;
			}else if constexpr(
				is_defined_v< extract_log_impl< LogF > >
			){
				return
					std::is_lvalue_reference_v<
						typename extract_log_impl< LogF >::log_type > &&
					is_valid_body_return_type_parameter< BodyRT,
						typename extract_log_impl< LogF >::rtp_type >;

			}else{
				return false;
			}
		}();


}


#endif
