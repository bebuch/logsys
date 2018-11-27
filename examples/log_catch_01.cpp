#include <logsys/log.hpp>
#include <logsys/stdlog.hpp>

int main(){
    bool success = logsys::exception_catching_log(
        [](logsys::stdlog& log){
            log << "calculate 5+5";
        },
        []{
            std::cout << "Hello World!\n";
        });

    std::cout << std::boolalpha << "success: " << success << '\n';
}
