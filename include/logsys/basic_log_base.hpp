//-----------------------------------------------------------------------------
// Copyright (c) 2017-2018 Benjamin Buch
//
// https://github.com/bebuch/logsys
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _logsys__tool__basic_log_base__hpp_INCLUDED_
#define _logsys__tool__basic_log_base__hpp_INCLUDED_

#include "log.hpp"


namespace logsys{


	/// \brief Call `manipulate()` on the given object as manipulate function
	///        while logging
	template < typename T >
	class manipulate_fn_forward{
	public:
		/// \brief Constructor
		constexpr manipulate_fn_forward(T const& ref)noexcept: ref_(ref) {}

		/// \brief Calls `ref_.manipulate(log)`
		template < typename Log >
		constexpr void operator()(Log& log)const noexcept{
			static_assert(
				detail::is_valid< Log >([](auto& log)->decltype((void)
					std::declval< T const& >()
					.template manipulate< Log >(log)){}),
				"T must have a member function .manipulate(log)const "
				"noexcept");

			static_assert(
				std::is_nothrow_invocable_v<
					decltype(&T::template manipulate< Log >), T const&, Log& >,
				"T.manipulate(Log&) must be nothrow invocable");

			ref_.template manipulate< Log >(log);
		}

	private:
		/// \brief Reference to the log manipulating object
		T const& ref_;
	};


	/// \brief Base class that implementes log functions as members
	///
	/// You have to set your derived class as template parameter. Your derived
	/// class must have an const member function `manipulate` that is callable
	/// with a reference to the log type.
	template < typename Derived >
	struct basic_log_base{
		/// \brief Add a line to the log
		template < typename LogF >
		void log(LogF&& log_f)const noexcept{
			detail::log(
				manipulate_fn_forward< Derived >(
					static_cast< Derived const& >(*this)), log_f);
		}

		/// \brief Add a line to the log with linked code block
		template < typename LogF, typename Body >
		decltype(auto) log(LogF&& log_f, Body&& body)const{
			return detail::log(
				manipulate_fn_forward< Derived >(
					static_cast< Derived const& >(*this)), log_f, body);
		}

		/// \brief Add a line to the log with linked code block and catch all
		///        exceptions
		template < typename LogF, typename Body >
		auto exception_catching_log(LogF&& log_f, Body&& body)const noexcept{
			return detail::exception_catching_log(
				manipulate_fn_forward< Derived >(
					static_cast< Derived const& >(*this)), log_f, body);
		}
	};


}


#endif
