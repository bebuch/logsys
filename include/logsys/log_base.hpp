//-----------------------------------------------------------------------------
// Copyright (c) 2015-2017 Benjamin Buch
//
// https://github.com/bebuch/logsys
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _logsys__log_base__hpp_INCLUDED_
#define _logsys__log_base__hpp_INCLUDED_

#include <ostream>
#include <memory>
#include <functional>


namespace logsys{


	/// \brief Base class for dynamic log tag classes
	class log_base{
	public:
		/// \brief Assign your log object maker to this variable
		static std::function< std::unique_ptr< log_base >() > factory;


		/// \brief Destructor
		virtual ~log_base(){}


		/// \brief Called immediate before message output
		virtual void pre(){}

		/// \brief Called immediate after message output
		virtual void post(){}

		/// \brief Called after post() if an exception is active
		virtual void failed(){}

		/// \brief Called if an std::exception derived is active
		virtual void set_exception(std::exception const&){}

		/// \brief Called if an not std::exception derived is active
		virtual void unknown_exception(){}

		/// \brief Called if a code block is associated with the log
		virtual void have_body(){}

		/// \brief Called after all work is done
		///
		/// Output your log message now.
		virtual void exec()const{}

		/// \brief Output operator overload
		template < typename T >
		friend log_base& operator<<(log_base& log, T&& data){
			log.os() << static_cast< T&& >(data);
			return log;
		}

	protected:
		/// \brief Provide an output stream for operator<<()
		virtual std::ostream& os() = 0;
	};


}


#endif
