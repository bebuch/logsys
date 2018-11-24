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

	template < typename LogF, typename Log, typename Body >
	constexpr bool is_extended_log_f = std::is_invocable_v< LogF&, Log&,
		optional< std::invoke_result_t< Body& > > const& >;

	template < typename LogF, typename Log, typename Body >
	constexpr bool is_valid_log_f = is_simple_log_f< LogF, Log >
		|| is_extended_log_f< LogF, Log, Body >;


	/// \brief Output the log message
	///
	///   1. Call log->pre() if it exists
	///   2. Call the log function: f(log)
	///   3. Call log->post() if it exists
	///   4. Call log->exec()
	template < typename LogF, typename Log, typename T = void >
	inline void exec_log(
		LogF& log_f,
		std::unique_ptr< Log >& log,
		T const* result = nullptr
	)noexcept{
		if constexpr(log_trait< Log >::has_pre){
			log->pre();
		}

		try{
			if constexpr(std::is_void_v< T >){
				(void)result; // Silance GCC
				std::invoke(log_f, *log);
			}else{
				std::invoke(log_f, *log, result);
			}
		}catch(std::exception const& e){
			std::cerr << "ERROR: exception while executing log function: "
				<< e.what() << std::endl;
			*log << "<EXCEPTION WHILE LOGGING: " << e.what() << ">";
		}catch(...){
			std::cerr << "ERROR: unknown exception while executing log function"
				<< std::endl;
			*log << "<EXCEPTION WHILE LOGGING: unknown exception>";
		}

		if constexpr(log_trait< Log >::has_post){
			log->post();
		}

		log->exec();
	}


	/// \brief Call the associated code block
	///
	///   - If no exception appears:
	///       1. return with associated code block result
	///   - If an exception appears:
	///       1. Call log->body_failed() if it exists
	///       2. Call exec_log
	///       3. rethrow the exception
	template < typename LogF, typename Body, typename Log >
	inline decltype(auto) exec_body(
		LogF& log_f,
		Body& body,
		std::unique_ptr< Log >& log
	)try{
		if constexpr(std::is_void_v< std::invoke_result_t< Body& > >){
			std::invoke(body);
			exec_log(log_f, log);
		}else{
			decltype(auto) result = std::invoke(body);
			if constexpr(is_simple_log_f< LogF, Log >){
				exec_log(log_f, log);
			}else{
				exec_log(log_f, log, result);
			}
			return result;
		}
	}catch(...){
		if constexpr(log_trait< Log >::has_body_failed){
			log->body_failed();
		}

		if constexpr(is_simple_log_f< LogF, Log >){
			exec_log(log_f, log);
		}else{
			exec_log(log_f, log, std::invoke_result_t< Body& >());
		}

		throw;
	}

	/// \brief Call the associated code block and catch exceptions
	///
	///   - If no exception appears:
	///       1. return with associated code block result as optional
	///   - If an exception appears:
	///       1. exception is derived from std::exception
	///           - yes: Call log->set_exception(exception)
	///           - no: Call log->unknown_exception()
	///       2. return with an empty optional
	template < typename Body, typename Log >
	inline auto exec_exception_catching_body(
		Body& body,
		std::unique_ptr< Log >& log
	)noexcept{
		using body_type = std::invoke_result_t< Body& >;
		constexpr auto is_void = std::is_void_v< body_type >;

		try{
			if constexpr(is_void){
				std::invoke(body);
				return true;
			}else{
				return optional< body_type >(std::invoke(body));
			}
		}catch(std::exception const& error){
			log->set_exception(error);
		}catch(...){
			log->unknown_exception();
		}

		if constexpr(is_void){
			return false;
		}else{
			return optional< body_type >();
		}
	}


	/// \brief Construct a new log object
	///
	///   - Log::factory exists
	///       - yes: construct by calling Log::factory()
	///       - no: construct by calling standard constructor
	template < typename Log >
	auto make_log()noexcept{
		if constexpr(log_trait< Log >::has_factory){
			return Log::factory();
		}else{
			return std::unique_ptr< Log >();
		}
	}


}


namespace logsys{


	/// \brief Add a log message without associated code block
	///
	/// Usage Example:
	///
	/// \code{.cpp}
	/// log< your_log_tag_type >(
	///     [](your_log_tag_type& os){ os << "your message"; });
	/// \endcode
	///
	/// If the call of the log message function is not overloaded, the library
	/// can deduce the log type from the function parameter, so you don't need
	/// to name it explicitly:
	///
	/// \code{.cpp}
	/// log([](your_log_tag_type& os){ os << "your message"; });
	/// \endcode
	template < typename Log, typename LogF >
	inline void log(LogF&& log_f)noexcept{
		static_assert(detail::is_simple_log_f< LogF, Log >,
			"Argument log_f is not a valid log message function. "
			"A valid log()-call without body must have the form: "
			"'logsys::log< Log >([](Log&){});' where Log is your Log type. "
			"The explicit naming of Log as template argument is optional if"
			"and only if the call of the given argument is not overloaded.");

		auto log = detail::make_log< Log >();
		detail::exec_log(log_f, log);
	}

