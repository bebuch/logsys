//-----------------------------------------------------------------------------
// Copyright (c) 2015-2018 Benjamin Buch
//
// https://github.com/bebuch/logsys
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#include <logsys/stdlog.hpp>
#include <logsys/stdlogb.hpp>
#include <logsys/log.hpp>


namespace{


	using logsys::optional;
	using logsys::optional_lvalue_reference;
	using logsys::optional_rvalue_reference;
	using logsys::detail::is_valid;


	template < typename Log >
	struct test_log_object{
		// check body return values

		static_assert(std::is_same_v<
			decltype(logsys::log(
				std::declval< void(Log&) >(),
				std::declval< void() >())),
			void >);

		static_assert(std::is_same_v<
			decltype(logsys::exception_catching_log(
				std::declval< void(Log&) >(),
				std::declval< void() >())),
			bool >);


		static_assert(std::is_same_v<
			decltype(logsys::log(
				std::declval< void(Log&) >(),
				std::declval< int() >())),
			int >);

		static_assert(std::is_same_v<
			decltype(logsys::exception_catching_log(
				std::declval< void(Log&) >(),
				std::declval< int() >())),
			std::optional< int > >);


		static_assert(std::is_same_v<
			decltype(logsys::log(
				std::declval< void(Log&) >(),
				std::declval< int*() >())),
			int* >);

		static_assert(std::is_same_v<
			decltype(logsys::exception_catching_log(
				std::declval< void(Log&) >(),
				std::declval< int*() >())),
			std::optional< int* > >);


		static_assert(std::is_same_v<
			decltype(logsys::log(
				std::declval< void(Log&) >(),
				std::declval< int&() >())),
			int& >);

		static_assert(std::is_same_v<
			decltype(logsys::exception_catching_log(
				std::declval< void(Log&) >(),
				std::declval< int&() >())),
			optional_lvalue_reference< int > >);


		static_assert(std::is_same_v<
			decltype(logsys::log(
				std::declval< void(Log&) >(),
				std::declval< int&&() >())),
			int&& >);

		static_assert(std::is_same_v<
			decltype(logsys::exception_catching_log(
				std::declval< void(Log&) >(),
				std::declval< int&&() >())),
			optional_rvalue_reference< int > >);


		// check log message provider parameters

		static_assert(is_valid< void(Log&) >(
				[](auto& logf)->decltype((void)
					logsys::log(logf)
			){}));

		static_assert(is_valid< void(Log&, bool) >(
				[](auto& logf)->decltype((void)
					logsys::log(logf)
			){}));

		static_assert(is_valid< void(Log&) >(
				[](auto& logf)->decltype((void)
					logsys::log(
						logf, std::declval< void() >())
			){}));

		static_assert(is_valid< void(Log&) >(
				[](auto& logf)->decltype((void)
					logsys::exception_catching_log(
						logf, std::declval< void() >())
			){}));

		static_assert(is_valid< void(Log&, bool) >(
				[](auto& logf)->decltype((void)
					logsys::log(
						logf, std::declval< void() >())
			){}));

		static_assert(is_valid< void(Log&, bool) >(
				[](auto& logf)->decltype((void)
					logsys::exception_catching_log(
						logf, std::declval< void() >())
			){}));

		static_assert(is_valid< void(Log&, bool const&) >(
				[](auto& logf)->decltype((void)
					logsys::log(
						logf, std::declval< void() >())
			){}));

		static_assert(is_valid< void(Log&, bool const&) >(
				[](auto& logf)->decltype((void)
					logsys::exception_catching_log(
						logf, std::declval< void() >())
			){}));


		static_assert(is_valid< void(Log&) >(
				[](auto& logf)->decltype((void)
					logsys::log(
						logf, std::declval< int() >())
			){}));

		static_assert(is_valid< void(Log&) >(
				[](auto& logf)->decltype((void)
					logsys::exception_catching_log(
						logf, std::declval< int() >())
			){}));

		static_assert(is_valid< void(Log&, std::optional< int >) >(
				[](auto& logf)->decltype((void)
					logsys::log(
						logf, std::declval< int() >())
			){}));

		static_assert(is_valid< void(Log&, std::optional< int >) >(
				[](auto& logf)->decltype((void)
					logsys::exception_catching_log(
						logf, std::declval< int() >())
			){}));

		static_assert(is_valid< void(Log&, std::optional< int > const&) >(
				[](auto& logf)->decltype((void)
					logsys::log(
						logf, std::declval< int() >())
			){}));

		static_assert(is_valid< void(Log&, std::optional< int > const&) >(
				[](auto& logf)->decltype((void)
					logsys::exception_catching_log(
						logf, std::declval< int() >())
			){}));


		static_assert(is_valid< void(Log&) >(
				[](auto& logf)->decltype((void)
					logsys::log(
						logf, std::declval< int&() >())
			){}));

		static_assert(is_valid< void(Log&) >(
				[](auto& logf)->decltype((void)
					logsys::exception_catching_log(
						logf, std::declval< int&() >())
			){}));

		static_assert(is_valid<
				void(Log&, optional_lvalue_reference< int >) >(
					[](auto& logf)->decltype((void)
						logsys::log(
							logf, std::declval< int&() >())
				){}));

		static_assert(is_valid<
				void(Log&, optional_lvalue_reference< int >) >(
					[](auto& logf)->decltype((void)
						logsys::exception_catching_log(
							logf, std::declval< int&() >())
				){}));

		static_assert(is_valid<
				void(Log&, optional_lvalue_reference< int > const&) >(
					[](auto& logf)->decltype((void)
						logsys::log(
							logf, std::declval< int&() >())
				){}));

		static_assert(is_valid<
				void(Log&, optional_lvalue_reference< int > const&) >(
					[](auto& logf)->decltype((void)
						logsys::exception_catching_log(
							logf, std::declval< int&() >())
				){}));


		static_assert(is_valid< void(Log&) >(
				[](auto& logf)->decltype((void)
					logsys::log(
						logf, std::declval< int&&() >())
			){}));

		static_assert(is_valid< void(Log&) >(
				[](auto& logf)->decltype((void)
					logsys::exception_catching_log(
						logf, std::declval< int&&() >())
			){}));

		static_assert(is_valid<
				void(Log&, optional_rvalue_reference< int >) >(
					[](auto& logf)->decltype((void)
						logsys::log(
							logf, std::declval< int&&() >())
				){}));

		static_assert(is_valid<
				void(Log&, optional_rvalue_reference< int >) >(
					[](auto& logf)->decltype((void)
						logsys::exception_catching_log(
							logf, std::declval< int&&() >())
				){}));

		static_assert(is_valid<
				void(Log&, optional_rvalue_reference< int > const&) >(
					[](auto& logf)->decltype((void)
						logsys::log(
							logf, std::declval< int&&() >())
				){}));

		static_assert(is_valid<
				void(Log&, optional_rvalue_reference< int > const&) >(
					[](auto& logf)->decltype((void)
						logsys::exception_catching_log(
							logf, std::declval< int&&() >())
				){}));
	};


	struct log_01{
		void exec()noexcept;
		void set_body_exception(std::exception_ptr error)noexcept;
		void set_log_exception(std::exception_ptr error)noexcept;
	};


	template struct test_log_object< log_01 >;

	template struct test_log_object< logsys::stdlog >;
	template struct test_log_object< logsys::stdlogb >;


}
