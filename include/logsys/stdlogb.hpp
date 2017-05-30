//-----------------------------------------------------------------------------
// Copyright (c) 2015-2017 Benjamin Buch
//
// https://github.com/bebuch/logsys
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _logsys__stdlogb__hpp_INCLUDED_
#define _logsys__stdlogb__hpp_INCLUDED_

#include <iostream>
#include <memory>
#include <functional>


namespace logsys{


	/// \brief Base class for dynamic log tag classes
	class stdlogb{
	public:
		/// \brief Assign your log object maker to this variable
		static std::function< std::unique_ptr< stdlogb >() > factory_object;

		static std::unique_ptr< stdlogb > factory()noexcept try{
			return factory_object();
		}catch(std::exception const& e){
			std::cerr << "terminate with exception in stdlogb factory: "
				<< e.what() << std::endl;
			std::exit(1);
		}catch(...){
			std::cerr << "terminate with unknown exception in stdlogb factory"
				<< std::endl;
			std::exit(1);
		}


		/// \brief Destructor
		virtual ~stdlogb()noexcept{}


		/// \brief Called immediate before message output
		virtual void pre()noexcept{}

		/// \brief Called immediate after message output
		virtual void post()noexcept{}

		/// \brief Called after post() if an exception is active
		virtual void failed()noexcept{}

		/// \brief Called if an std::exception derived is active
		virtual void set_exception(std::exception const&)noexcept{}

		/// \brief Called if an not std::exception derived is active
		virtual void unknown_exception()noexcept{}

		/// \brief Called if a code block is associated with the log
		virtual void have_body()noexcept{}

		/// \brief Called after all work is done
		///
		/// Output your log message now.
		virtual void exec()const noexcept{}

		/// \brief Output operator overload
		template < typename T >
		friend stdlogb& operator<<(stdlogb& log, T&& data)noexcept{
			log.os() << static_cast< T&& >(data);
			return log;
		}

	protected:
		/// \brief Provide an output stream for operator<<()
		virtual std::ostream& os()noexcept = 0;
	};


}


#endif
