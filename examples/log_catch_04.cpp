#include <logsys/log.hpp>
#include <logsys/stdlog.hpp>

int main(){
    std::optional< int > value = logsys::exception_catching_log(
        [](logsys::stdlog& log){
            log << "calculate 5+5";
        },
        [throw_exception = true]{
            if(throw_exception){
                throw std::runtime_error("something failed");
            }
            return 5 + 5;
        });

    auto success = static_cast< bool >(value);
    std::cout << std::boolalpha << "success: " << success << '\n';

    if(success){
        std::cout << "Value: " << *value << '\n';
    }
}
