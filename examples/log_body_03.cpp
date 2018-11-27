#include <logsys/log.hpp>
#include <logsys/stdlog.hpp>

int main()try{
    int value = logsys::log(
        [](logsys::stdlog& log){
            log << "calculate 5+5";
        },
        [throw_exception = true]{
            if(throw_exception){
                throw std::runtime_error("something failed");
            }
            return 5 + 5;
        });

    std::cout << "Value: " << value << '\n';
}catch(std::exception const& e){
    std::cerr << "program terminated with exception: " << e.what() << '\n';
}
