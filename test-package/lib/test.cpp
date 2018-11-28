#include <logsys/log.hpp>
#include <logsys/stdlogb.hpp>

int main(){
	logsys::log([](logsys::stdlogb&){});
}
