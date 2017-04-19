//-----------------------------------------------------------------------------
// Copyright (c) 2015-2017 Benjamin Buch
//
// https://github.com/bebuch/log
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _log__stdlogd__hpp_INCLUDED_
#define _log__stdlogd__hpp_INCLUDED_

#include "log_base.hpp"
#include "stdlog.hpp"


namespace log{


	/// \brief The standard dynamic log class
	class stdlogd: public log_base, protected stdlog{
	public:
		/// \copydoc stdlog::pre()
		void pre()override{
			stdlog::pre();
		}

		/// \copydoc stdlog::post()
		void post()override{
			stdlog::post();
		}

		/// \copydoc stdlog::failed()
		void failed()override{
			stdlog::failed();
		}

		/// \copydoc stdlog::set_exception()
		void set_exception(std::exception const& error)override{
			stdlog::set_exception(error);
		}

		/// \copydoc stdlog::unknown_exception()
		void unknown_exception()override{
			stdlog::unknown_exception();
		}

		/// \copydoc stdlog::have_body()
		void have_body()override{
			stdlog::have_body();
		}

		/// \copydoc stdlog::exec()
		void exec()const override{
			stdlog::exec();
		}

		/// \brief Forward every output to the message stream
		template < typename T >
		friend stdlogd& operator<<(stdlogd& log, T&& data){
			log.os() << static_cast< T&& >(data);
			return log;
		}


	protected:
		/// \brief The message stream
		std::ostream& os()override{
			return os_;
		}
	};


}


#endif
