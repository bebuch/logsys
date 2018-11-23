//-----------------------------------------------------------------------------
// Copyright (c) 2017-2018 Benjamin Buch
//
// https://github.com/bebuch/logsys
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _logsys__detail__log_trait__hpp_INCLUDED_
#define _logsys__detail__log_trait__hpp_INCLUDED_

#include "type_traits.hpp"

#include <exception>
#include <memory>


namespace logsys{


	/// \brief Type trait for log types
	template < typename Log >
	struct log_trait{
		/// \brief true if Log has a exec member function without arguments,
		///        otherwise false
		static constexpr bool has_exec = detail::is_valid< Log >(
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
		static constexpr bool has_set_exception = detail::is_valid< Log >(
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
		static constexpr bool has_unknown_exception = detail::is_valid< Log >(
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
		static constexpr bool has_pre = detail::is_valid< Log >(
			[](auto& x)->decltype((void)x.pre()){});

		/// \brief true if has_pre is false, or pre() is nothrow callable,
		///        false otherwise
		static constexpr bool is_pre_noexcept = []{
				if constexpr(has_pre){
					return noexcept(std::declval< Log >().pre());
				}else{
					return true;
				}
			}();

		static_assert(is_pre_noexcept,
			"Log member function .pre() must be nothrow callable.");


		/// \brief true if Log has a post member function without arguments,
		///        otherwise false
		static constexpr bool has_post = detail::is_valid< Log >(
			[](auto& x)->decltype((void)x.post()){});

		/// \brief true if has_post is false, or post() is nothrow callable,
		///        false otherwise
		static constexpr bool is_post_noexcept = []{
				if constexpr(has_post){
					return noexcept(std::declval< Log >().post());
				}else{
					return true;
				}
			}();

		static_assert(is_post_noexcept,
			"Log member function .post() must be nothrow callable.");


		/// \brief true if Log has a body_failed member function without
		///        arguments, otherwise false
		static constexpr bool has_body_failed = detail::is_valid< Log >(
			[](auto& x)->decltype((void)x.body_failed()){});

		/// \brief true if has_body_failed is false, or body_failed() is
		///        nothrow callable, false otherwise
		static constexpr bool is_body_failed_noexcept = []{
				if constexpr(has_body_failed){
					return noexcept(std::declval< Log >().body_failed());
				}else{
					return true;
				}
			}();

		static_assert(is_body_failed_noexcept,
			"Log member function .body_failed() must be nothrow callable.");


		/// \brief true if Log has a have_body member function
		///        without arguments, otherwise false
		static constexpr bool has_have_body = detail::is_valid< Log >(
			[](auto& x)->decltype((void)x.have_body()){});

		/// \brief true if has_have_body is false, or have_body() is nothrow
		///        callable, false otherwise
		static constexpr bool is_have_body_noexcept =
			[]{
				if constexpr(has_have_body){
					return noexcept(std::declval< Log >().have_body());
				}else{
					return true;
				}
			}();

		static_assert(is_have_body_noexcept,
			"Log member function .have_body() must be nothrow callable.");


		/// \brief true if Log has a static factory function without arguments
		///        returning a std::unique_ptr< Log >, otherwise false
		static constexpr bool has_factory = detail::is_valid< Log >(
			[](auto& x)->decltype((void)
				std::remove_reference_t< decltype(x) >::factory()){});

		/// \brief true if has_factory is false, or factory() return a
		///        std::unique_ptr< Log >, false otherwise
		static constexpr bool has_factory_valid_return_type =
			[]{
				if constexpr(has_factory){
					return detail::is_valid< Log >([](auto& x)->decltype((void)
						static_cast< std::unique_ptr<
							std::remove_reference_t< decltype(x) > > >(
								std::remove_reference_t< decltype(x) >
									::factory()
							)){});
				}else{
					return true;
				}
			}();

		static_assert(has_factory_valid_return_type,
			"static Log member function ::factory() must return a "
			"std::unique_ptr< Log >.");

		/// \brief true if has_factory is false, or factory() is nothrow
		///        callable, false otherwise
		static constexpr bool is_factory_noexcept =
			[]{
				if constexpr(has_factory){
					return noexcept(std::declval< Log >().factory());
				}else{
					return true;
				}
			}();

		static_assert(is_factory_noexcept,
			"static Log member function ::factory() must be nothrow callable.");


		static_assert(has_factory ||
			std::is_nothrow_default_constructible_v< Log >,
			"Log must either have a static member function ::factory() or "
			"be nothrow default constructible");
	};


}


#endif
