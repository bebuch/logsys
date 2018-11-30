//-----------------------------------------------------------------------------
// Copyright (c) 2018 Benjamin Buch
//
// https://github.com/bebuch/logsys
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#include <logsys/detail/extract_log_t.hpp>


namespace {


	using logsys::detail::nobody_t;
	struct body_t;

	using op_nobody_t = std::optional< nobody_t >;
	using op_body_t = std::optional< body_t >;

	template < typename T >
	using opl = logsys::optional_lvalue_reference< T >;

	template < typename T >
	using opr = logsys::optional_rvalue_reference< T >;

	template < typename BodyRT, typename RTP >
	constexpr bool valid_rt_v =
		logsys::detail::is_valid_body_return_type_parameter< BodyRT, RTP >;

	template < typename LogF, typename BodyRT >
	using log_t = logsys::detail::extract_log_t< LogF, BodyRT >;

	template < typename LogF, typename BodyRT >
	constexpr bool valid_v =
		logsys::detail::is_extract_log_valid_v< LogF, BodyRT >;


	template < typename T >
	constexpr bool is_int_v = std::is_same_v< T, int >;


	static_assert( valid_rt_v< void, bool >);
	static_assert(!valid_rt_v< void, bool& >);
	static_assert( valid_rt_v< void, bool const >);
	static_assert( valid_rt_v< void, bool const& >);
	static_assert(!valid_rt_v< void, bool&& >);
	static_assert(!valid_rt_v< void, bool const&& >);

	static_assert( valid_rt_v< int, std::optional< int > >);
	static_assert(!valid_rt_v< int, std::optional< int >& >);
	static_assert( valid_rt_v< int, std::optional< int > const >);
	static_assert( valid_rt_v< int, std::optional< int > const& >);
	static_assert(!valid_rt_v< int, std::optional< int >&& >);
	static_assert(!valid_rt_v< int, std::optional< int > const&& >);

	static_assert( valid_rt_v< int&, opl< int > >);
	static_assert(!valid_rt_v< int&, opl< int >& >);
	static_assert( valid_rt_v< int&, opl< int > const >);
	static_assert( valid_rt_v< int&, opl< int > const& >);
	static_assert(!valid_rt_v< int&, opl< int >&& >);
	static_assert(!valid_rt_v< int&, opl< int > const&& >);

	static_assert( valid_rt_v< int&&, opr< int > >);
	static_assert(!valid_rt_v< int&&, opr< int >& >);
	static_assert( valid_rt_v< int&&, opr< int > const >);
	static_assert( valid_rt_v< int&&, opr< int > const& >);
	static_assert(!valid_rt_v< int&&, opr< int >&& >);
	static_assert(!valid_rt_v< int&&, opr< int > const&& >);


	struct A;


	constexpr void valid_fn_t1(int&){}
	constexpr void valid_fn_n1(int&)noexcept{}

	struct valid_object_t1{
		constexpr void operator()(int&){}
	};

	struct valid_object_t2{
		constexpr void operator()(int&)const{}
	};

	struct valid_object_n1{
		constexpr void operator()(int&)noexcept{}
	};

	struct valid_object_n2{
		constexpr void operator()(int&)const noexcept{}
	};


	static_assert(valid_v< void(int&), void >);
	static_assert(valid_v< void(&)(int&), void >);
	static_assert(valid_v< void(&&)(int&), void >);
	static_assert(valid_v< void(*)(int&), void >);
	static_assert(valid_v< void(int&)noexcept, void >);
	static_assert(valid_v< void(&)(int&)noexcept, void >);
	static_assert(valid_v< void(&&)(int&)noexcept, void >);
	static_assert(valid_v< void(*)(int&)noexcept, void >);
	static_assert(valid_v< void(A::*)(int&), void >);
	static_assert(valid_v< void(A::*)(int&)const, void >);
	static_assert(valid_v< void(A::*)(int&)noexcept, void >);
	static_assert(valid_v< void(A::*)(int&)const noexcept, void >);