	/// \copydoc log< Log, LogF >(LogF&&)
	template < typename LogF >
	inline void log(LogF&& log_f)noexcept{
		static_assert(detail::is_extract_log_valid_v< LogF >,
			"Can not extract Log type from first parameter of the log "
			"function. "
			"A valid log()-call without body must have the form: "
			"'logsys::log< Log >([](Log&){});' where Log is your Log type. "
			"The explicit naming of Log as template argument is optional if"
			"and only if the call of the given argument is not overloaded.");

		::logsys::log< detail::extract_log_t< LogF > >(log_f);
	}


	/// \brief Add a log message with associated code block
	///
	/// Usage Example:
	///
	/// \code{.cpp}
	/// int result = log< your_log_tag_type >(
	///     [](your_log_tag_type& os, int const* result){
	///         os << "your message";
	///         // output result if body did not throw
	///         if(result != nullptr) os << ": " << *result;
	///     }, []{
	///         // your code
	///         return 5;
	///     });
	/// \endcode
	template < typename Log, typename LogF, typename Body >
	inline std::invoke_result_t< Body&& > log(LogF&& log_f, Body&& body){
		static_assert(std::is_invocable_v< Body& >,
			"Parameter body must be a callable without arguments.");

		using result_type = std::invoke_result_t< Body& >;

		static_assert(detail::is_valid_log_f< LogF, Log, Body >,
			"Argument log_f is not a valid log message function. "
			"A valid log()-call with body must have the form: "
			"'logsys::log< Log >([](Log&){}, []{});' or "
			"'logsys::log< Log >([](Log&, value_type const* value_ptr){}, "
			"[]{ return value; });' where Log is your Log type. "
			"The explicit naming of Log as template argument is optional if"
			"and only if the call of the given argument is not overloaded.");


		auto log = detail::make_log< Log >();

		static_assert(detail::is_simple_log_f< LogF, Log >
			|| !std::is_void_v< result_type >,
			"return type of the body function is void. Remove the second "
			"argument from your log message function.");

		if constexpr(log_trait< Log >::has_have_body){
			log->have_body();
		}

		return detail::exec_body(log_f, body, log);
	}

	/// \copydoc log< Log, LogF, Body >(LogF&&, Body&&)
	template < typename LogF, typename Body >
	inline std::invoke_result_t< Body&& > log(LogF&& log_f, Body&& body){
		static_assert(detail::is_extract_log_valid_v< LogF >,
			"Can not extract Log type from first parameter of the log function."
			"A valid log()-call with body must have the form: "
			"'logsys::log< Log >([](Log&){}, []{});' or "
			"'logsys::log< Log >([](Log&, value_type const* value_ptr){}, "
			"[]{ return value; });' where Log is your Log type. "
			"The explicit naming of Log as template argument is optional if"
			"and only if the call of the given argument is not overloaded.");

		return ::logsys::log< detail::extract_log_t< LogF > >(log_f, body);
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
	///     [](your_log_tag_type& os){ os << "your message"; },
	///     []{
	///         // your code
	///         return 5;
	///     });
	/// \endcode
	template < typename Log, typename LogF, typename Body >
	inline auto exception_catching_log(LogF&& log_f, Body&& body)noexcept
	-> std::conditional_t< std::is_void_v< std::invoke_result_t< Body&& > >,
		bool, optional< std::invoke_result_t< Body&& > > >
	{
		auto log = detail::make_log< Log >();

		if constexpr(log_trait< Log >::has_have_body){
			log->have_body();
		}

		auto result = detail::exec_exception_catching_body(body, log);
		if constexpr(detail::is_simple_log_f< LogF, Log >){
			detail::exec_log(log_f, log);
		}else{
			detail::exec_log(log_f, log, result);
		}
		return result;
	}

	/// \copydoc exception_catching_log< Log, LogF, Body >(LogF&&, Body&&)
	template < typename LogF, typename Body >
	inline auto exception_catching_log(LogF&& log_f, Body&& body)noexcept
	-> std::conditional_t< std::is_void_v< std::invoke_result_t< Body&& > >,
		bool, optional< std::invoke_result_t< Body&& > > >
	{
		static_assert(detail::is_extract_log_valid_v< LogF >,
			"Can not extract Log type from first parameter of the "
			"exception_catching_log function. A valid "
			"exception_catching_log()-call must have the form: "
			"'logsys::exception_catching_log< Log >([](Log&){}, []{});' or "
			"'logsys::exception_catching_log< Log >("
			"[](Log&, value_type const* value_ptr){}, "
			"[]{ return value });' where Log is your Log type. "
			"The explicit naming of Log as template argument is optional if"
			"and only if the call of the given argument is not overloaded.");

		return ::logsys::exception_catching_log
			< detail::extract_log_t< LogF > >(log_f, body);
	}



}


#endif
