//-----------------------------------------------------------------------------
// Copyright (c) 2015-2017 Benjamin Buch
//
// https://github.com/bebuch/log
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _log__log_tag__hpp_INCLUDED_
#define _log__log_tag__hpp_INCLUDED_

#include "log_base.hpp"

#include <io_tools/time_to_string.hpp>
#include <io_tools/mask_non_print.hpp>

#include <atomic>


namespace log{


	/// \brief A timed log type
	class log_tag_base{
	public:
		/// \brief Get a unique id for every message
		static std::size_t unique_id(){
			static std::atomic< std::size_t > next_id(0);
			return next_id++;
		}

		/// \brief Save start time
		log_tag_base():
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
			auto error_type_name = [&error]->std::string{
				try{
					return boost::typeindex::type_id_runtime(value).pretty_name();
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
		friend log_tag_base& operator<<(log_tag_base& log, T&& data){
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


	/// \brief Disposer log type
	class log_tag: public log_base, protected log_tag_base{
	public:
		/// \copydoc log_tag_base::pre()
		void pre()override{
			log_tag_base::pre();
		}

		/// \copydoc log_tag_base::post()
		void post()override{
			log_tag_base::post();
		}

		/// \copydoc log_tag_base::failed()
		void failed()override{
			log_tag_base::failed();
		}

		/// \copydoc log_tag_base::set_exception()
		void set_exception(std::exception const& error)override{
			log_tag_base::set_exception(error);
		}

		/// \copydoc log_tag_base::unknown_exception()
		void unknown_exception()override{
			log_tag_base::unknown_exception();
		}

		/// \copydoc log_tag_base::have_body()
		void have_body()override{
			log_tag_base::have_body();
		}

		/// \copydoc log_tag_base::exec()
		void exec()const override{
			log_tag_base::exec();
		}

		/// \brief Forward every output to the message stream
		template < typename T >
		friend log_tag& operator<<(log_tag& log, T&& data){
			log.os() << static_cast< T&& >(data);
			return log;
		}


	protected:
		/// \brief The message stream
		std::ostream& os()override{
			return os_;
		}
	};


	namespace detail{


		inline std::unique_ptr< log_tag > log_base_default_factory(){
			return std::make_unique< log_tag >();
		}


	}


}


#endif
