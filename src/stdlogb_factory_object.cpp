//-----------------------------------------------------------------------------
// Copyright (c) 2015-2018 Benjamin Buch
//
// https://github.com/bebuch/logsys
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#include <logsys/stdlogb_factory_object.hpp>
#include <logsys/stdlogd.hpp>


namespace logsys{


	std::function< std::unique_ptr< stdlogb >() > stdlogb_factory_object(
		[](){ return std::make_unique< stdlogd >(); }
	);


}
