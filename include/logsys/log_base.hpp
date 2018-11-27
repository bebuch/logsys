//-----------------------------------------------------------------------------
// Copyright (c) 2017-2018 Benjamin Buch
//
// https://github.com/bebuch/logsys
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _logsys__tool__log_base__hpp_INCLUDED_
#define _logsys__tool__log_base__hpp_INCLUDED_

#include "basic_log_base.hpp"
#include "stdlogb.hpp"

#include <string>


namespace logsys{


	/// \brief Base class that implementes log functions as members
	class log_base: public basic_log_base< log_base >{
	public:
		/// \brief Get the log prefix message
		template < typename Log >
		void manipulate(Log& log)const noexcept{
			log << log_prefix_; // TODO: This may throw
		}


		/// \brief Get the log prefix message
		std::string log_prefix()const{
			return log_prefix_;
		}


	protected:
		log_base(std::string const& log_prefix)
			: log_prefix_(log_prefix) {}

		log_base(std::string&& log_prefix)noexcept
			: log_prefix_(std::move(log_prefix)) {}


	private:
		/// \brief The log prefix message
		std::string log_prefix_;
	};


}


#endif
