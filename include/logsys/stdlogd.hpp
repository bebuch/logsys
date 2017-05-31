//-----------------------------------------------------------------------------
// Copyright (c) 2015-2017 Benjamin Buch
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
		/// \copydoc stdlog::pre()
		void pre()noexcept override{
			stdlog::pre();
		}

		/// \copydoc stdlog::post()
		void post()noexcept override{
			stdlog::post();
		}

		/// \copydoc stdlog::failed()
		void failed()noexcept override{
			stdlog::failed();
		}

		/// \copydoc stdlog::set_exception()
		void set_exception(std::exception const& error)noexcept override{
			stdlog::set_exception(error);
		}

		/// \copydoc stdlog::unknown_exception()
		void unknown_exception()noexcept override{
			stdlog::unknown_exception();
		}

		/// \copydoc stdlog::have_body()
		void have_body()noexcept override{
			stdlog::have_body();
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
