//-----------------------------------------------------------------------------
// Copyright (c) 2018 Benjamin Buch
//
// https://github.com/bebuch/logsys
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _logsys__optional__hpp_INCLUDED_
#define _logsys__optional__hpp_INCLUDED_

#include <optional>


namespace logsys{


	/// \brief Optional for every lvalue type
	template < typename T >
	struct optional_lvalue_reference{
	public:
		/// \brief Default constructor
		constexpr optional_lvalue_reference(): value_(nullptr) {}

		/// \brief Constructor
		constexpr optional_lvalue_reference(T& value): value_(&value) {}


		/// \brief Accesses the contained value without check
		constexpr T const* operator->()const{
			return value_;
		}

		/// \brief Accesses the contained value without check
		constexpr T* operator->(){
			return value_;
		}

		/// \brief Accesses the contained value without check
		constexpr T const& operator*()const{
			return *value_;
		}

		/// \brief Accesses the contained value without check
		constexpr T& operator*(){
			return *value_;
		}


		/// \brief Checks whether *this contains a value
		constexpr bool has_value()const noexcept{
			return value_ != nullptr;
		}

		/// \brief Checks whether *this contains a value
		constexpr explicit operator bool()const noexcept{
			return has_value();
		}


		/// \brief Returns the contained value
		constexpr T& value(){
			if(has_value()){
				return *value_;
			}else{
				throw std::bad_optional_access();
			}
		}

		/// \brief Returns the contained value
		constexpr const T& value()const{
			if(has_value()){
				return *value_;
			}else{
				throw std::bad_optional_access();
			}
		}


		/// \brief Returns the contained value if available, another value
		///        otherwise
		template < typename U >
		constexpr T value_or(U&& default_value)const{
			return has_value()
				? value()
				: static_cast< T >(static_cast< U&& >(default_value));
		}


	private:
		T* value_;
	};

	/// \brief Optional for every rvalue type
	template < typename T >
	struct optional_rvalue_reference{
		/// \brief Default constructor
		constexpr optional_rvalue_reference(): value_(nullptr) {}

		/// \brief Constructor
		constexpr optional_rvalue_reference(T&& value): value_(&value) {}


		/// \brief Accesses the contained value without check
		constexpr T const* operator->()const{
			return value_;
		}

		/// \brief Accesses the contained value without check
		constexpr T* operator->(){
			return value_;
		}

		/// \brief Accesses the contained value without check
		constexpr T const& operator*()const{
			return *value_;
		}

		/// \brief Accesses the contained value without check
		constexpr T&& operator*(){
			return std::move(*value_);
		}


		/// \brief Checks whether *this contains a value
		constexpr bool has_value()const noexcept{
			return value_ != nullptr;
		}

		/// \brief Checks whether *this contains a value
		constexpr explicit operator bool()const noexcept{
			return has_value();
		}


		/// \brief Returns the contained value
		constexpr T&& value(){
			if(has_value()){
				return std::move(*value_);
			}else{
				throw std::bad_optional_access();
			}
		}

		/// \brief Returns the contained value
		constexpr T const& value()const{
			if(has_value()){
				return *value_;
			}else{
				throw std::bad_optional_access();
			}
		}


		/// \brief Returns the contained value if available, another value
		///        otherwise
		///
		/// \note Value is moved into return value!
		template < typename U >
		constexpr T value_or(U&& default_value){
			return has_value()
				? std::move(value())
				: static_cast< T >(static_cast< U&& >(default_value));
		}

		/// \brief Returns the contained value if available, another value
		///        otherwise
		template < typename U >
		constexpr T value_or(U&& default_value)const{
			return has_value()
				? value()
				: static_cast< T >(static_cast< U&& >(default_value));
		}


	private:
		T* value_;
	};


	namespace detail{


		template < typename T >
		struct optional_choose{
			using type = std::optional< T >;
		};

		template < typename T >
		struct optional_choose< T& >{
			using type = optional_lvalue_reference< T >;
		};

		template < typename T >
		struct optional_choose< T&& >{
			using type = optional_rvalue_reference< T >;
		};

		template <>
		struct optional_choose< void >{
			using type = bool;
		};


	}


	/// \brief Return type for exception_catching_log
	template < typename T >
	using optional = typename detail::optional_choose< T >::type;


}


#endif
