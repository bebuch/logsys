//-----------------------------------------------------------------------------
// Copyright (c) 2015-2017 Benjamin Buch
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


namespace logsys{


	/// \brief A timed log type
	class stdlog{
	public:
		/// \brief Get a unique id for every message
		static std::size_t unique_id(){
			static std::atomic< std::size_t > next_id(0);
			return next_id++;
		}

		/// \brief Save start time
		stdlog():
			body_(false),
			exception_(false),
			id_(unique_id()),
			start_(std::chrono::system_clock::now())
			{}

		/// \brief Output ID and time block
		void pre(){
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
		}

		/// \brief Output exception indicator
		void post(){
			if(exception_) os_ << " (failed)";
			os_ << exception_text_;
		}

		/// \brief Set exception indicator to true
		void failed(){
			exception_ = true;
		}

		/// \brief Save exception message
		void set_exception(std::exception const& error){
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

			exception_text_ = " (exception catched: [" + error_type_name + "] "
				+ error.what() + ")";
		}

		/// \brief Save text for unknown exception
		void unknown_exception(){
			exception_text_ = " (unknown exception catched)";
		}

		/// \brief Set body indicator to true
		void have_body(){
			body_ = true;
		}

		/// \brief Output the combinded message to std::log
		void exec()const{
			std::clog << (io_tools::mask_non_print(os_.str()) + '\n');
		}

		/// \brief Forward every output to the message stream
		template < typename T >
		friend stdlog& operator<<(stdlog& log, T&& data){
			log.os_ << static_cast< T&& >(data);
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
