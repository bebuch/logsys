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
	private:
		/// \brief Info about the body
		enum body{
			none,
			exists,
			failed_by_exception,
			catched_exception,
		};

	public:
		/// \brief Save start time
		stdlog()noexcept:
			id_(unique_id()),
			start_(std::chrono::system_clock::now())
			{ os_ << std::boolalpha; }

		/// \brief Output ID and time block
		void body_finished()noexcept{
			end_ = std::chrono::system_clock::now();
			body_ = body::exists;
		}

		/// \brief Save body exception
		void set_body_exception(std::exception_ptr error, bool rethrow)noexcept{
			body_exception_ = error;
			if(rethrow){
				body_ = body::failed_by_exception;
			}else{
				body_ = body::catched_exception;
			}
		}

		/// \brief Save log exception
		void set_log_exception(std::exception_ptr error)noexcept{
			log_exception_ = error;
		}

		/// \brief Output the combinded message to std::log
		void exec()const noexcept try{
			std::clog << make_log_line();
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

		std::string make_log_line()const{
			std::ostringstream os;

			os << std::setfill('0') << std::setw(6) << id_ << ' ';

			io_tools::time_to_string(os, start_);

			if(body_ != body::none){
				os << " ( " << std::setfill(' ') << std::setprecision(3)
					<< std::setw(12)
					<< std::chrono::duration< double, std::milli >(
							end_ - start_
						).count() << "ms ) ";
			}else{
				os << " ( no content     ) ";
			}

			if(log_exception_){
				os << "LOG EXCEPTION CATCHED: ";

				print_exception(os, log_exception_);

				os << "; Probably incomplete log message: '"
					<< io_tools::mask_non_print(os_.str()) << "'";
			}else{
				os << io_tools::mask_non_print(os_.str());
			}

			if(body_exception_){
				if(body_ != body::catched_exception){
					os << " (BODY EXCEPTION CATCHED: ";
				}else{
					os << " (BODY FAILED: ";
				}

				print_exception(os, body_exception_);

				os << ')';
			}

			os << '\n';

			return os.str();
		}

	protected:
		/// \brief Get a unique id for every message
		static std::size_t unique_id()noexcept{
			static std::atomic< std::size_t > next_id(0);
			return next_id++;
		}

		static void print_exception(
			std::ostringstream& os,
			std::exception_ptr exception
		){
			try{
				std::rethrow_exception(exception);
			}catch(std::exception const& error){
				os << '[';

				try{
					using boost::typeindex::type_id_runtime;
					os << type_id_runtime(error).pretty_name();
				}catch(std::exception const& e){
					using namespace std::literals::string_literals;
					os << "could not find type: " << e.what();
				}catch(...){
					os << "could not find type";
				}

				os << "] " << error.what();
			}catch(...){
				os << "unknown exception";
			}
		}


		/// \brief The message stream
		std::ostringstream os_;

		/// \brief The body indicator
		bool body_ = body::none;

		/// \brief Exception throw in body function
		std::exception_ptr body_exception_ = nullptr;

		/// \brief Exception throw in log function
		std::exception_ptr log_exception_ = nullptr;

		/// \brief The unique ID of this log message
		std::size_t id_;

		/// \brief Time point before associated code block is executed
		std::chrono::system_clock::time_point start_;

		/// \brief Time point after associated code block is executed
		std::chrono::system_clock::time_point end_;
	};


}


#endif
