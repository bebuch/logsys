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


	/// \brief Base class that implementes log functions as members
	class log_base{
	public:
		/// \brief Add a line to the log
		template < typename LogF >
		decltype(auto) log(LogF&& log_f)const{
			static_assert(detail::is_extract_log_valid_v< LogF, detail::nobody_t >,
				"Can not extract Log type from first parameter of the log "
				"function. "
				"A valid log()-call without body must have the form: "
				"'this->log([](Log&){});' where Log is your Log type.");

			using log_type = detail::extract_log_t< LogF, detail::nobody_t >;

			static_assert(detail::is_simple_log_f< LogF, log_type >,
				"Argument log_f is not a valid log message function. "
				"A valid log()-call without body must have the form: "
				"'this->log([](Log&){});' where Log is your Log type.");

			::logsys::log([&](log_type& log){
					log << log_prefix_;
					log_f(log);
				});
		}

		/// \brief Add a line to the log with linked code block
		template < typename LogF, typename Body >
		decltype(auto) log(LogF&& log_f, Body&& body)const{
			using body_return_type = detail::body_return_t< Body >;
			static_assert(
				detail::is_extract_log_valid_v< LogF, body_return_type >,
				"Can not extract Log type from first parameter of the log "
				"function. A valid log()-call with body must have the form: "
				"'this->log([](Log&){}, []{});' or "
				"'this->log([](Log&, "
				"logsys::optional< value_type > const& value){}, "
				"[]{ return value; });' where Log is your Log type.");

			using log_type = detail::extract_log_t< LogF, body_return_type >;

			if constexpr(detail::is_simple_log_f< LogF, log_type >){
				return ::logsys::log(
						[&](log_type& log){
							log << log_prefix_;
							log_f(log);
						}, body);
			}else{
				return ::logsys::log(
						[&](
							log_type& log,
							optional< body_return_type > const& result
						){
							log << log_prefix_;
							log_f(log, result);
						}, body);
			}
		}

		/// \brief Add a line to the log with linked code block and catch all
		///        exceptions
		template < typename LogF, typename Body >
		auto exception_catching_log(LogF&& log_f, Body&& body)const{
			using body_return_type = detail::body_return_t< Body >;
			static_assert(
				detail::is_extract_log_valid_v< LogF, body_return_type >,
				"Can not extract Log type from first parameter of the "
				"exception_catching_log function. A valid "
				"exception_catching_log()-call must have the form: "
				"'this->exception_catching_log([](Log&){}, []{});' or "
				"'this->exception_catching_log("
				"[](Log&, logsys::optional< value_type > const& value){}, "
				"[]{ return value });' where Log is your Log type.");

			using log_type = detail::extract_log_t< LogF, body_return_type >;

			if constexpr(detail::is_simple_log_f< LogF, log_type >){
				return ::logsys::exception_catching_log(
						[&](log_type& log){
							log << log_prefix_;
							log_f(log);
						}, body);
			}else{
				return ::logsys::exception_catching_log(
						[&](
							log_type& log,
							optional< body_return_type > const& result
						){
							log << log_prefix_;
							log_f(log, result);
						}, body);
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
		/// \brief The log prefix message
		std::string log_prefix_;
	};


}


#endif
