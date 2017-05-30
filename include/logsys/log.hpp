//-----------------------------------------------------------------------------
// Copyright (c) 2015-2017 Benjamin Buch
//
// https://github.com/bebuch/logsys
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _logsys__log__hpp_INCLUDED_
#define _logsys__log__hpp_INCLUDED_

#include <boost/hana.hpp>

#include <optional>
#include <memory>


namespace logsys{


	namespace detail{


		namespace hana = boost::hana;


		/// \brief Implementation of extract_log_t
		template < typename Function >
		struct extract_log_from_function;

		/// \brief Implementation of extract_log_t
		template < typename F, typename R, typename Log >
		struct extract_log_from_function< R(F::*)(Log&) >{
			using type = Log;
		};

		/// \brief Implementation of extract_log_t
		template < typename F, typename R, typename Log >
		struct extract_log_from_function< R(F::*)(Log&)const >{
			using type = Log;
		};

		/// \brief Implementation of extract_log_t
		template < typename F, typename R, typename Log >
		struct extract_log_from_function< R(F::*)(Log&)volatile >{
			using type = Log;
		};

		/// \brief Implementation of extract_log_t
		template < typename F, typename R, typename Log >
		struct extract_log_from_function< R(F::*)(Log&)volatile const >{
			using type = Log;
		};

		/// \brief Extract type of first Function parameter
		template < typename Function >
		struct extract_log{
			using type =
				typename extract_log_from_function<
					decltype(&Function::operator())
				>::type;
		};

		/// \brief Implementation of extract_log_t
		template < typename Function >
		struct extract_log< Function& >{
			using type = typename extract_log< Function >::type;
		};

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

		/// \copydoc extract_log
		template < typename Function >
		using extract_log_t = typename extract_log< Function >::type;


		/// \brief Output the log message
		///
		///   1. Call log->pre() if it exists
		///   2. Call the log function: f(log)
		///   3. Call log->post() if it exists
		///   4. Call log->exec()
		template < typename F, typename Log >
		inline void exec_log(F& f, std::unique_ptr< Log >& log)noexcept{
			auto has_pre = hana::is_valid(
					[](auto& x)->decltype((void)x->pre()){}
				)(log);

			auto has_post = hana::is_valid(
					[](auto& x)->decltype((void)x->post()){}
				)(log);

			if constexpr(has_pre){
				static_assert(noexcept(log->pre()),
					"In 'log([](Log& os){ ... })', 'os.pre()' must be a "
					"nothrow callable expression.");
				log->pre();
			}

			f(*log);

			if constexpr(has_post){
				static_assert(noexcept(log->post()),
					"In 'log([](Log& os){ ... })', 'os.post()' must be a "
					"nothrow callable expression.");
				log->post();
			}

			static_assert(noexcept(log->exec()),
				"In 'log([](Log& os){ ... })', 'os.exec()' must be a "
				"nothrow callable expression.");
			log->exec();
		}


