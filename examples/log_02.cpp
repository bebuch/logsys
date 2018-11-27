#include <logsys/log.hpp>
#include <logsys/stdlog.hpp>

int main()try{
    logsys::log(
        [throw_exception = true](logsys::stdlog& log){
            log << "Hello ";
            if(throw_exception){
                throw std::logic_error("throw in log fn!");
            }
            log << "World!";
        });
}catch(std::exception const& e){
    std::cerr << "program terminated with exception: " << e.what() << '\n';
}
