#include <logsys/log.hpp>
#include <logsys/stdlog.hpp>

int main(){
    logsys::log(
        [](logsys::stdlog& log){
            log << "Hello World!";
        });
}
