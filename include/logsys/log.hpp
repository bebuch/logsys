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

#include "detail/log_impl.hpp"


namespace logsys{


	struct no_manipulator{
		template < typename Log >
		constexpr void operator()(Log&)const noexcept{}
	};


	/// \brief Add a log message without associated code block
	///
	/// Usage Example:
	///
	/// \code{.cpp}
	/// log([](your_log_type& os){ os << "your message"; });
	/// \endcode
	template < typename LogF >
	inline void log(LogF&& log_f)noexcept{
		detail::log(no_manipulator(), log_f);
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
		return detail::log(no_manipulator(), log_f, body);
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
		return detail::exception_catching_log(no_manipulator(), log_f, body);
	}



}


#endif
