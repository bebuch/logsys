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

#include "is_valid.hpp"

#include <string_view>
#include <type_traits>
#include <functional>
#include <iostream>
#include <optional>
#include <memory>


namespace logsys::detail{


	/// \brief Extract type of Function parameter
	template < typename Function >
	struct extract_log: extract_log< decltype(&Function::operator()) >{};

	/// \brief Implementation of extract_log_t
	template < typename Function >
	struct extract_log< Function& >: extract_log< Function >{};


	/// \brief Implementation of extract_log_t
	template < typename Log, typename R >
	struct extract_log< R(Log&) >{
		using type = Log;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename R >
	struct extract_log< R(*)(Log&) >{
		using type = Log;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename R, typename F >
	struct extract_log< R(F::*)(Log&) >{
		using type = Log;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename R, typename F >
	struct extract_log< R(F::*)(Log&) const >{
		using type = Log;
	};


	/// \brief Implementation of extract_log_t
	template < typename Log, typename T, typename R >
	struct extract_log< R(Log&, T const*) >{
		using type = Log;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename T, typename R >
	struct extract_log< R(*)(Log&, T const*) >{
		using type = Log;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename T, typename R, typename F >
	struct extract_log< R(F::*)(Log&, T const*) >{
		using type = Log;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename T, typename R, typename F >
	struct extract_log< R(F::*)(Log&, T const*) const >{
		using type = Log;
	};


#ifdef __cpp_noexcept_function_type
	/// \brief Implementation of extract_log_t
	template < typename Log, typename R >
	struct extract_log< R(Log&)noexcept >{
		using type = Log;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename R >
	struct extract_log< R(*)(Log&)noexcept >{
		using type = Log;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename R, typename F >
	struct extract_log< R(F::*)(Log&)noexcept >{
		using type = Log;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename R, typename F >
	struct extract_log< R(F::*)(Log&)const noexcept >{
		using type = Log;
	};


	/// \brief Implementation of extract_log_t
	template < typename Log, typename T, typename R >
	struct extract_log< R(Log&, T const*)noexcept >{
		using type = Log;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename T, typename R >
	struct extract_log< R(*)(Log&, T const*)noexcept >{
		using type = Log;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename T, typename R, typename F >
	struct extract_log< R(F::*)(Log&, T const*)noexcept >{
		using type = Log;
	};

	/// \brief Implementation of extract_log_t
	template < typename Log, typename T, typename R, typename F >
	struct extract_log< R(F::*)(Log&, T const*)const noexcept >{
		using type = Log;
	};
#endif


	/// \brief Extract type of first parameter from log function
	template < typename Function >
	using extract_log_t = typename extract_log< Function >::type;


	/// \brief Type trait for log types
	template < typename Log >
	struct log_trait{
		/// \brief true if Log has a exec member function without arguments,
		///        otherwise false
		static constexpr bool has_exec = is_valid< Log >(
			[](auto& x)->decltype((void)x.exec()){});

		static_assert(has_exec,
			"Log has no member function .exec().");

		/// \brief true if exec() is nothrow callable, false otherwise
		static constexpr bool is_exec_noexcept =
			noexcept(std::declval< Log >().exec());

		static_assert(is_exec_noexcept,
			"Log member function .exec() must be nothrow callable.");


		/// \brief true if Log has a set_exception member function with one
		///        lvalue argument of type std::exception, otherwise false
		static constexpr bool has_set_exception = is_valid< Log >(
			[](auto& x)->decltype(
				(void)x.set_exception(std::declval< std::exception const& >())
			){});

		static_assert(has_set_exception,
			"Log has no member function "
			".set_exception(std::exception const&).");

		/// \brief true if set_exception() is nothrow callable, false otherwise
		static constexpr bool is_set_exception_noexcept =
			noexcept(std::declval< Log >()
				.set_exception(std::declval< std::exception const& >()));

		static_assert(is_set_exception_noexcept,
			"Log member function .set_exception() must be nothrow callable.");


		/// \brief true if Log has a unknown_exception member function without
		///        arguments, otherwise false
		static constexpr bool has_unknown_exception = is_valid< Log >(
			[](auto& x)->decltype((void)x.unknown_exception()){});

