#include <logsys/log.hpp>

struct A{};
struct B{ int v; };
struct C{ void f(){} };

int main(){
	static_assert(!logsys::detail::is_valid< A >([](auto&& x)->decltype(
		(void)x.v
	){}));
	static_assert(logsys::detail::is_valid< B >([](auto&& x)->decltype(
		(void)x.v
	){}));
	static_assert(!logsys::detail::is_valid< C >([](auto&& x)->decltype(
		(void)x.v
	){}));

	static_assert(!logsys::detail::is_valid< A >([](auto&& x)->decltype(
		(void)x.f()
	){}));
	static_assert(!logsys::detail::is_valid< B >([](auto&& x)->decltype(
		(void)x.f()
	){}));
	static_assert(logsys::detail::is_valid< C >([](auto&& x)->decltype(
		(void)x.f()
	){}));
}
