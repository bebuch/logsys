//-----------------------------------------------------------------------------
// Copyright (c) 2015-2018 Benjamin Buch
//
// https://github.com/bebuch/logsys
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _logsys__log__hpp_INCLUDED_
#define _logsys__log__hpp_INCLUDED_

#include "detail/extract_log_t.hpp"
#include "detail/log_trait.hpp"

#include <functional>


namespace logsys::detail{


	template < typename LogF, typename Log >
	constexpr bool is_simple_log_f = std::is_invocable_v< LogF&, Log& >;

	template < typename LogF, typename Log, typename BodyRT >
	constexpr bool is_extended_log_f = std::is_invocable_v< LogF&, Log&,
		optional< BodyRT > const& >;

	template < typename LogF, typename Log, typename BodyRT >
	constexpr bool is_valid_log_f = is_simple_log_f< LogF, Log >
		|| is_extended_log_f< LogF, Log, BodyRT >;


	template < typename Body >
	struct body_return_type{
		static_assert(std::is_invocable_v< Body& >,
			"Parameter body must be a callable without arguments.");

		using type = std::invoke_result_t< Body& >;
	};

	template < typename Body >
	using body_return_t = typename body_return_type< Body >::type;


	/// \brief Execute user defined log function and call `exec` on log object
	///
	/// Call `set_log_exception` before `exec` if user defined log function
	/// throwed an exception.
	template < typename LogF, typename Log, typename BodyRT >
	inline void exec_log(
		LogF& log_f,
		Log& log,
		optional< BodyRT > const& return_value
	)noexcept{
		try{
			if constexpr(is_simple_log_f< LogF, Log >){
				(void)return_value; // Silance GCC
				std::invoke(log_f, log);
			}else{
				std::invoke(log_f, log, return_value);
			}
		}catch(...){
			log.set_log_exception(std::current_exception());
		}

		log.exec();
	}

	/// \brief Log without a body function
	template < typename LogF, typename Log >
	inline void log(LogF& log_f)noexcept{
		auto log = Log();

		try{
			std::invoke(log_f, log);
		}catch(...){
			log.set_log_exception(std::current_exception());
		}

		log.exec();
	}

	/// \brief Log with a body function
	template < typename LogF, typename Log, typename Body, typename BodyRT >
	inline BodyRT log(LogF& log_f, Body& body){
		auto log = Log();

		try{
			if constexpr(std::is_void_v< BodyRT >){
				std::invoke(body);

				if constexpr(log_trait< Log >::has_body_finished){
					log.body_finished();
				}

				exec_log< LogF, Log, BodyRT >(log_f, log, true);
			}else{
				optional< BodyRT > body_value(std::invoke(body));

				if constexpr(log_trait< Log >::has_body_finished){
					log.body_finished();
				}

				exec_log< LogF, Log, BodyRT >(log_f, log, body_value);

				return *std::move(body_value);
			}
		}catch(...){
			if constexpr(log_trait< Log >::has_body_finished){
				log.body_finished();
			}

			log.set_body_exception(std::current_exception());

			auto body_value = optional< BodyRT >();
			exec_log< LogF, Log, BodyRT >(log_f, log, body_value);

			throw;
		}
	}

	/// \brief Exception catching log (with a body function)
	template < typename LogF, typename Log, typename Body, typename BodyRT >
	inline optional< BodyRT >
	exception_catching_log(LogF& log_f, Body& body)noexcept{
		auto log = Log();

		try{
			if constexpr(std::is_void_v< BodyRT >){
				std::invoke(body);

				if constexpr(log_trait< Log >::has_body_finished){
					log.body_finished();
				}

				exec_log< LogF, Log, BodyRT >(log_f, log, true);

				return true;
			}else{
				optional< BodyRT > body_value(std::invoke(body));

				if constexpr(log_trait< Log >::has_body_finished){
					log.body_finished();
				}

				exec_log< LogF, Log, BodyRT >(log_f, log, body_value);

				return body_value;
			}
		}catch(...){
			if constexpr(log_trait< Log >::has_body_finished){
				log.body_finished();
			}

			log.set_body_exception(std::current_exception());

			auto body_value = optional< BodyRT >();
			exec_log< LogF, Log, BodyRT >(log_f, log, body_value);

			return body_value;
		}
	}


}


