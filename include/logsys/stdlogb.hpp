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
	class [[gnu::visibility("default")]] stdlog_base{
	public:
		/// \brief Destructor
		virtual ~stdlog_base()noexcept{}


		/// \brief Called after body function was executed
		virtual void body_finished()noexcept{}

		/// \brief Called if body function throw an exception
		virtual void set_body_exception(std::exception_ptr, bool)noexcept{}

		/// \brief Called if log function throw an exception
		virtual void set_log_exception(std::exception_ptr)noexcept{}

		/// \brief Called after all work is done
		///
		/// Output your log message now.
		virtual void exec()const noexcept{}


		/// \brief Output operator overload
		template < typename T >
		friend stdlog_base& operator<<(stdlog_base& log, T&& data){
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


	/// \brief Base class for dynamic log tag classes
	class [[gnu::visibility("default")]] stdlogb{
	public:
		/// \brief Create a stdlogb derived log object
		///
		/// Link against logsys/liblogsys.so and assign your log object creater
		/// function to stdlogb_factory_object.
		static std::unique_ptr< stdlog_base > factory()noexcept;


		/// \brief Construct a new derived log
		stdlogb()noexcept: derived_(factory()) {}


		/// \brief Called after body function was executed
		void body_finished()noexcept{
			derived_->body_finished();
		}

		/// \brief Called if body function throw an exception
		void set_body_exception(std::exception_ptr error, bool rethrow)noexcept{
			derived_->set_body_exception(error, rethrow);
		}

		/// \brief Called if log function throw an exception
		void set_log_exception(std::exception_ptr error)noexcept{
			derived_->set_log_exception(error);
		}

		/// \brief Called after all work is done
		///
		/// Output your log message now.
		void exec()const noexcept{
			derived_->exec();
		}


		/// \brief Output operator overload
		template < typename T >
		friend stdlogb& operator<<(stdlogb& log, T&& data){
			*log.derived_ << static_cast< T&& >(data);
			return log;
		}

	protected:
		/// \brief The actual log object
		std::unique_ptr< stdlog_base > derived_;
	};


}


#endif
