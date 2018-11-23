#include <logsys/detail/extract_log_t.hpp>

// -----------------------------------------------------------------------------
// TODO: Test seconed parameter!!!
// -----------------------------------------------------------------------------


namespace {


	using logsys::detail::remove_lvalue_reference_t;
	using logsys::detail::is_valid_body_return_type_parameter;
	using logsys::optional_lvalue_reference;
	using logsys::optional_rvalue_reference;

	static_assert(std::is_same_v< remove_lvalue_reference_t< int >, int >);
	static_assert(std::is_same_v< remove_lvalue_reference_t< int& >, int >);
	static_assert(std::is_same_v< remove_lvalue_reference_t< int&& >, int&& >);

	static_assert( is_valid_body_return_type_parameter< void, bool >);
	static_assert( is_valid_body_return_type_parameter< void, bool& >);
	static_assert( is_valid_body_return_type_parameter< void, bool const >);
	static_assert( is_valid_body_return_type_parameter< void, bool const& >);
	static_assert(!is_valid_body_return_type_parameter< void, bool&& >);
	static_assert(!is_valid_body_return_type_parameter< void, bool const&& >);

	static_assert( is_valid_body_return_type_parameter< int,
		std::optional< int > >);
	static_assert( is_valid_body_return_type_parameter< int,
		std::optional< int >& >);
	static_assert( is_valid_body_return_type_parameter< int,
		std::optional< int > const >);
	static_assert( is_valid_body_return_type_parameter< int,
		std::optional< int > const& >);
	static_assert(!is_valid_body_return_type_parameter< int,
		std::optional< int >&& >);
	static_assert(!is_valid_body_return_type_parameter< int,
		std::optional< int > const&& >);

	static_assert( is_valid_body_return_type_parameter< int&,
		optional_lvalue_reference< int > >);
	static_assert( is_valid_body_return_type_parameter< int&,
		optional_lvalue_reference< int >& >);
	static_assert( is_valid_body_return_type_parameter< int&,
		optional_lvalue_reference< int > const >);
	static_assert( is_valid_body_return_type_parameter< int&,
		optional_lvalue_reference< int > const& >);
	static_assert(!is_valid_body_return_type_parameter< int&,
		optional_lvalue_reference< int >&& >);
	static_assert(!is_valid_body_return_type_parameter< int&,
		optional_lvalue_reference< int > const&& >);

	static_assert( is_valid_body_return_type_parameter< int&&,
		optional_rvalue_reference< int > >);
	static_assert( is_valid_body_return_type_parameter< int&&,
		optional_rvalue_reference< int >& >);
	static_assert( is_valid_body_return_type_parameter< int&&,
		optional_rvalue_reference< int > const >);
	static_assert( is_valid_body_return_type_parameter< int&&,
		optional_rvalue_reference< int > const& >);
	static_assert(!is_valid_body_return_type_parameter< int&&,
		optional_rvalue_reference< int >&& >);
	static_assert(!is_valid_body_return_type_parameter< int&&,
		optional_rvalue_reference< int > const&& >);


	struct A{};


	constexpr void valid_fn_t1(int&){};
	constexpr void valid_fn_n1(int&)noexcept{};

	struct valid_object_t1{
		constexpr void operator()(int&){};
	};

	struct valid_object_t2{
		constexpr void operator()(int&)const{};
	};

	struct valid_object_n1{
		constexpr void operator()(int&)noexcept{};
	};

	struct valid_object_n2{
		constexpr void operator()(int&)const noexcept{};
	};


	template < typename T >
	constexpr bool is_int_v = std::is_same_v< T, int >;