	static_assert(valid_v< void(int&, bool), void >);
	static_assert(valid_v< void(&)(int&, bool), void >);
	static_assert(valid_v< void(&&)(int&, bool), void >);
	static_assert(valid_v< void(*)(int&, bool), void >);
	static_assert(valid_v< void(int&, bool)noexcept, void >);
	static_assert(valid_v< void(&)(int&, bool)noexcept, void >);
	static_assert(valid_v< void(&&)(int&, bool)noexcept, void >);
	static_assert(valid_v< void(*)(int&, bool)noexcept, void >);
	static_assert(valid_v< void(A::*)(int&, bool), void >);
	static_assert(valid_v< void(A::*)(int&, bool)const, void >);
	static_assert(valid_v< void(A::*)(int&, bool)noexcept, void >);
	static_assert(valid_v<
		void(A::*)(int&, bool)const noexcept, void >);

	static_assert(valid_v< void(int&), nobody_t >);
	static_assert(valid_v< void(&)(int&), nobody_t >);
	static_assert(valid_v< void(&&)(int&), nobody_t >);
	static_assert(valid_v< void(*)(int&), nobody_t >);
	static_assert(valid_v< void(int&)noexcept, nobody_t >);
	static_assert(valid_v< void(&)(int&)noexcept, nobody_t >);
	static_assert(valid_v< void(&&)(int&)noexcept, nobody_t >);
	static_assert(valid_v< void(*)(int&)noexcept, nobody_t >);
	static_assert(valid_v< void(A::*)(int&), nobody_t >);
	static_assert(valid_v< void(A::*)(int&)const, nobody_t >);
	static_assert(valid_v< void(A::*)(int&)noexcept, nobody_t >);
	static_assert(valid_v< void(A::*)(int&)const noexcept, nobody_t >);

	static_assert(valid_v< decltype(&valid_fn_t1), void >);
	static_assert(valid_v< decltype(&valid_fn_n1), void >);

	static_assert(valid_v< valid_object_t1, void >);
	static_assert(valid_v< valid_object_t2, void >);
	static_assert(valid_v< valid_object_n1, void >);
	static_assert(valid_v< valid_object_n2, void >);


	static_assert(is_int_v< log_t< void(int&), void > >);
	static_assert(is_int_v< log_t< void(&)(int&), void > >);
	static_assert(is_int_v< log_t< void(&&)(int&), void > >);
	static_assert(is_int_v< log_t< void(*)(int&), void > >);
	static_assert(is_int_v< log_t< void(int&)noexcept, void > >);
	static_assert(is_int_v< log_t< void(&)(int&)noexcept, void > >);
	static_assert(is_int_v< log_t< void(&&)(int&)noexcept, void > >);
	static_assert(is_int_v< log_t< void(*)(int&)noexcept, void > >);
	static_assert(is_int_v< log_t< void(A::*)(int&), void > >);
	static_assert(is_int_v< log_t< void(A::*)(int&)const, void > >);
	static_assert(is_int_v< log_t< void(A::*)(int&)noexcept, void > >);
	static_assert(is_int_v< log_t<
		void(A::*)(int&)const noexcept, void > >);

	static_assert(is_int_v< log_t< void(int&, bool), void > >);
	static_assert(is_int_v< log_t< void(&)(int&, bool), void > >);
	static_assert(is_int_v< log_t< void(&&)(int&, bool), void > >);
	static_assert(is_int_v< log_t< void(*)(int&, bool), void > >);
	static_assert(is_int_v< log_t< void(int&, bool)noexcept, void > >);
	static_assert(is_int_v< log_t< void(&)(int&, bool)noexcept, void > >);
	static_assert(is_int_v< log_t<
		void(&&)(int&, bool)noexcept, void > >);
	static_assert(is_int_v< log_t< void(*)(int&, bool)noexcept, void > >);
	static_assert(is_int_v< log_t< void(A::*)(int&, bool), void > >);
	static_assert(is_int_v< log_t< void(A::*)(int&, bool)const, void > >);
	static_assert(is_int_v< log_t<
		void(A::*)(int&, bool)noexcept, void > >);
	static_assert(is_int_v< log_t<
		void(A::*)(int&, bool)const noexcept, void > >);


