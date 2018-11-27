#include <logsys/log.hpp>
#include <logsys/stdlog.hpp>

int main(){
    int value = logsys::log(
        [](logsys::stdlog& log){
            log << "calculate 5+5";
        },
        []{
            return 5 + 5;
        });

    std::cout << "Value: " << value << '\n';
}
