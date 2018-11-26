//-----------------------------------------------------------------------------
// Copyright (c) 2015-2018 Benjamin Buch
//
// https://github.com/bebuch/logsys
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _logsys__stdlogd__hpp_INCLUDED_
#define _logsys__stdlogd__hpp_INCLUDED_

#include "stdlogb.hpp"
#include "stdlog.hpp"


namespace logsys{


	/// \brief The standard dynamic log class
	class stdlogd: public stdlogb, protected stdlog{
	public:
		/// \copydoc stdlog::body_finished()
		void body_finished()noexcept override{
			stdlog::body_finished();
		}

		/// \copydoc stdlog::set_body_exception()
		void set_body_exception(std::exception_ptr error)noexcept override{
			stdlog::set_body_exception(error);
		}

		/// \copydoc stdlog::set_log_exception()
		void set_log_exception(std::exception_ptr error)noexcept override{
			stdlog::set_log_exception(error);
		}

		/// \copydoc stdlog::exec()
		void exec()const noexcept override{
			stdlog::exec();
		}


	protected:
		/// \brief The message stream
		std::ostream& os()noexcept override{
			return os_;
		}
	};


}


#endif
