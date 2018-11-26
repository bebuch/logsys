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

#include <functional>
#include <exception>
#include <memory>


namespace logsys{


	template < typename T >
	constexpr bool is_unique_ptr = false;

	template < typename T >
	constexpr bool is_unique_ptr< std::unique_ptr< T > > = true;


	/// \brief Type trait for log types
	template < typename Log >
	struct log_trait{
		/// \brief true if Log has a exec member function without arguments,
		///        otherwise false
		static constexpr bool has_exec = detail::is_valid< Log >(
			[](auto& x)->decltype((void)x.exec()){});

		static_assert(has_exec,
			"Log has no member function .exec()noexcept.");

		/// \brief true if exec() is nothrow callable, false otherwise
		static constexpr bool is_exec_noexcept =
			noexcept(std::declval< Log >().exec());

		static_assert(is_exec_noexcept,
			"Log member function .exec()noexcept must be nothrow callable.");


		/// \brief true if Log has a set_body_exception member function with one
		///        argument of type std::exception_ptr, false otherwise
		static constexpr bool has_set_body_exception = detail::is_valid< Log >(
			[](auto& x)->decltype(
				(void)x.set_body_exception(std::declval< std::exception_ptr >())
			){});

		static_assert(has_set_body_exception,
			"Log has no member function "
			".set_body_exception(std::exception_ptr)noexcept.");

		/// \brief true if set_body_exception() is nothrow callable, false
		///        otherwise
		static constexpr bool is_set_body_exception_noexcept =
			noexcept(std::declval< Log >()
				.set_body_exception(std::declval< std::exception_ptr >()));

		static_assert(is_set_body_exception_noexcept,
			"Log member function .set_body_exception(std::exception_ptr) must "
			"be nothrow callable.");


		/// \brief true if Log has a set_log_exception member function with one
		///        argument of type std::exception_ptr, false otherwise
		static constexpr bool has_set_log_exception = detail::is_valid< Log >(
			[](auto& x)->decltype(
				(void)x.set_log_exception(std::declval< std::exception_ptr >())
			){});

		static_assert(has_set_log_exception,
			"Log has no member function "
			".set_log_exception(std::exception_ptr)noexcept.");

		/// \brief true if set_log_exception() is nothrow callable, false
		///        otherwise
		static constexpr bool is_set_log_exception_noexcept =
			noexcept(std::declval< Log >()
				.set_log_exception(std::declval< std::exception_ptr >()));

		static_assert(is_set_log_exception_noexcept,
			"Log member function .set_log_exception(std::exception_ptr) must "
			"be nothrow callable.");


		/// \brief true if Log has a body_finished member function without arguments,
		///        otherwise false
		static constexpr bool has_body_finished = detail::is_valid< Log >(
			[](auto& x)->decltype((void)x.body_finished()){});

		/// \brief true if has_body_finished is false, or body_finished() is nothrow callable,
		///        false otherwise
		static constexpr bool is_body_finished_noexcept = []{
				if constexpr(has_body_finished){
					return noexcept(std::declval< Log >().body_finished());
				}else{
					return true;
				}
			}();

		static_assert(is_body_finished_noexcept,
			"Log member function .body_finished() must be nothrow callable.");


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
					using factory_type =
						std::invoke_result_t< decltype(&Log::factory) >;
					if constexpr(is_unique_ptr< factory_type >){
						using log_type = typename factory_type::element_type;
						return std::is_base_of_v< log_type, Log >;
					}else{
						return false;
					}
				}else{
					return true;
				}
			}();

		static_assert(has_factory_valid_return_type,
			"static Log member function ::factory() must return a "
			"std::unique_ptr< T > where T is equal or derived from Log.");

		/// \brief true if has_factory is false, or factory() is nothrow
		///        callable, false otherwise
		static constexpr bool is_factory_noexcept =
			[]{
				if constexpr(has_factory){
					return std::is_nothrow_invocable_v<
						decltype(&Log::factory) >;
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