	void valid(){
		using logsys::detail::extract_log_t;
		using logsys::detail::is_extract_log_valid_v;

		int stuff = 0;
		auto valid_lambda_t1 = [](int&){};
		auto valid_lambda_t2 = [stuff](int&){};
		auto valid_lambda_t3 = [&stuff](int&){};
		auto valid_lambda_t4 = [](int&)mutable{};
		auto valid_lambda_t5 = [stuff](int&)mutable{};
		auto valid_lambda_t6 = [&stuff](int&)mutable{};

		auto valid_lambda_n1 = [](int&)noexcept{};
		auto valid_lambda_n2 = [stuff](int&)noexcept{};
		auto valid_lambda_n3 = [&stuff](int&)noexcept{};
		auto valid_lambda_n4 = [](int&)mutable noexcept{};
		auto valid_lambda_n5 = [stuff](int&)mutable noexcept{};
		auto valid_lambda_n6 = [&stuff](int&)mutable noexcept{};

		static_assert(is_extract_log_valid_v< void(int&) >);
		static_assert(is_extract_log_valid_v< void(&)(int&) >);
		static_assert(is_extract_log_valid_v< void(&&)(int&) >);
		static_assert(is_extract_log_valid_v< void(*)(int&) >);
		static_assert(is_extract_log_valid_v< void(int&)noexcept >);
		static_assert(is_extract_log_valid_v< void(&)(int&)noexcept >);
		static_assert(is_extract_log_valid_v< void(&&)(int&)noexcept >);
		static_assert(is_extract_log_valid_v< void(*)(int&)noexcept >);
		static_assert(is_extract_log_valid_v< void(A::*)(int&) >);
		static_assert(is_extract_log_valid_v< void(A::*)(int&)const >);
		static_assert(is_extract_log_valid_v< void(A::*)(int&)noexcept >);
		static_assert(is_extract_log_valid_v< void(A::*)(int&)const noexcept >);

		static_assert(is_extract_log_valid_v< void(int&, bool) >);
		static_assert(is_extract_log_valid_v< void(&)(int&, bool) >);
		static_assert(is_extract_log_valid_v< void(&&)(int&, bool) >);
		static_assert(is_extract_log_valid_v< void(*)(int&, bool) >);
		static_assert(is_extract_log_valid_v< void(int&, bool)noexcept >);
		static_assert(is_extract_log_valid_v< void(&)(int&, bool)noexcept >);
		static_assert(is_extract_log_valid_v< void(&&)(int&, bool)noexcept >);
		static_assert(is_extract_log_valid_v< void(*)(int&, bool)noexcept >);
		static_assert(is_extract_log_valid_v< void(A::*)(int&, bool) >);
		static_assert(is_extract_log_valid_v< void(A::*)(int&, bool)const >);
		static_assert(is_extract_log_valid_v< void(A::*)(int&, bool)noexcept >);
		static_assert(is_extract_log_valid_v<
			void(A::*)(int&, bool)const noexcept >);

		static_assert(is_extract_log_valid_v< decltype(&valid_fn_t1) >);
		static_assert(is_extract_log_valid_v< decltype(&valid_fn_n1) >);

		static_assert(is_extract_log_valid_v< valid_object_t1 >);
		static_assert(is_extract_log_valid_v< valid_object_t2 >);
		static_assert(is_extract_log_valid_v< valid_object_n1 >);
		static_assert(is_extract_log_valid_v< valid_object_n2 >);

		static_assert(is_extract_log_valid_v< decltype(valid_lambda_t1) >);
		static_assert(is_extract_log_valid_v< decltype(valid_lambda_t2) >);
		static_assert(is_extract_log_valid_v< decltype(valid_lambda_t3) >);
		static_assert(is_extract_log_valid_v< decltype(valid_lambda_t4) >);
		static_assert(is_extract_log_valid_v< decltype(valid_lambda_t5) >);
		static_assert(is_extract_log_valid_v< decltype(valid_lambda_t6) >);
		static_assert(is_extract_log_valid_v< decltype(valid_lambda_n1) >);
		static_assert(is_extract_log_valid_v< decltype(valid_lambda_n2) >);
		static_assert(is_extract_log_valid_v< decltype(valid_lambda_n3) >);
		static_assert(is_extract_log_valid_v< decltype(valid_lambda_n4) >);
		static_assert(is_extract_log_valid_v< decltype(valid_lambda_n5) >);
		static_assert(is_extract_log_valid_v< decltype(valid_lambda_n6) >);


		static_assert(is_int_v< extract_log_t< void(int&) > >);
		static_assert(is_int_v< extract_log_t< void(&)(int&) > >);
		static_assert(is_int_v< extract_log_t< void(&&)(int&) > >);
		static_assert(is_int_v< extract_log_t< void(*)(int&) > >);
		static_assert(is_int_v< extract_log_t< void(int&)noexcept > >);
		static_assert(is_int_v< extract_log_t< void(&)(int&)noexcept > >);
		static_assert(is_int_v< extract_log_t< void(&&)(int&)noexcept > >);
		static_assert(is_int_v< extract_log_t< void(*)(int&)noexcept > >);
		static_assert(is_int_v< extract_log_t< void(A::*)(int&) > >);
		static_assert(is_int_v< extract_log_t< void(A::*)(int&)const > >);
		static_assert(is_int_v< extract_log_t< void(A::*)(int&)noexcept > >);
		static_assert(is_int_v< extract_log_t<
			void(A::*)(int&)const noexcept > >);

		static_assert(is_int_v< extract_log_t< void(int&, bool) > >);
		static_assert(is_int_v< extract_log_t< void(&)(int&, bool) > >);
		static_assert(is_int_v< extract_log_t< void(&&)(int&, bool) > >);
		static_assert(is_int_v< extract_log_t< void(*)(int&, bool) > >);
		static_assert(is_int_v< extract_log_t< void(int&, bool)noexcept > >);
		static_assert(is_int_v< extract_log_t< void(&)(int&, bool)noexcept > >);
		static_assert(is_int_v< extract_log_t<
			void(&&)(int&, bool)noexcept > >);
		static_assert(is_int_v< extract_log_t< void(*)(int&, bool)noexcept > >);
		static_assert(is_int_v< extract_log_t< void(A::*)(int&, bool) > >);
		static_assert(is_int_v< extract_log_t< void(A::*)(int&, bool)const > >);
		static_assert(is_int_v< extract_log_t<
			void(A::*)(int&, bool)noexcept > >);
		static_assert(is_int_v< extract_log_t<
			void(A::*)(int&, bool)const noexcept > >);

		static_assert(is_int_v< extract_log_t< decltype(&valid_fn_t1) > >);
		static_assert(is_int_v< extract_log_t< decltype(&valid_fn_n1) > >);

		static_assert(is_int_v< extract_log_t< valid_object_t1 > >);
		static_assert(is_int_v< extract_log_t< valid_object_t2 > >);
		static_assert(is_int_v< extract_log_t< valid_object_n1 > >);
		static_assert(is_int_v< extract_log_t< valid_object_n2 > >);

		static_assert(is_int_v< extract_log_t< decltype(valid_lambda_t1) > >);
		static_assert(is_int_v< extract_log_t< decltype(valid_lambda_t2) > >);
		static_assert(is_int_v< extract_log_t< decltype(valid_lambda_t3) > >);
		static_assert(is_int_v< extract_log_t< decltype(valid_lambda_t4) > >);
		static_assert(is_int_v< extract_log_t< decltype(valid_lambda_t5) > >);
		static_assert(is_int_v< extract_log_t< decltype(valid_lambda_t6) > >);
		static_assert(is_int_v< extract_log_t< decltype(valid_lambda_n1) > >);
		static_assert(is_int_v< extract_log_t< decltype(valid_lambda_n2) > >);
		static_assert(is_int_v< extract_log_t< decltype(valid_lambda_n3) > >);
		static_assert(is_int_v< extract_log_t< decltype(valid_lambda_n4) > >);
		static_assert(is_int_v< extract_log_t< decltype(valid_lambda_n5) > >);
		static_assert(is_int_v< extract_log_t< decltype(valid_lambda_n6) > >);
	}


