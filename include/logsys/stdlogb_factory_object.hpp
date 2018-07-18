//-----------------------------------------------------------------------------
// Copyright (c) 2015-2018 Benjamin Buch
//
// https://github.com/bebuch/logsys
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#ifndef _logsys__stdlogb_factory_object__hpp_INCLUDED_
#define _logsys__stdlogb_factory_object__hpp_INCLUDED_

#include <boost/config.hpp>

#include <memory>
#include <functional>


namespace logsys{


	class stdlogb;

	/// \brief Assign your log object maker to this variable
	extern BOOST_SYMBOL_EXPORT
	std::unique_ptr< stdlogb >(*stdlogb_factory_object)()noexcept;


}


#endif
