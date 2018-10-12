//-----------------------------------------------------------------------------
// Copyright (c) 2017-2018 Benjamin Buch
//
// https://github.com/bebuch/logsys
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _logsys__tool__log_base__hpp_INCLUDED_
#define _logsys__tool__log_base__hpp_INCLUDED_

#include "stdlogb.hpp"
#include "log.hpp"

#include <string>


namespace logsys{


	namespace detail{


		template < typename LogF >
		constexpr bool is_void_log_fn =
			std::is_invocable_v< LogF, logsys::stdlogb& >;

		template < typename LogF, typename T >
		constexpr bool is_result_log_fn =
			std::is_invocable_v< LogF, logsys::stdlogb&, T >;


	}


	/// \brief Base class that implementes log functions as members
	class log_base{
	public:
		/// \brief Add a line to the log
		template < typename LogF >
		decltype(auto) log(LogF&& f)const{
			return logsys::log< logsys::stdlogb >(simple_impl(f));
		}

		/// \brief Add a line to the log with linked code block
		template < typename LogF, typename Body >
		decltype(auto) log(LogF&& f, Body&& body)const{
			using result_type = logsys::detail::result_as_ptr_t< Body >;

			if constexpr(std::is_void_v< result_type >){
				static_assert(detail::is_void_log_fn< LogF >,
					"expected a log call of the form: "
					"'.log([](logsys::stdlogb&){}, []{})'");

				logsys::log< logsys::stdlogb >(simple_impl(f), body);
			}else{
				static_assert(detail::is_void_log_fn< LogF >
					|| detail::is_result_log_fn< LogF, result_type >,
					"expected a log call of the form: "
					"'.log([](logsys::stdlogb&){}, []{ return ...; })' or "
					"'.log([](logsys::stdlogb&, auto const* result){}, "
					"[]{ return ...; })'");

				if constexpr(detail::is_void_log_fn< LogF >){
					return logsys::log< logsys::stdlogb >(simple_impl(f), body);
				}else{
					return logsys::log< logsys::stdlogb >(
						extended_impl< result_type >(f), body);
				}
			}
		}

		/// \brief Add a line to the log with linked code block and catch all
		///        exceptions
		template < typename LogF, typename Body >
		decltype(auto) exception_catching_log(LogF&& f, Body&& body)const{
			using result_type = logsys::detail::result_as_ptr_t< Body >;

			if constexpr(std::is_void_v< result_type >){
				static_assert(detail::is_void_log_fn< LogF >,
					"expected a log call of the form: "
					"'.exception_catching_log([](logsys::stdlogb&){}, []{})'");

				logsys::exception_catching_log< logsys::stdlogb >(
					simple_impl(f), body);
			}else{
				static_assert(detail::is_void_log_fn< LogF >
					|| detail::is_result_log_fn< LogF, result_type >,
					"expected a log call of the form: "
					"'.exception_catching_log([](logsys::stdlogb&){}, "
					"[]{ return ...; })' or "
					"'.exception_catching_log([](logsys::stdlogb&, "
					"auto const* result){}, "
					"[]{ return ...; })'");

				if constexpr(detail::is_void_log_fn< LogF >){
					return logsys::exception_catching_log< logsys::stdlogb >(
						simple_impl(f), body);
				}else{
					return logsys::exception_catching_log< logsys::stdlogb >(
						extended_impl< result_type >(f), body);
				}
			}
		}


		/// \brief Get the log prefix message
		std::string log_prefix()const{
			return log_prefix_;
		}


	protected:
		log_base(std::string const& log_prefix)
			: log_prefix_(log_prefix) {}

		log_base(std::string&& log_prefix)noexcept
			: log_prefix_(std::move(log_prefix)) {}


	private:
		/// \brief Helper for log message functions
		template < typename LogF >
		auto simple_impl(LogF& log)const{
			return [&](logsys::stdlogb& os){
				os << log_prefix_;
				log(os);
			};
		}

		/// \brief Helper for log message functions
		template < typename T, typename LogF >
		auto extended_impl(LogF& log)const{
			return [&](logsys::stdlogb& os, T result){
				os << log_prefix_;
				log(os, result);
			};
		}


		/// \brief The log prefix message
		std::string log_prefix_;
	};


}


#endif
