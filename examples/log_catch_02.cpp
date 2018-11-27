#include <logsys/log.hpp>
#include <logsys/stdlog.hpp>

int main(){
    bool success = logsys::exception_catching_log(
        [](logsys::stdlog& log){
            log << "calculate 5+5";
        },
        []{
            throw std::runtime_error("something failed");
        });

    std::cout << std::boolalpha << "success: " << success << '\n';
}
