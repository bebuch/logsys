//-----------------------------------------------------------------------------
// Copyright (c) 2015-2018 Benjamin Buch
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


namespace logsys{


	/// \brief Base class for dynamic log tag classes
	class [[gnu::visibility("default")]] stdlogb{
	public:
		/// \brief Create a stdlogb derived log object
		///
		/// Link against logsys/liblogsys.so and assign your log object creater
		/// function to stdlogb_factory_object.
		static std::unique_ptr< stdlogb > factory()noexcept;

		/// \brief Destructor
		virtual ~stdlogb()noexcept{}


		/// \brief Called after body function was executed
		virtual void body_finished()noexcept{}

		/// \brief Called if body function throw an exception
		virtual void set_body_exception(std::exception_ptr)noexcept{}

		/// \brief Called if log function throw an exception
		virtual void set_log_exception(std::exception_ptr)noexcept{}

		/// \brief Called after all work is done
		///
		/// Output your log message now.
		virtual void exec()const noexcept{}


		/// \brief Output operator overload
		template < typename T >
		friend stdlogb& operator<<(stdlogb& log, T&& data){
			using type = std::remove_cv_t< std::remove_reference_t< T > >;
			if constexpr(
				std::is_same_v< type, char > ||
				std::is_same_v< type, signed char > ||
				std::is_same_v< type, unsigned char >
			){
				log.os() << static_cast< int >(data);
			}else{
				log.os() << static_cast< T&& >(data);
			}
			return log;
		}

	protected:
		/// \brief Provide an output stream for operator<<()
		virtual std::ostream& os()noexcept = 0;
	};


}


#endif
