//-----------------------------------------------------------------------------
// Copyright (c) 2015-2017 Benjamin Buch
//
// https://github.com/bebuch/logsys
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#include <logsys/stdlogd.hpp>


namespace logsys{


	std::function< std::unique_ptr< stdlogb >() > stdlogb::factory(
		[]{ return std::make_unique< stdlogd >(); }
	);


}
