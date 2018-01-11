//-----------------------------------------------------------------------------
// Copyright (c) 2015-2018 Benjamin Buch
//
// https://github.com/bebuch/logsys
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _logsys__stdlog__hpp_INCLUDED_
#define _logsys__stdlog__hpp_INCLUDED_

#include <io_tools/time_to_string.hpp>
#include <io_tools/mask_non_print.hpp>

#include <boost/type_index.hpp>

#include <atomic>
#include <iomanip>


namespace logsys{


	/// \brief A timed log type
	class stdlog{
	public:
		/// \brief Get a unique id for every message
		static std::size_t unique_id()noexcept{
			static std::atomic< std::size_t > next_id(0);
			return next_id++;
		}

		/// \brief Save start time
		stdlog()noexcept:
			body_(false),
			exception_(false),
			id_(unique_id()),
			start_(std::chrono::system_clock::now())
			{ os_ << std::boolalpha; }

		/// \brief Output ID and time block
		void pre()noexcept try{
			auto end = std::chrono::system_clock::now();

			os_ << std::setfill('0') << std::setw(6) << id_ << ' ';

			io_tools::time_to_string(os_, start_);

			if(body_){
				os_ << " ( " << std::setfill(' ') << std::setprecision(3)
					<< std::setw(12)
					<< std::chrono::duration< double, std::milli >(
							end - start_
						).count() << "ms ) ";
			}else{
				os_ << " ( no content     ) ";
			}
		}catch(std::exception const& e){
			std::cerr << "terminate with exception in stdlog.pre(): "
				<< e.what() << std::endl;
			std::terminate();
		}catch(...){
			std::cerr << "terminate with unknown exception in stdlog.pre()"
				<< std::endl;
			std::terminate();
		}

		/// \brief Output exception indicator
		void post()noexcept try{
			if(exception_) os_ << " (FAILED)";
			os_ << exception_text_;
		}catch(std::exception const& e){
			std::cerr << "terminate with exception in stdlog.post(): "
				<< e.what() << std::endl;
			std::terminate();
		}catch(...){
			std::cerr << "terminate with unknown exception in stdlog.post()"
				<< std::endl;
			std::terminate();
		}

		/// \brief Set exception indicator to true
		void body_failed()noexcept{
			exception_ = true;
		}

		/// \brief Save exception message
		void set_exception(std::exception const& error)noexcept try{
			auto error_type_name = [&error]()->std::string{
				try{
					using boost::typeindex::type_id_runtime;
					return type_id_runtime(error).pretty_name();
				}catch(std::exception const& e){
					using namespace std::literals::string_literals;
					return "could not find type: "s + e.what();
				}catch(...){
					return "could not find type";
				}
			}();

			exception_text_ = " (EXCEPTION CATCHED: [" + error_type_name + "] "
				+ error.what() + ")";
		}catch(std::exception const& e){
			std::cerr << "terminate with exception in stdlog.set_exception(): "
				<< e.what() << std::endl;
			std::terminate();
		}catch(...){
			std::cerr << "terminate with unknown exception in "
				"stdlog.set_exception()" << std::endl;
			std::terminate();
		}

		/// \brief Save text for unknown exception
		void unknown_exception()noexcept try{
			exception_text_ = " (UNKNOWN EXCEPTION CATCHED)";
		}catch(std::exception const& e){
			std::cerr << "terminate with exception in "
				"stdlog.unknown_exception(): " << e.what() << std::endl;
			std::terminate();
		}catch(...){
			std::cerr << "terminate with unknown exception in "
				"stdlog.unknown_exception()" << std::endl;
			std::terminate();
		}

		/// \brief Set body indicator to true
		void have_body()noexcept{
			body_ = true;
		}

		/// \brief Output the combinded message to std::log
		void exec()const noexcept try{
			std::clog << (io_tools::mask_non_print(os_.str()) + '\n');
		}catch(std::exception const& e){
			std::cerr << "terminate with exception in stdlog.exec(): "
				<< e.what() << std::endl;
			std::terminate();
		}catch(...){
			std::cerr << "terminate with unknown exception in stdlog.exec()"
				<< std::endl;
			std::terminate();
		}

		/// \brief Forward every output to the message stream
		template < typename T >
		friend stdlog& operator<<(stdlog& log, T&& data){
			using type = std::remove_cv_t< std::remove_reference_t< T > >;
			if constexpr(
				std::is_same_v< type, char > ||
				std::is_same_v< type, signed char > ||
				std::is_same_v< type, unsigned char >
			){
				log.os_ << static_cast< int >(data);
			}else{
				log.os_ << static_cast< T&& >(data);
			}
			return log;
		}

	protected:
		/// \brief The message stream
		std::ostringstream os_;

		/// \brief The exception text
		std::string exception_text_;

		/// \brief The body indicator
		bool body_;

		/// \brief The exception indicator
		bool exception_;

		/// \brief The unique ID of this log message
		std::size_t id_;

		/// \brief Time point before associated code block is executed
		std::chrono::system_clock::time_point const start_;
	};


}


#endif
