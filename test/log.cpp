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

#include "gtest/gtest.h"


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
		void exec()noexcept{}
		void set_body_exception(
			std::exception_ptr error, bool rethrow)noexcept{}
		void set_log_exception(std::exception_ptr error)noexcept{}
	};


	template struct test_log_object< log_01 >;

	template struct test_log_object< logsys::stdlog >;
	template struct test_log_object< logsys::stdlogb >;



	TEST(log, without_body){
		struct type{
			std::size_t i = 0;

			void exec()noexcept{
				EXPECT_EQ(i, 1);
				++i;
			}

			void set_body_exception(std::exception_ptr, bool)noexcept{
				EXPECT_TRUE(false);
			}

			void set_log_exception(std::exception_ptr)noexcept{
				EXPECT_TRUE(false);
			}
		};

		logsys::log([](type& t){
				EXPECT_EQ(t.i, 0);
				++t.i;
			});
	}

	TEST(log, without_body_log_exception){
		struct type{
			std::size_t i = 0;

			void exec()noexcept{
				EXPECT_EQ(i, 2);
				++i;
			}

			void set_body_exception(std::exception_ptr, bool)noexcept{
				EXPECT_TRUE(false);
			}

			void set_log_exception(std::exception_ptr error)noexcept{
				EXPECT_EQ(i, 1);
				++i;

				try{
					std::rethrow_exception(error);
				}catch(std::runtime_error const& e){
					using namespace std::literals::string_view_literals;
					EXPECT_EQ(std::string_view(e.what()), "message"sv);
				}catch(...){
					EXPECT_TRUE(false);
				}
			}
		};

		logsys::log([](type& t){
				EXPECT_EQ(t.i, 0);
				++t.i;
				throw std::runtime_error("message");
			});
	}

	TEST(log, with_void_body){
		struct type{
			std::size_t i = 0;

			void exec()noexcept{
				EXPECT_EQ(i, 1);
				++i;
			}

			void set_body_exception(std::exception_ptr, bool)noexcept{
				EXPECT_TRUE(false);
			}

			void set_log_exception(std::exception_ptr)noexcept{
				EXPECT_TRUE(false);
			}
		};

		bool body_executed = false;
		logsys::log([](type& t){
				EXPECT_EQ(t.i, 0);
				++t.i;
			}, [&body_executed]{
				EXPECT_FALSE(body_executed);
				body_executed = true;
			});
		EXPECT_TRUE(body_executed);
	}

	TEST(log, with_void_body_result_in_log){
		struct type{
			std::size_t i = 0;

			void exec()noexcept{
				EXPECT_EQ(i, 1);
				++i;
			}

			void set_body_exception(std::exception_ptr, bool)noexcept{
				EXPECT_TRUE(false);
			}

			void set_log_exception(std::exception_ptr)noexcept{
				EXPECT_TRUE(false);
			}
		};

		bool body_executed = false;
		logsys::log([](type& t, bool success){
				EXPECT_EQ(t.i, 0);
				++t.i;
				EXPECT_TRUE(success);
			}, [&body_executed]{
				EXPECT_FALSE(body_executed);
				body_executed = true;
			});
		EXPECT_TRUE(body_executed);
	}

	TEST(log, with_void_body_log_exception){
		struct type{
			std::size_t i = 0;

			void exec()noexcept{
				EXPECT_EQ(i, 2);
				++i;
			}

			void set_body_exception(std::exception_ptr, bool)noexcept{
				EXPECT_TRUE(false);
			}

			void set_log_exception(std::exception_ptr error)noexcept{
				EXPECT_EQ(i, 1);
				++i;

				try{
					std::rethrow_exception(error);
				}catch(std::runtime_error const& e){
					using namespace std::literals::string_view_literals;
					EXPECT_EQ(std::string_view(e.what()), "message"sv);
				}catch(...){
					EXPECT_TRUE(false);
				}
			}
		};

		bool body_executed = false;
		logsys::log([](type& t){
				EXPECT_EQ(t.i, 0);
				++t.i;
				throw std::runtime_error("message");
			}, [&body_executed]{
				EXPECT_FALSE(body_executed);
				body_executed = true;
			});
		EXPECT_TRUE(body_executed);
	}

	TEST(log, with_void_body_exception){
		struct type{
			std::size_t i = 0;

			void exec()noexcept{
				EXPECT_EQ(i, 2);
				++i;
			}

			void set_body_exception(
				std::exception_ptr error,
				bool rethrow
			)noexcept{
				EXPECT_EQ(i, 0);
				++i;

				EXPECT_TRUE(rethrow);

				try{
					std::rethrow_exception(error);
				}catch(std::runtime_error const& e){
					using namespace std::literals::string_view_literals;
					EXPECT_EQ(std::string_view(e.what()), "message"sv);
				}catch(...){
					EXPECT_TRUE(false);
				}
			}

			void set_log_exception(std::exception_ptr)noexcept{
				EXPECT_TRUE(false);
			}
		};

		bool body_executed = false;
		try{
			logsys::log([](type& t){
					EXPECT_EQ(t.i, 1);
					++t.i;
				}, [&body_executed]{
					EXPECT_FALSE(body_executed);
					body_executed = true;
					throw std::runtime_error("message");
				});
		}catch(std::runtime_error const& e){
			using namespace std::literals::string_view_literals;
			EXPECT_EQ(std::string_view(e.what()), "message"sv);
		}catch(...){
			EXPECT_TRUE(false);
		}
		EXPECT_TRUE(body_executed);
	}

	TEST(log, with_void_body_exception_and_result_in_log){
		struct type{
			std::size_t i = 0;

			void exec()noexcept{
				EXPECT_EQ(i, 2);
				++i;
			}

			void set_body_exception(
				std::exception_ptr error,
				bool rethrow
			)noexcept{
				EXPECT_EQ(i, 0);
				++i;

				EXPECT_TRUE(rethrow);

				try{
					std::rethrow_exception(error);
				}catch(std::runtime_error const& e){
					using namespace std::literals::string_view_literals;
					EXPECT_EQ(std::string_view(e.what()), "message"sv);
				}catch(...){
					EXPECT_TRUE(false);
				}
			}

			void set_log_exception(std::exception_ptr)noexcept{
				EXPECT_TRUE(false);
			}
		};

		bool body_executed = false;
		try{
			logsys::log([](type& t, bool success){
					EXPECT_EQ(t.i, 1);
					++t.i;
					EXPECT_FALSE(success);
				}, [&body_executed]{
					EXPECT_FALSE(body_executed);
					body_executed = true;
					throw std::runtime_error("message");
				});
		}catch(std::runtime_error const& e){
			using namespace std::literals::string_view_literals;
			EXPECT_EQ(std::string_view(e.what()), "message"sv);
		}catch(...){
			EXPECT_TRUE(false);
		}
		EXPECT_TRUE(body_executed);
	}

	TEST(log, with_void_body_both_exception){
		struct type{
			std::size_t i = 0;

			void exec()noexcept{
				EXPECT_EQ(i, 3);
				++i;
			}

			void set_body_exception(
				std::exception_ptr error,
				bool rethrow
			)noexcept{
				EXPECT_EQ(i, 0);
				++i;

				EXPECT_TRUE(rethrow);

				try{
					std::rethrow_exception(error);
				}catch(std::runtime_error const& e){
					using namespace std::literals::string_view_literals;
					EXPECT_EQ(std::string_view(e.what()), "body message"sv);
				}catch(...){
					EXPECT_TRUE(false);
				}
			}

			void set_log_exception(std::exception_ptr error)noexcept{
				EXPECT_EQ(i, 2);
				++i;

				try{
					std::rethrow_exception(error);
				}catch(std::runtime_error const& e){
					using namespace std::literals::string_view_literals;
					EXPECT_EQ(std::string_view(e.what()), "log message"sv);
				}catch(...){
					EXPECT_TRUE(false);
				}
			}
		};

		bool body_executed = false;
		try{
			logsys::log([](type& t){
					EXPECT_EQ(t.i, 1);
					++t.i;
					throw std::runtime_error("log message");
				}, [&body_executed]{
					EXPECT_FALSE(body_executed);
					body_executed = true;
					throw std::runtime_error("body message");
				});
		}catch(std::runtime_error const& e){
			using namespace std::literals::string_view_literals;
			EXPECT_EQ(std::string_view(e.what()), "body message"sv);
		}catch(...){
			EXPECT_TRUE(false);
		}
		EXPECT_TRUE(body_executed);
	}


	TEST(log, with_int_body){
		struct type{
			std::size_t i = 0;

			void exec()noexcept{
				EXPECT_EQ(i, 1);
				++i;
			}

			void set_body_exception(std::exception_ptr, bool)noexcept{
				EXPECT_TRUE(false);
			}

			void set_log_exception(std::exception_ptr)noexcept{
				EXPECT_TRUE(false);
			}
		};

		bool body_executed = false;
		int result = logsys::log([](type& t){
				EXPECT_EQ(t.i, 0);
				++t.i;
			}, [&body_executed]{
				EXPECT_FALSE(body_executed);
				body_executed = true;
				return 555;
			});
		EXPECT_TRUE(body_executed);
		EXPECT_EQ(result, 555);
	}

	TEST(log, with_int_body_result_in_log){
		struct type{
			std::size_t i = 0;

			void exec()noexcept{
				EXPECT_EQ(i, 1);
				++i;
			}

			void set_body_exception(std::exception_ptr, bool)noexcept{
				EXPECT_TRUE(false);
			}

			void set_log_exception(std::exception_ptr)noexcept{
				EXPECT_TRUE(false);
			}
		};

		bool body_executed = false;
		int result = logsys::log(
			[](type& t, std::optional< int > value){
				EXPECT_EQ(t.i, 0);
				++t.i;
				EXPECT_TRUE(static_cast< bool >(value));
				if(value){
					EXPECT_EQ(*value, 555);
				}
			}, [&body_executed]{
				EXPECT_FALSE(body_executed);
				body_executed = true;
				return 555;
			});
		EXPECT_TRUE(body_executed);
		EXPECT_EQ(result, 555);
	}

	TEST(log, with_int_body_log_exception){
		struct type{
			std::size_t i = 0;

			void exec()noexcept{
				EXPECT_EQ(i, 2);
				++i;
			}

			void set_body_exception(std::exception_ptr, bool)noexcept{
				EXPECT_TRUE(false);
			}

			void set_log_exception(std::exception_ptr error)noexcept{
				EXPECT_EQ(i, 1);
				++i;

				try{
					std::rethrow_exception(error);
				}catch(std::runtime_error const& e){
					using namespace std::literals::string_view_literals;
					EXPECT_EQ(std::string_view(e.what()), "message"sv);
				}catch(...){
					EXPECT_TRUE(false);
				}
			}
		};

		bool body_executed = false;
		int result = logsys::log([](type& t){
				EXPECT_EQ(t.i, 0);
				++t.i;
				throw std::runtime_error("message");
			}, [&body_executed]{
				EXPECT_FALSE(body_executed);
				body_executed = true;
				return 555;
			});
		EXPECT_TRUE(body_executed);
		EXPECT_EQ(result, 555);
	}

	TEST(log, with_int_body_exception){
		struct type{
			std::size_t i = 0;

			void exec()noexcept{
				EXPECT_EQ(i, 2);
				++i;
			}

			void set_body_exception(
				std::exception_ptr error,
				bool rethrow
			)noexcept{
				EXPECT_EQ(i, 0);
				++i;

				EXPECT_TRUE(rethrow);

				try{
					std::rethrow_exception(error);
				}catch(std::runtime_error const& e){
					using namespace std::literals::string_view_literals;
					EXPECT_EQ(std::string_view(e.what()), "message"sv);
				}catch(...){
					EXPECT_TRUE(false);
				}
			}

			void set_log_exception(std::exception_ptr)noexcept{
				EXPECT_TRUE(false);
			}
		};

		bool body_executed = false;
		try{
			int result = logsys::log([](type& t){
					EXPECT_EQ(t.i, 1);
					++t.i;
				}, [&body_executed]()->int{
					EXPECT_FALSE(body_executed);
					body_executed = true;
					throw std::runtime_error("message");
				});
			(void)result;
		}catch(std::runtime_error const& e){
			using namespace std::literals::string_view_literals;
			EXPECT_EQ(std::string_view(e.what()), "message"sv);
		}catch(...){
			EXPECT_TRUE(false);
		}
		EXPECT_TRUE(body_executed);
	}

	TEST(log, with_int_body_exception_and_result_in_log){
		struct type{
			std::size_t i = 0;

			void exec()noexcept{
				EXPECT_EQ(i, 2);
				++i;
			}

			void set_body_exception(
				std::exception_ptr error,
				bool rethrow
			)noexcept{
				EXPECT_EQ(i, 0);
				++i;

				EXPECT_TRUE(rethrow);

				try{
					std::rethrow_exception(error);
				}catch(std::runtime_error const& e){
					using namespace std::literals::string_view_literals;
					EXPECT_EQ(std::string_view(e.what()), "message"sv);
				}catch(...){
					EXPECT_TRUE(false);
				}
			}

			void set_log_exception(std::exception_ptr)noexcept{
				EXPECT_TRUE(false);
			}
		};

		bool body_executed = false;
		try{
			int result = logsys::log(
				[](type& t, std::optional< int > value){
					EXPECT_EQ(t.i, 1);
					++t.i;
					EXPECT_FALSE(static_cast< bool >(value));
				}, [&body_executed]()->int{
					EXPECT_FALSE(body_executed);
					body_executed = true;
					throw std::runtime_error("message");
				});
			(void)result;
		}catch(std::runtime_error const& e){
			using namespace std::literals::string_view_literals;
			EXPECT_EQ(std::string_view(e.what()), "message"sv);
		}catch(...){
			EXPECT_TRUE(false);
		}
		EXPECT_TRUE(body_executed);
	}

	TEST(log, with_int_body_both_exception){
		struct type{
			std::size_t i = 0;

			void exec()noexcept{
				EXPECT_EQ(i, 3);
				++i;
			}

			void set_body_exception(
				std::exception_ptr error,
				bool rethrow
			)noexcept{
				EXPECT_EQ(i, 0);
				++i;

				EXPECT_TRUE(rethrow);

				try{
					std::rethrow_exception(error);
				}catch(std::runtime_error const& e){
					using namespace std::literals::string_view_literals;
					EXPECT_EQ(std::string_view(e.what()), "body message"sv);
				}catch(...){
					EXPECT_TRUE(false);
				}
			}

			void set_log_exception(std::exception_ptr error)noexcept{
				EXPECT_EQ(i, 2);
				++i;

				try{
					std::rethrow_exception(error);
				}catch(std::runtime_error const& e){
					using namespace std::literals::string_view_literals;
					EXPECT_EQ(std::string_view(e.what()), "log message"sv);
				}catch(...){
					EXPECT_TRUE(false);
				}
			}
		};

		bool body_executed = false;
		try{
			int result = logsys::log([](type& t){
					EXPECT_EQ(t.i, 1);
					++t.i;
					throw std::runtime_error("log message");
				}, [&body_executed]()->int{
					EXPECT_FALSE(body_executed);
					body_executed = true;
					throw std::runtime_error("body message");
				});
			(void)result;
		}catch(std::runtime_error const& e){
			using namespace std::literals::string_view_literals;
			EXPECT_EQ(std::string_view(e.what()), "body message"sv);
		}catch(...){
			EXPECT_TRUE(false);
		}
		EXPECT_TRUE(body_executed);
	}


}
