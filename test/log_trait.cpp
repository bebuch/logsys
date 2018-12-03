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


	struct log_required_functions{
		void exec()noexcept{}
		void set_body_exception(
			std::exception_ptr error, bool rethrow)noexcept{}
		void set_log_exception(std::exception_ptr error)noexcept{}
	};


	struct log_01: log_required_functions{};

	using trait_01 = logsys::log_trait< log_01 >;

	static_assert(!trait_01::has_body_finished);


	struct log_02: log_required_functions{
		void body_finished()noexcept{}
	};

	using trait_02 = logsys::log_trait< log_02 >;

	static_assert( trait_02::has_body_finished);


}
