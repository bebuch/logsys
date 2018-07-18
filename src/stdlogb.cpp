//-----------------------------------------------------------------------------
// Copyright (c) 2015-2018 Benjamin Buch
//
// https://github.com/bebuch/logsys
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#include <logsys/stdlogb_factory_object.hpp>
#include <logsys/stdlogb.hpp>

#include <cassert>


namespace logsys{


	std::unique_ptr< stdlogb > stdlogb::factory()noexcept try{
		assert(stdlogb_factory_object != nullptr);
		return stdlogb_factory_object();
	}catch(std::exception const& e){
		std::cerr << "terminate with exception in stdlogb factory: "
			<< e.what() << '\n';
		std::terminate();
	}catch(...){
		std::cerr << "terminate with unknown exception in stdlogb factory\n";
		std::terminate();
	}


}