namespace logsys{


	/// \brief Add a log message without associated code block
	///
	/// Usage Example:
	///
	/// \code{.cpp}
	/// log([](your_log_type& os){ os << "your message"; });
	/// \endcode
	template < typename LogF >
	inline void log(LogF&& log_f)noexcept{
		static_assert(detail::is_extract_log_valid_v< LogF, detail::nobody_t >,
			"Can not extract Log type from first parameter of the log "
			"function. "
			"A valid log()-call without body must have the form: "
			"'logsys::log([](Log&){});' where Log is your Log type.");

		using log_type = detail::extract_log_t< LogF, detail::nobody_t >;

		static_assert(detail::is_simple_log_f< LogF, log_type >,
			"Argument log_f is not a valid log message function. "
			"A valid log()-call without body must have the form: "
			"'logsys::log([](Log&){});' where Log is your Log type.");

		::logsys::detail::log< LogF, log_type >(log_f);
	}

	/// \brief Add a log message with associated code block
	///
	/// Usage Example:
	///
	/// \code{.cpp}
	/// int result = log(
	///     [](your_log_type& os, int const* result){
	///         os << "your message";
	///         // output result if body did not throw
	///         if(result != nullptr) os << ": " << *result;
	///     }, []{
	///         // your code
	///         return 5;
	///     });
	/// \endcode
	template < typename LogF, typename Body >
	inline decltype(auto) log(LogF&& log_f, Body&& body){
		using body_return_type = detail::body_return_t< Body >;
		static_assert(
			detail::is_extract_log_valid_v< LogF, body_return_type >,
			"Can not extract Log type from first parameter of the log "
			"function. A valid log()-call with body must have the form: "
			"'logsys::log([](Log&){}, []{});' or "
			"'logsys::log([](Log&, "
			"logsys::optional< value_type > const& value){}, "
			"[]{ return value; });' where Log is your Log type.");

		using log_type = detail::extract_log_t< LogF, body_return_type >;

		return ::logsys::detail::log<
			LogF, log_type, Body, body_return_type >(log_f, body);
	}

	/// \brief Catch all exceptions
	///
	/// Call the function and catch all exceptions throwing by the function.
	/// The name is emited via error_log together with the exception message.
	///
	/// As function the usage of a Lambda function is possible, which captures
	/// all variables by reference. ([&]{/* ... */})
	///
	/// If the Lambda function does not return anything, result will be a bool,
	/// indicating with false whether an exception appeared. Otherwise, the
	/// result will be a type that is convertible to bool. If and only if the
	/// conversion becomes true, accessability to the function result using
	/// member-function result() is permitted. Otherwise, result() will throw
	/// a std::logic_error.
	///
	/// Usage Example:
	///
	/// \code{.cpp}
	/// optional< int > result = exception_catching_log(
	///     [](your_log_type& os){ os << "your message"; },
	///     []{
	///         // your code
	///         return 5;
	///     });
	/// \endcode
	template < typename LogF, typename Body >
	inline auto exception_catching_log(LogF&& log_f, Body&& body)noexcept{
		using body_return_type = detail::body_return_t< Body >;
		static_assert(
			detail::is_extract_log_valid_v< LogF, body_return_type >,
			"Can not extract Log type from first parameter of the "
			"exception_catching_log function. A valid "
			"exception_catching_log()-call must have the form: "
			"'logsys::exception_catching_log([](Log&){}, []{});' or "
			"'logsys::exception_catching_log("
			"[](Log&, logsys::optional< value_type > const& value){}, "
			"[]{ return value });' where Log is your Log type.");

		using log_type = detail::extract_log_t< LogF, body_return_type >;

		return ::logsys::detail::exception_catching_log<
			LogF, log_type, Body, body_return_type >(log_f, body);
	}



}


#endif