		static_assert(has_unknown_exception,
			"Log has no member function .unknown_exception().");

		/// \brief true if unknown_exception() is nothrow callable, false
		///        otherwise
		static constexpr bool is_unknown_exception_noexcept =
			noexcept(std::declval< Log >().unknown_exception());

		static_assert(is_unknown_exception_noexcept,
			"Log member function .unknown_exception() must be nothrow "
			"callable.");


		/// \brief true if Log has a pre member function without arguments,
		///        otherwise false
		static constexpr bool has_pre = is_valid< Log >(
			[](auto& x)->decltype((void)x.pre()){});

		/// \brief true if has_pre is false, or pre() is nothrow callable,
		///        false otherwise
		static constexpr bool is_pre_noexcept = []{
			if constexpr(has_pre){
				return noexcept(std::declval< Log >().pre());
			}else{ return true; } }();

		static_assert(is_pre_noexcept,
			"Log member function .pre() must be nothrow callable.");


		/// \brief true if Log has a post member function without arguments,
		///        otherwise false
		static constexpr bool has_post = is_valid< Log >(
			[](auto& x)->decltype((void)x.post()){});

		/// \brief true if has_post is false, or post() is nothrow callable,
		///        false otherwise
		static constexpr bool is_post_noexcept = []{
			if constexpr(has_post){
				return noexcept(std::declval< Log >().post());
			}else{ return true; } }();

		static_assert(is_post_noexcept,
			"Log member function .post() must be nothrow callable.");


		/// \brief true if Log has a body_failed member function without
		///        arguments, otherwise false
		static constexpr bool has_body_failed = is_valid< Log >(
			[](auto& x)->decltype((void)x.body_failed()){});

		/// \brief true if has_body_failed is false, or body_failed() is
		///        nothrow callable, false otherwise
		static constexpr bool is_body_failed_noexcept = []{
			if constexpr(has_body_failed){
				return noexcept(std::declval< Log >().body_failed());
			}else{ return true; } }();

		static_assert(is_body_failed_noexcept,
			"Log member function .body_failed() must be nothrow callable.");


		/// \brief true if Log has a have_body member function
		///        without arguments, otherwise false
		static constexpr bool has_have_body = is_valid< Log >(
			[](auto& x)->decltype((void)x.have_body()){});

		/// \brief true if has_have_body is false, or have_body() is nothrow
		///        callable, false otherwise
		static constexpr bool is_have_body_noexcept = []{
			if constexpr(has_have_body){
				return noexcept(std::declval< Log >().have_body());
			}else{ return true; } }();

		static_assert(is_have_body_noexcept,
			"Log member function .have_body() must be nothrow callable.");


		/// \brief true if Log has a static factory function without arguments
		///        returning a std::unique_ptr< Log >, otherwise false
		static constexpr bool has_factory = is_valid< Log >(
			[](auto& x)->decltype((void)
				std::remove_reference_t< decltype(x) >::factory()){});

		/// \brief true if has_factory is false, or factory() return a
		///        std::unique_ptr< Log >, false otherwise
		static constexpr bool has_factory_valid_return_type = []{
			if constexpr(has_factory){
				return is_valid< Log >([](auto& x)->decltype((void)
					static_cast< std::unique_ptr<
						std::remove_reference_t< decltype(x) > > >(
							std::remove_reference_t< decltype(x) >::factory()
						)){});
			}else{ return true; } }();

		static_assert(has_factory_valid_return_type,
			"static Log member function ::factory() must return a "
			"std::unique_ptr< Log >.");

		/// \brief true if has_factory is false, or factory() is nothrow
		///        callable, false otherwise
		static constexpr bool is_factory_noexcept = []{
			if constexpr(has_factory){
				return noexcept(std::declval< Log >().factory());
			}else{ return true; } }();

		static_assert(is_factory_noexcept,
			"static Log member function ::factory() must be nothrow callable.");


		static_assert(has_factory ||
			std::is_nothrow_default_constructible_v< Log >,
			"Log must either have a static member function ::factory() or "
			"be nothrow default constructible");
	};