	static_assert(is_int_v< log_t< void(int&), nobody_t > >);
	static_assert(is_int_v< log_t< void(&)(int&), nobody_t > >);
	static_assert(is_int_v< log_t< void(&&)(int&), nobody_t > >);
	static_assert(is_int_v< log_t< void(*)(int&), nobody_t > >);
	static_assert(is_int_v< log_t< void(int&)noexcept, nobody_t > >);
	static_assert(is_int_v< log_t< void(&)(int&)noexcept, nobody_t > >);
	static_assert(is_int_v< log_t< void(&&)(int&)noexcept, nobody_t > >);
	static_assert(is_int_v< log_t< void(*)(int&)noexcept, nobody_t > >);
	static_assert(is_int_v< log_t< void(A::*)(int&), nobody_t > >);
	static_assert(is_int_v< log_t< void(A::*)(int&)const, nobody_t > >);
	static_assert(is_int_v< log_t< void(A::*)(int&)noexcept, nobody_t > >);
	static_assert(is_int_v< log_t<
		void(A::*)(int&)const noexcept, nobody_t > >);

	static_assert(is_int_v< log_t< decltype(&valid_fn_t1), void > >);
	static_assert(is_int_v< log_t< decltype(&valid_fn_n1), void > >);

	static_assert(is_int_v< log_t< valid_object_t1, void > >);
	static_assert(is_int_v< log_t< valid_object_t2, void > >);
	static_assert(is_int_v< log_t< valid_object_n1, void > >);
	static_assert(is_int_v< log_t< valid_object_n2, void > >);

	static_assert(valid_v< void(int&, op_body_t), body_t >);
	static_assert(valid_v< void(&)(int&, op_body_t), body_t >);
	static_assert(valid_v< void(&&)(int&, op_body_t), body_t >);
	static_assert(valid_v< void(*)(int&, op_body_t), body_t >);
	static_assert(valid_v< void(int&, op_body_t)noexcept, body_t >);
	static_assert(valid_v< void(&)(int&, op_body_t)noexcept, body_t >);
	static_assert(valid_v< void(&&)(int&, op_body_t)noexcept, body_t >);
	static_assert(valid_v< void(*)(int&, op_body_t)noexcept, body_t >);
	static_assert(valid_v< void(A::*)(int&, op_body_t), body_t >);
	static_assert(valid_v< void(A::*)(int&, op_body_t)const, body_t >);
	static_assert(valid_v< void(A::*)(int&, op_body_t)noexcept, body_t >);



	constexpr void invalid_fn_t1p(int){}
	constexpr void invalid_fn_n1p(int)noexcept{}
	constexpr void invalid_fn_t1c(int&, int){}
	constexpr void invalid_fn_n1c(int&, int)noexcept{}
	constexpr void invalid_fn_t1n(){}
	constexpr void invalid_fn_n1n()noexcept{}


	struct invalid_object_t1p{
		constexpr void operator()(int){}
	};

	struct invalid_object_t2p{
		constexpr void operator()(int)const{}
	};

	struct invalid_object_n1p{
		constexpr void operator()(int)noexcept{}
	};

	struct invalid_object_n2p{
		constexpr void operator()(int)const noexcept{}
	};

	struct invalid_object_t1c{
		constexpr void operator()(int&, int){}
	};

	struct invalid_object_t2c{
		constexpr void operator()(int&, int)const{}
	};

	struct invalid_object_n1c{
		constexpr void operator()(int&, int)noexcept{}
	};

	struct invalid_object_n2c{
		constexpr void operator()(int&, int)const noexcept{}
	};

	struct invalid_object_t1n{
		constexpr void operator()(){}
	};

	struct invalid_object_t2n{
		constexpr void operator()()const{}
	};

	struct invalid_object_n1n{
		constexpr void operator()()noexcept{}
	};

	struct invalid_object_n2n{
		constexpr void operator()()const noexcept{}
	};

	struct invalid_object_multi{
		constexpr void operator()(int&){}
		constexpr void operator()(int&)const{}
	};