		/// \brief Call the associated code block
		///
		///   - If no exception appears:
		///       1. return with associated code block result
		///   - If an exception appears:
		///       1. Call log->failed() if it exists
		///       2. Call exec_log
		///       3. rethrow the exception
		template < typename LogF, typename Body, typename Log >
		inline decltype(auto) exec_body(
			LogF&& log_f,
			Body&& body,
			std::unique_ptr< Log >& log
		)try{
			return body();
		}catch(...){
			auto has_failed = hana::is_valid(
					[](auto& x)->decltype((void)x->failed()){}
				)(log);

			if constexpr(has_failed){
				static_assert(noexcept(log->failed()),
					"In 'log([](Log& os){ ... })', 'os.failed()' must be a "
					"nothrow callable expression.");
				log->failed();
			}

			exec_log(log_f, log);

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
			Body&& body,
			std::unique_ptr< Log >& log
		)noexcept{
			constexpr auto is_void = std::is_void_v< decltype(body()) >;

			using body_type = decltype(body());
			using return_type = std::conditional_t<
				std::is_reference_v< body_type >,
				std::reference_wrapper< std::remove_reference_t< body_type > >,
				body_type >;

			try{
				if constexpr(is_void){
					body();
					return true;
				}else{
					return std::optional< return_type >(body());
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


		template < typename T >
		struct is_unique_ptr: std::false_type{};

		template < typename T >
		struct is_unique_ptr< std::unique_ptr< T > >: std::true_type{};

		template < typename T >
		constexpr bool is_unique_ptr_v = is_unique_ptr< T >::value;


		/// \brief Construct a new log object
		///
		///   - Log::factory exists
		///       - yes: construct by calling Log::factory()
		///       - no: construct by calling standard constructor
		template < typename Log >
		auto make_log()noexcept{
			constexpr auto has_factory = hana::is_valid(
				[](auto t)->decltype((void)decltype(t)::type::factory()){}
			);

			if constexpr(has_factory(hana::type_c< Log >)){
				static_assert(is_unique_ptr_v< decltype(Log::factory()) >,
					"Log::factory() must return a std::unique_ptr");


				static_assert(noexcept(Log::factory()),
					"Log::factory() must be a nothrow callable expression.");

				return Log::factory();
			}else{
				return std::unique_ptr< Log >();
			}
		}

		/// \brief Check if a type has a exec() function
		constexpr auto has_exec = hana::is_valid(
			[](auto& x)->decltype((void)x->exec()){}
		);

		/// \brief Check if a type has a have_body() function
		constexpr auto has_have_body = hana::is_valid(
			[](auto& x)->decltype((void)x->have_body()){}
		);


	}


	/// \brief Add a log message without associated code block
	///
	/// Usage Example:
	///
	/// \code{.cpp}
	/// log([](your_log_tag_type& os){ os << "your message"; });
	/// \endcode
	template < typename LogF >
	inline void log(LogF&& log_f)noexcept{
		using log_t = detail::extract_log_t< LogF >;

		auto log = detail::make_log< log_t >();

		static_assert(
			detail::has_exec(log),
			"In 'log([](Log& os){ ... })', 'os.exec()' must be a callable "
			"expression."
		);

		detail::exec_log(log_f, log);
	}

	/// \brief Add a log message with associated code block
	///
	/// Usage Example:
	///
	/// \code{.cpp}
	/// int result = log([](your_log_tag_type& os){ os << "your message"; }, []{
	///      // your code
	///      return 5;
	/// });
	/// \endcode
	template < typename LogF, typename Body >
	inline decltype(auto) log(LogF&& log_f, Body&& body){
		namespace hana = boost::hana;

		using log_t = detail::extract_log_t< LogF >;

		auto log = detail::make_log< log_t >();

		static_assert(detail::has_exec(log),
			"In 'log([](Log& os){ ... }, []{ ... })', 'os.exec()' must be a "
			"callable expression."
		);

		if constexpr(detail::has_have_body(log)){
			static_assert(noexcept(log->have_body()),
				"In 'log([](Log& os){ ... })', 'os.have_body()' "
				"must be a nothrow callable expression.");

			log->have_body();
		}

		if constexpr(hana::traits::is_void(hana::type_c< decltype(body()) >)){
			detail::exec_body(log_f, body, log);
			detail::exec_log(log_f, log);
		}else{
			decltype(auto) result = detail::exec_body(log_f, body, log);
			detail::exec_log(log_f, log);
			return result;
		}
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
	template < typename LogF, typename Body >
	inline auto exception_catching_log(LogF&& log_f, Body&& body)noexcept{
		namespace hana = boost::hana;

		using log_t = detail::extract_log_t< LogF >;


		constexpr auto has_set_exception = hana::is_valid(
			[](auto& x)->decltype(
				(void)x->set_exception(std::declval< std::exception >())
			){}
		);

		constexpr auto has_unknown_exception = hana::is_valid(
			[](auto& x)->decltype((void)x->unknown_exception()){}
		);


		auto log = detail::make_log< log_t >();


		static_assert(
			detail::has_exec(log),
			"In 'exception_catching_log([](Log& os){ ... }, []{ ... })', "
			"'os.exec()' must be a callable expression."
		);

		static_assert(
			has_set_exception(log),
			"In 'exception_catching_log([](Log& os){ ... }, []{ ... })', "
			"'os.set_exception(std::exception())' must be a "
			"callable expression."
		);

		static_assert(
			has_unknown_exception(log),
			"In 'exception_catching_log([](Log& os){ ... }, []{ ... })', "
			"'os.unknown_exception()' must be a callable expression."
		);

		static_assert(
			noexcept(log->set_exception(std::declval< std::exception >())),
			"In 'log([](Log& os){ ... })', "
			"'os.set_exception(std::exception())' "
			"must be a nothrow callable expression.");

		static_assert(noexcept(log->unknown_exception()),
			"In 'log([](Log& os){ ... })', 'os.unknown_exception()' "
			"must be a nothrow callable expression.");

		if constexpr(detail::has_have_body(log)){
			static_assert(noexcept(log->have_body()),
				"In 'log([](Log& os){ ... })', 'os.have_body()' "
				"must be a nothrow callable expression.");
			log->have_body();
		}

		auto result = detail::exec_exception_catching_body(body, log);
		detail::exec_log(log_f, log);
		return result;
	}



}


#endif