	template < typename Log >
	constexpr bool is_extractable_log_f =
		is_valid< Log >([](auto& x)->decltype((void)
			std::declval< extract_log_t<
				std::remove_reference_t< decltype(x) > > >()){});

	template < typename Body >
	using result_as_ptr_t =
		std::remove_reference_t< std::invoke_result_t< Body& > > const*;

	template < typename LogF, typename Log >
	constexpr bool is_simple_log_f = std::is_invocable_v< LogF&, Log& >;

	template < typename LogF, typename Log, typename Body >
	constexpr bool is_extended_log_f = std::is_invocable_v< LogF&, Log&,
		result_as_ptr_t< Body > >;

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
		LogF& log_f, std::unique_ptr< Log >& log, T const* result = nullptr
	)noexcept{
		if constexpr(log_trait< Log >::has_pre){ log->pre(); }

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

		if constexpr(log_trait< Log >::has_post){ log->post(); }

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
				exec_log(log_f, log,
					static_cast< result_as_ptr_t< Body > >(
						std::addressof(result)));
			}
			return result;
		}
	}catch(...){
		if constexpr(log_trait< Log >::has_body_failed){ log->body_failed(); }

		if constexpr(is_simple_log_f< LogF, Log >){
			exec_log(log_f, log);
		}else{
			exec_log(log_f, log,
				static_cast< result_as_ptr_t< Body > >(nullptr));
		}

		throw;
	}

	/// \brief Call the associated code block and catch exceptions
	///
	///   - If no exception appears:
	///       1. return with associated code block result as std::optional
	///   - If an exception appears:
	///       1. exception is derived from std::exception
	///           - yes: Call log->set_exception(exception)
	///           - no: Call log->unknown_exception()
	///       2. return with an empty std::optional
	template < typename Body, typename Log >
	inline auto exec_exception_catching_body(
		Body& body,
		std::unique_ptr< Log >& log
	)noexcept{
		using body_type = std::invoke_result_t< Body& >;
		constexpr auto is_void = std::is_void_v< body_type >;

		using return_type = std::conditional_t<
			std::is_reference_v< body_type >,
			std::reference_wrapper< std::remove_reference_t< body_type > >,
			body_type >;

		try{
			if constexpr(is_void){
				std::invoke(body);
				return true;
			}else{
				return std::optional< return_type >(std::invoke(body));
			}
		}catch(std::exception const& error){
			log->set_exception(error);
		}catch(...){
			log->unknown_exception();
		}

		if constexpr(is_void){
			return false;
		}else{
			return std::optional< return_type >();
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
		static_assert(detail::is_extractable_log_f< LogF >,
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
	inline decltype(auto) log(LogF&& log_f, Body&& body){
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

		if constexpr(detail::log_trait< Log >::has_have_body){
			log->have_body();
		}

		return detail::exec_body(log_f, body, log);
	}

	/// \copydoc log< Log, LogF, Body >(LogF&&, Body&&)
	template < typename LogF, typename Body >
	inline decltype(auto) log(LogF&& log_f, Body&& body){
		static_assert(detail::is_extractable_log_f< LogF >,
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
	/// std::optional< int > result = exception_catching_log(
	///     [](your_log_tag_type& os){ os << "your message"; },
	///     []{
	///         // your code
	///         return 5;
	///     });
	/// \endcode
	template < typename Log, typename LogF, typename Body >
	inline auto exception_catching_log(LogF&& log_f, Body&& body)noexcept{
		auto log = detail::make_log< Log >();

		if constexpr(detail::log_trait< Log >::has_have_body){
			log->have_body();
		}

		auto result = detail::exec_exception_catching_body(body, log);
		if constexpr(detail::is_simple_log_f< LogF, Log >){
			detail::exec_log(log_f, log);
		}else{
			detail::exec_log(log_f, log,
				static_cast< detail::result_as_ptr_t< Body > >(
					result ? std::addressof(*result) : nullptr));
		}
		return result;
	}

	/// \copydoc exception_catching_log< Log, LogF, Body >(LogF&&, Body&&)
	template < typename LogF, typename Body >
	inline auto exception_catching_log(LogF&& log_f, Body&& body)noexcept{
		static_assert(detail::is_extractable_log_f< LogF >,
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