	constexpr void invalid_fn_t1p(int){};
	constexpr void invalid_fn_n1p(int)noexcept{};
	constexpr void invalid_fn_t1c(int&, int){};
	constexpr void invalid_fn_n1c(int&, int)noexcept{};
	constexpr void invalid_fn_t1n(){};
	constexpr void invalid_fn_n1n()noexcept{};


	struct invalid_object_t1p{
		constexpr void operator()(int){};
	};

	struct invalid_object_t2p{
		constexpr void operator()(int)const{};
	};

	struct invalid_object_n1p{
		constexpr void operator()(int)noexcept{};
	};

	struct invalid_object_n2p{
		constexpr void operator()(int)const noexcept{};
	};

	struct invalid_object_t1c{
		constexpr void operator()(int&, int){};
	};

	struct invalid_object_t2c{
		constexpr void operator()(int&, int)const{};
	};

	struct invalid_object_n1c{
		constexpr void operator()(int&, int)noexcept{};
	};

	struct invalid_object_n2c{
		constexpr void operator()(int&, int)const noexcept{};
	};

	struct invalid_object_t1n{
		constexpr void operator()(){};
	};

	struct invalid_object_t2n{
		constexpr void operator()()const{};
	};

	struct invalid_object_n1n{
		constexpr void operator()()noexcept{};
	};

