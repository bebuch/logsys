#include <logsys/optional.hpp>

#include "gtest/gtest.h"


namespace{


	using logsys::optional;

	static_assert(std::is_same_v< optional< void >, bool >);
	static_assert(std::is_same_v< optional< int >, std::optional< int > >);
	static_assert(std::is_same_v< optional< int* >, std::optional< int* > >);
	static_assert(std::is_same_v< optional< int& >,
		logsys::optional_lvalue_reference< int >
	>);
	static_assert(std::is_same_v< optional< int const& >,
		logsys::optional_lvalue_reference< int const >
	>);
	static_assert(std::is_same_v< optional< int&& >,
		logsys::optional_rvalue_reference< int >
	>);
	static_assert(std::is_same_v< optional< int const&& >,
		logsys::optional_rvalue_reference< int const >
	>);


	struct integer{
		constexpr integer(int v): v(v) {}
		integer(integer const&) = default;
		constexpr integer(integer&& other): v(other.v) {
			other.v = -1;
		}

		constexpr integer& operator=(int v){
			this->v = v;
			return *this;
		}

		int v;
	};

	constexpr bool operator==(integer const& l, integer const& r)noexcept{
		return l.v == r.v;
	}


	TEST(optional_lvalue_reference, dereference){
		integer v = 5;
		logsys::optional_lvalue_reference< integer > o(v);

		EXPECT_EQ(*o, v);

		*o = 6;

		EXPECT_EQ(v, 6);

		v = 7;

		EXPECT_EQ(*o, 7);

		integer n = *o;

		EXPECT_EQ(n.v, 7);
		EXPECT_EQ(v.v, 7);
		EXPECT_EQ(*o, 7);
	}

	TEST(optional_lvalue_reference, const_dereference){
		integer v = 5;
		logsys::optional_lvalue_reference< integer > const o(v);

		EXPECT_EQ(*o, v);

		v = 7;

		EXPECT_EQ(*o, 7);

		integer n = *o;

		EXPECT_EQ(n.v, 7);
		EXPECT_EQ(v.v, 7);
		EXPECT_EQ(*o, 7);
	}

	TEST(optional_lvalue_reference, dereference_access){
		integer v = 5;
		logsys::optional_lvalue_reference< integer > o(v);

		EXPECT_EQ(o->v, v.v);

		o->v = 6;

		EXPECT_EQ(v, 6);

		v = 7;

		EXPECT_EQ(o->v, 7);
	}

	TEST(optional_lvalue_reference, const_dereference_access){
		integer v = 5;
		logsys::optional_lvalue_reference< integer > const o(v);

		EXPECT_EQ(o->v, v.v);

		v = 7;

		EXPECT_EQ(o->v, 7);
	}

	TEST(optional_lvalue_reference, has_value){
		{
			logsys::optional_lvalue_reference< integer > o;

			EXPECT_FALSE(o.has_value());
		}
		{
			integer v = 5;
			logsys::optional_lvalue_reference< integer > o(v);

			EXPECT_TRUE(o.has_value());
		}
	}

	TEST(optional_lvalue_reference, bool_convert){
		{
			logsys::optional_lvalue_reference< integer > o;

			EXPECT_FALSE(o);
		}
		{
			integer v = 5;
			logsys::optional_lvalue_reference< integer > o(v);

			EXPECT_TRUE(o);
		}
	}

	TEST(optional_lvalue_reference, value){
		integer v = 5;
		logsys::optional_lvalue_reference< integer > o(v);

		EXPECT_EQ(o.value(), v);

		o.value() = 6;

		EXPECT_EQ(v, 6);

		v = 7;

		EXPECT_EQ(o.value(), 7);

		integer n = o.value();

		EXPECT_EQ(n.v, 7);
		EXPECT_EQ(v.v, 7);
		EXPECT_EQ(o.value(), 7);
	}

	TEST(optional_lvalue_reference, const_value){
		integer v = 5;
		logsys::optional_lvalue_reference< integer > const o(v);

		EXPECT_EQ(o.value(), v);

		v = 7;

		EXPECT_EQ(o.value(), 7);

		integer n = o.value();

		EXPECT_EQ(n.v, 7);
		EXPECT_EQ(v.v, 7);
		EXPECT_EQ(o.value(), 7);
	}

	TEST(optional_lvalue_reference, value_throw){
		logsys::optional_lvalue_reference< integer > o;

		EXPECT_THROW(o.value(), std::bad_optional_access);
	}

	TEST(optional_lvalue_reference, const_value_throw){
		logsys::optional_lvalue_reference< integer > const o;

		EXPECT_THROW(o.value(), std::bad_optional_access);
	}

