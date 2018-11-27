#include <logsys/log.hpp>
#include <logsys/stdlog.hpp>

int main(){
    logsys::log(
        [](logsys::stdlog& log){
            log << "calculate 5+5";
        },
        []{
            std::cout << "Hello World!\n";
        });
}
