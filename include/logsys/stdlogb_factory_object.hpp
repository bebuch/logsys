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

#include <memory>
#include <functional>


namespace logsys{


	class stdlog_base;

	/// \brief Assign your log object maker to this variable
	extern std::unique_ptr< stdlog_base >(*stdlogb_factory_object)()noexcept;


}


#endif