	struct invalid_object_n2n{
		constexpr void operator()()const noexcept{};
	};

	struct invalid_object_multi{
		constexpr void operator()(int&){};
		constexpr void operator()(int&)const{};
	};

	struct invalid_object_none{};


	void invalid(){
		using logsys::detail::is_extract_log_valid_v;

		static_assert(!is_extract_log_valid_v< void(int) >);
		static_assert(!is_extract_log_valid_v< void(&)(int) >);
		static_assert(!is_extract_log_valid_v< void(&&)(int) >);
		static_assert(!is_extract_log_valid_v< void(*)(int) >);
		static_assert(!is_extract_log_valid_v< void(int)noexcept >);
		static_assert(!is_extract_log_valid_v< void(&)(int)noexcept >);
		static_assert(!is_extract_log_valid_v< void(&&)(int)noexcept >);
		static_assert(!is_extract_log_valid_v< void(*)(int)noexcept >);
		static_assert(!is_extract_log_valid_v< void(int&, int) >);
		static_assert(!is_extract_log_valid_v< void(&)(int&, int) >);
		static_assert(!is_extract_log_valid_v< void(&&)(int&, int) >);
		static_assert(!is_extract_log_valid_v< void(*)(int&, int) >);
		static_assert(!is_extract_log_valid_v< void(int&, int)noexcept >);
		static_assert(!is_extract_log_valid_v< void(&)(int&, int)noexcept >);
		static_assert(!is_extract_log_valid_v< void(&&)(int&, int)noexcept >);
		static_assert(!is_extract_log_valid_v< void(*)(int&, int)noexcept >);
		static_assert(!is_extract_log_valid_v< void() >);
		static_assert(!is_extract_log_valid_v< void(&)() >);
		static_assert(!is_extract_log_valid_v< void(&&)() >);
		static_assert(!is_extract_log_valid_v< void(*)() >);
		static_assert(!is_extract_log_valid_v< void()noexcept >);
		static_assert(!is_extract_log_valid_v< void(&)()noexcept >);
		static_assert(!is_extract_log_valid_v< void(&&)()noexcept >);
		static_assert(!is_extract_log_valid_v< void(*)()noexcept >);

		static_assert(!is_extract_log_valid_v< decltype(&invalid_fn_t1p) >);
		static_assert(!is_extract_log_valid_v< decltype(&invalid_fn_n1p) >);
		static_assert(!is_extract_log_valid_v< decltype(&invalid_fn_t1c) >);
		static_assert(!is_extract_log_valid_v< decltype(&invalid_fn_n1c) >);
		static_assert(!is_extract_log_valid_v< decltype(&invalid_fn_t1n) >);
		static_assert(!is_extract_log_valid_v< decltype(&invalid_fn_n1n) >);

		static_assert(!is_extract_log_valid_v< invalid_object_t1p >);
		static_assert(!is_extract_log_valid_v< invalid_object_t2p >);
		static_assert(!is_extract_log_valid_v< invalid_object_n1p >);
		static_assert(!is_extract_log_valid_v< invalid_object_n2p >);
		static_assert(!is_extract_log_valid_v< invalid_object_t1c >);
		static_assert(!is_extract_log_valid_v< invalid_object_t2c >);
		static_assert(!is_extract_log_valid_v< invalid_object_n1c >);
		static_assert(!is_extract_log_valid_v< invalid_object_n2c >);
		static_assert(!is_extract_log_valid_v< invalid_object_t1n >);
		static_assert(!is_extract_log_valid_v< invalid_object_t2n >);
		static_assert(!is_extract_log_valid_v< invalid_object_n1n >);
		static_assert(!is_extract_log_valid_v< invalid_object_n2n >);
		static_assert(!is_extract_log_valid_v< invalid_object_multi >);
		static_assert(!is_extract_log_valid_v< invalid_object_none >);
	}

}
