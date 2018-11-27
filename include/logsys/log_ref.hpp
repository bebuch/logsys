//-----------------------------------------------------------------------------
// Copyright (c) 2018 Benjamin Buch
//
// https://github.com/bebuch/logsys
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _logsys__tool__log_ref__hpp_INCLUDED_
#define _logsys__tool__log_ref__hpp_INCLUDED_

#include "log_base.hpp"


namespace logsys{


	/// \brief Base class that forwards log members to a log_base instance
	class log_ref{
	public:
		/// \brief Add a line to the log
		template < typename LogF >
		void log(LogF&& f)const{
			ref_.log(static_cast< LogF&& >(f));
		}

		/// \brief Add a line to the log with linked code block
		template < typename LogF, typename Body >
		decltype(auto) log(LogF&& f, Body&& body)const{
			return ref_.log(
				static_cast< LogF&& >(f), static_cast< Body&& >(body));
		}

		/// \brief Add a line to the log with linked code block and catch all
		///        exceptions
		template < typename LogF, typename Body >
		auto exception_catching_log(LogF&& f, Body&& body)const{
			return ref_.exception_catching_log(
				static_cast< LogF&& >(f), static_cast< Body&& >(body));
		}


		/// \brief Get the log prefix message
		std::string log_prefix()const{
			return ref_.log_prefix();
		}


	protected:
		log_ref(log_base const& ref)
			: ref_(ref) {}


	private:
		/// \brief Reference to a log_base
		log_base const& ref_;
	};


}


#endif
