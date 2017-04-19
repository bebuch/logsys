//-----------------------------------------------------------------------------
// Copyright (c) 2015-2017 Benjamin Buch
//
// https://github.com/bebuch/log
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#include <log/stdlogd.hpp>


namespace log{


	std::function< std::unique_ptr< log_base >() > log_base::factory(
		[]{ return std::make_unique< stdlogd >(); }
	);


}
