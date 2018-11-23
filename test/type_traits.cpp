#include <logsys/detail/type_traits.hpp>


namespace{


	struct D1{};
	struct D2;

	using logsys::detail::is_defined_v;

	static_assert( is_defined_v< D1 >);
	static_assert(!is_defined_v< D2 >);


	struct V1{};
	struct V2{ int v; };
	struct V3{ void f(){} };
	struct V4{ void f()const{} };

	using logsys::detail::is_valid;

	static_assert(!is_valid< V1 >([](auto& x)->decltype((void)x.v){}));
	static_assert( is_valid< V2 >([](auto& x)->decltype((void)x.v){}));
	static_assert(!is_valid< V3 >([](auto& x)->decltype((void)x.v){}));
	static_assert(!is_valid< V4 >([](auto& x)->decltype((void)x.v){}));

	static_assert(!is_valid< V1 >([](auto& x)->decltype((void)x.f()){}));
	static_assert(!is_valid< V2 >([](auto& x)->decltype((void)x.f()){}));
	static_assert( is_valid< V3 >([](auto& x)->decltype((void)x.f()){}));
	static_assert( is_valid< V4 >([](auto& x)->decltype((void)x.f()){}));

	static_assert(!is_valid< V1& >([](auto& x)->decltype((void)x.v){}));
	static_assert( is_valid< V2& >([](auto& x)->decltype((void)x.v){}));
	static_assert(!is_valid< V3& >([](auto& x)->decltype((void)x.v){}));
	static_assert(!is_valid< V4& >([](auto& x)->decltype((void)x.v){}));

	static_assert(!is_valid< V1& >([](auto& x)->decltype((void)x.f()){}));
	static_assert(!is_valid< V2& >([](auto& x)->decltype((void)x.f()){}));
	static_assert( is_valid< V3& >([](auto& x)->decltype((void)x.f()){}));
	static_assert( is_valid< V4& >([](auto& x)->decltype((void)x.f()){}));

	static_assert(!is_valid< V1&& >([](auto& x)->decltype((void)x.v){}));
	static_assert( is_valid< V2&& >([](auto& x)->decltype((void)x.v){}));
	static_assert(!is_valid< V3&& >([](auto& x)->decltype((void)x.v){}));
	static_assert(!is_valid< V4&& >([](auto& x)->decltype((void)x.v){}));

	static_assert(!is_valid< V1&& >([](auto& x)->decltype((void)x.f()){}));
	static_assert(!is_valid< V2&& >([](auto& x)->decltype((void)x.f()){}));
	static_assert( is_valid< V3&& >([](auto& x)->decltype((void)x.f()){}));
	static_assert( is_valid< V4&& >([](auto& x)->decltype((void)x.f()){}));

	static_assert(!is_valid< V1 const& >([](auto& x)->decltype((void)x.v){}));
	static_assert( is_valid< V2 const& >([](auto& x)->decltype((void)x.v){}));
	static_assert(!is_valid< V3 const& >([](auto& x)->decltype((void)x.v){}));
	static_assert(!is_valid< V4 const& >([](auto& x)->decltype((void)x.v){}));

	static_assert(!is_valid< V1 const& >([](auto& x)->decltype((void)x.f()){}));
	static_assert(!is_valid< V2 const& >([](auto& x)->decltype((void)x.f()){}));
	static_assert(!is_valid< V3 const& >([](auto& x)->decltype((void)x.f()){}));
	static_assert( is_valid< V4 const& >([](auto& x)->decltype((void)x.f()){}));



	struct X1{};
	struct X2{ void operator()(){} };
	struct X3{ void operator()()const{} };
	struct X4{ void operator()(){} void operator()()const{} };

	using logsys::detail::has_unique_call_operator_v;

	static_assert(!has_unique_call_operator_v< X1 >);
	static_assert( has_unique_call_operator_v< X2 >);
	static_assert( has_unique_call_operator_v< X3 >);
	static_assert(!has_unique_call_operator_v< X4 >);

	using logsys::detail::extract_callable_t;

	static_assert(std::is_same_v< extract_callable_t< X2 >,
		void(X2::*)() >);
	static_assert(std::is_same_v< extract_callable_t< X3 >,
		void(X3::*)()const >);

}