	TEST(optional_lvalue_reference, value_or){
		{
			logsys::optional_lvalue_reference< integer > o;

			EXPECT_EQ(o.value_or(6), 6);
		}
		{
			integer v = 5;
			logsys::optional_lvalue_reference< integer > o(v);

			EXPECT_EQ(o.value_or(6), 5);
			EXPECT_EQ(o.value(), 5);
		}
	}


	TEST(optional_rvalue_reference, dereference){
		integer v = 5;
		logsys::optional_rvalue_reference< integer > o(std::move(v));

		EXPECT_EQ(*o, v);

		*o = 6;

		EXPECT_EQ(v, 6);

		v = 7;

		EXPECT_EQ(*o, 7);

		integer n = *o;

		EXPECT_EQ(n.v, 7);
		EXPECT_EQ(v.v, -1);
		EXPECT_EQ(*o, -1);
	}

	TEST(optional_rvalue_reference, const_dereference){
		integer v = 5;
		logsys::optional_rvalue_reference< integer > const o(std::move(v));

		EXPECT_EQ(*o, v);

		v = 7;

		EXPECT_EQ(*o, 7);

		integer n = *o;

		EXPECT_EQ(n.v, 7);
		EXPECT_EQ(v.v, 7);
		EXPECT_EQ(*o, 7);
	}

	TEST(optional_rvalue_reference, dereference_access){
		integer v = 5;
		logsys::optional_rvalue_reference< integer > o(std::move(v));

		EXPECT_EQ(o->v, v.v);

		o->v = 6;

		EXPECT_EQ(v, 6);

		v = 7;

		EXPECT_EQ(o->v, 7);
	}

	TEST(optional_rvalue_reference, const_dereference_access){
		integer v = 5;
		logsys::optional_rvalue_reference< integer > const o(std::move(v));

		EXPECT_EQ(o->v, v.v);

		v = 7;

		EXPECT_EQ(o->v, 7);
	}

	TEST(optional_rvalue_reference, has_value){
		{
			logsys::optional_rvalue_reference< integer > o;

			EXPECT_FALSE(o.has_value());
		}
		{
			integer v = 5;
			logsys::optional_rvalue_reference< integer > o(std::move(v));

			EXPECT_TRUE(o.has_value());
		}
	}

	TEST(optional_rvalue_reference, bool_convert){
		{
			logsys::optional_rvalue_reference< integer > o;

			EXPECT_FALSE(o);
		}
		{
			integer v = 5;
			logsys::optional_rvalue_reference< integer > o(std::move(v));

			EXPECT_TRUE(o);
		}
	}

	TEST(optional_rvalue_reference, value){
		integer v = 5;
		logsys::optional_rvalue_reference< integer > o(std::move(v));

		EXPECT_EQ(o.value(), v);

		o.value() = 6;

		EXPECT_EQ(v, 6);

		v = 7;

		EXPECT_EQ(o.value(), 7);

		integer n = o.value();

		EXPECT_EQ(n.v, 7);
		EXPECT_EQ(v.v, -1);
		EXPECT_EQ(o.value(), -1);
	}

	TEST(optional_rvalue_reference, const_value){
		integer v = 5;
		logsys::optional_rvalue_reference< integer > const o(std::move(v));

		EXPECT_EQ(o.value(), v);

		v = 7;

		EXPECT_EQ(o.value(), 7);

		integer n = o.value();

		EXPECT_EQ(n.v, 7);
		EXPECT_EQ(v.v, 7);
		EXPECT_EQ(o.value(), 7);
	}

	TEST(optional_rvalue_reference, value_throw){
		logsys::optional_rvalue_reference< integer > o;

		EXPECT_THROW(o.value(), std::bad_optional_access);
	}

	TEST(optional_rvalue_reference, const_value_throw){
		logsys::optional_rvalue_reference< integer > const o;

		EXPECT_THROW(o.value(), std::bad_optional_access);
	}

	TEST(optional_rvalue_reference, value_or){
		{
			logsys::optional_rvalue_reference< integer > o;

			EXPECT_EQ(o.value_or(6), 6);
		}
		{
			integer v = 5;
			logsys::optional_rvalue_reference< integer > o(std::move(v));

			EXPECT_EQ(o.value_or(6), 5);
			EXPECT_EQ(o.value(), -1);
		}
		{
			integer v = 5;
			logsys::optional_rvalue_reference< integer > const o(std::move(v));

			EXPECT_EQ(o.value_or(6), 5);
			EXPECT_EQ(o.value(), 5);
		}
	}


}