	struct invalid_object_none{};


	static_assert(!valid_v< void(int), void >);
	static_assert(!valid_v< void(&)(int), void >);
	static_assert(!valid_v< void(&&)(int), void >);
	static_assert(!valid_v< void(*)(int), void >);
	static_assert(!valid_v< void(int)noexcept, void >);
	static_assert(!valid_v< void(&)(int)noexcept, void >);
	static_assert(!valid_v< void(&&)(int)noexcept, void >);
	static_assert(!valid_v< void(*)(int)noexcept, void >);
	static_assert(!valid_v< void(int&, int), void >);
	static_assert(!valid_v< void(&)(int&, int), void >);
	static_assert(!valid_v< void(&&)(int&, int), void >);
	static_assert(!valid_v< void(*)(int&, int), void >);
	static_assert(!valid_v< void(int&, int)noexcept, void >);
	static_assert(!valid_v< void(&)(int&, int)noexcept, void >);
	static_assert(!valid_v< void(&&)(int&, int)noexcept, void >);
	static_assert(!valid_v< void(*)(int&, int)noexcept, void >);
	static_assert(!valid_v< void(), void >);
	static_assert(!valid_v< void(&)(), void >);
	static_assert(!valid_v< void(&&)(), void >);
	static_assert(!valid_v< void(*)(), void >);
	static_assert(!valid_v< void()noexcept, void >);
	static_assert(!valid_v< void(&)()noexcept, void >);
	static_assert(!valid_v< void(&&)()noexcept, void >);
	static_assert(!valid_v< void(*)()noexcept, void >);

	static_assert(!valid_v< decltype(&invalid_fn_t1p), void >);
	static_assert(!valid_v< decltype(&invalid_fn_n1p), void >);
	static_assert(!valid_v< decltype(&invalid_fn_t1c), void >);
	static_assert(!valid_v< decltype(&invalid_fn_n1c), void >);
	static_assert(!valid_v< decltype(&invalid_fn_t1n), void >);
	static_assert(!valid_v< decltype(&invalid_fn_n1n), void >);

	static_assert(!valid_v< invalid_object_t1p, void >);
	static_assert(!valid_v< invalid_object_t2p, void >);
	static_assert(!valid_v< invalid_object_n1p, void >);
	static_assert(!valid_v< invalid_object_n2p, void >);
	static_assert(!valid_v< invalid_object_t1c, void >);
	static_assert(!valid_v< invalid_object_t2c, void >);
	static_assert(!valid_v< invalid_object_n1c, void >);
	static_assert(!valid_v< invalid_object_n2c, void >);
	static_assert(!valid_v< invalid_object_t1n, void >);
	static_assert(!valid_v< invalid_object_t2n, void >);
	static_assert(!valid_v< invalid_object_n1n, void >);
	static_assert(!valid_v< invalid_object_n2n, void >);
	static_assert(!valid_v< invalid_object_multi, void >);
	static_assert(!valid_v< invalid_object_none, void >);

	static_assert(!valid_v< void(int&, op_nobody_t), nobody_t >);
	static_assert(!valid_v< void(&)(int&, op_nobody_t), nobody_t >);
	static_assert(!valid_v< void(&&)(int&, op_nobody_t), nobody_t >);
	static_assert(!valid_v< void(*)(int&, op_nobody_t), nobody_t >);
	static_assert(!valid_v< void(int&, op_nobody_t)noexcept, nobody_t >);
	static_assert(!valid_v< void(&)(int&, op_nobody_t)noexcept, nobody_t >);
	static_assert(!valid_v< void(&&)(int&, op_nobody_t)noexcept, nobody_t >);
	static_assert(!valid_v< void(*)(int&, op_nobody_t)noexcept, nobody_t >);
	static_assert(!valid_v< void(A::*)(int&, op_nobody_t), nobody_t >);
	static_assert(!valid_v< void(A::*)(int&, op_nobody_t)const, nobody_t >);
	static_assert(!valid_v< void(A::*)(int&, op_nobody_t)noexcept, nobody_t >);

}
