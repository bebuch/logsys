//-----------------------------------------------------------------------------
// Copyright (c) 2018 Benjamin Buch
//
// https://github.com/bebuch/logsys
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#include <logsys/detail/log_trait.hpp>


namespace{


	struct log_01{
		void exec()noexcept;
		void set_exception(std::exception const&)noexcept;
		void unknown_exception()noexcept;
	};


	using trait_01 = logsys::log_trait< log_01 >;

	static_assert(!trait_01::has_pre);
	static_assert(!trait_01::has_post);
	static_assert(!trait_01::has_body_failed);
	static_assert(!trait_01::has_have_body);
	static_assert(!trait_01::has_factory);


}
