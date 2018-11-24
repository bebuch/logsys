//-----------------------------------------------------------------------------
// Copyright (c) 2015-2018 Benjamin Buch
//
// https://github.com/bebuch/logsys
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt)
//-----------------------------------------------------------------------------
#include <logsys/stdlogb.hpp>
#include <logsys/log.hpp>

#include <boost/type_index.hpp>

#include <iostream>

l
using logsys::stdlogb;


// log function types

struct fo1{
	void operator()(stdlogb& os){
		os << "struct";
	}
};

struct fo2{
	void operator()(stdlogb& os)const{
		os << "struct const";
	}
};


void f(stdlogb& os){
	os << "f";
}

void fp(stdlogb& os){
	os << "fp";
}

void fr(stdlogb& os){
	os << "fr";
}

void fpr(stdlogb& os){
	os << "fpr";
}

void (*p)(stdlogb& os) = &fp;
void (&r)(stdlogb& os) = fr;



// body return type

struct t1{
	int operator()(){
		return 1;
	}
};

struct t2{
	int a = 0;
	decltype(auto) operator()(){
		return (a);
	}
};


void test(){
	std::function< void(stdlogb&) > stdf_r = [](stdlogb& os){ os << "stdf_r"; };
	auto lambda_r = [](stdlogb& os){ os << "lambda_r"; };

	// log function types
	logsys::log([](stdlogb& os){ os << "lambda"; });
	logsys::log(fo1());
	logsys::log(fo2());
	logsys::log(f);
	logsys::log(&fp);
	logsys::log(p);
	logsys::log(r);
	logsys::log(std::function< void(stdlogb&) >([](stdlogb& os){ os << "stdf"; }));
	logsys::log(stdf_r);
	logsys::log(lambda_r);


	// body return type
	std::cout << boost::typeindex::type_id_with_cvr< decltype(logsys::log(f, t1{})) >().pretty_name() << " == int" << std::endl;
	std::cout << boost::typeindex::type_id_with_cvr< decltype(logsys::log(f, t2{})) >().pretty_name() << " == int&" << std::endl;

	int a = 0;
	decltype(auto) w1 = []{ return 1; }();
	std::cout << boost::typeindex::type_id_with_cvr< decltype(w1) >().pretty_name() << std::endl;
	decltype(auto) w2 = [&a]()->decltype(auto){ return (a); }();
	std::cout << boost::typeindex::type_id_with_cvr< decltype(w2) >().pretty_name() << std::endl;

	logsys::log([](stdlogb& os){ os << "lambda"; }, []{  });
	decltype(auto) v2 = logsys::log([](stdlogb& os){ os << "lambda"; }, []{ return 1; });
	std::cout << boost::typeindex::type_id_with_cvr< decltype(v2) >().pretty_name() << std::endl;
	decltype(auto) v3 = logsys::log([](stdlogb& os){ os << "lambda"; }, [&a]()->decltype(auto){ return (a); });
	std::cout << boost::typeindex::type_id_with_cvr< decltype(v3) >().pretty_name() << std::endl;

	decltype(auto) x1 = logsys::exception_catching_log([](stdlogb& os){ os << "exception_catching"; }, []{  });
	std::cout << boost::typeindex::type_id_with_cvr< decltype(x1) >().pretty_name() << std::endl;
	decltype(auto) x2 = logsys::exception_catching_log([](stdlogb& os){ os << "exception_catching"; }, []{ return 1; });
	std::cout << boost::typeindex::type_id_with_cvr< decltype(x2) >().pretty_name() << std::endl;
	decltype(auto) x3 = logsys::exception_catching_log([](stdlogb& os){ os << "exception_catching"; }, [&a]()->decltype(auto){ return (a); });
	std::cout << boost::typeindex::type_id_with_cvr< decltype(x3) >().pretty_name() << std::endl;

	decltype(auto) y1 = logsys::exception_catching_log([](stdlogb& os){ os << "exception_catching"; }, []{ throw std::runtime_error("test"); });
	std::cout << boost::typeindex::type_id_with_cvr< decltype(y1) >().pretty_name() << std::endl;
	decltype(auto) y2 = logsys::exception_catching_log([](stdlogb& os){ os << "exception_catching"; }, []{ throw std::logic_error("test"); return 1; });
	std::cout << boost::typeindex::type_id_with_cvr< decltype(y2) >().pretty_name() << std::endl;
	decltype(auto) y3 = logsys::exception_catching_log([](stdlogb& os){ os << "exception_catching"; }, [&a]()->decltype(auto){ throw std::out_of_range("test"); return (a); });
	std::cout << boost::typeindex::type_id_with_cvr< decltype(y3) >().pretty_name() << std::endl;

	decltype(auto) z1 = logsys::exception_catching_log([](stdlogb& os){ os << "exception_catching"; }, []{ throw ""; });
	std::cout << boost::typeindex::type_id_with_cvr< decltype(z1) >().pretty_name() << std::endl;
	decltype(auto) z2 = logsys::exception_catching_log([](stdlogb& os){ os << "exception_catching"; }, []{ throw 5; return 1; });
	std::cout << boost::typeindex::type_id_with_cvr< decltype(z2) >().pretty_name() << std::endl;
	decltype(auto) z3 = logsys::exception_catching_log([](stdlogb& os){ os << "exception_catching"; }, [&a]()->decltype(auto){ throw 3.9; return (a); });
	std::cout << boost::typeindex::type_id_with_cvr< decltype(z3) >().pretty_name() << std::endl;
}
