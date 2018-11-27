# logsys

A C++17 high performance log messages library.

## Configuration

You can use this libery in two different ways. As **header-only** library **or** as **linkable** library.

If you want to use the log library in a application project, you can configure the log library completely at compile time and use the header-only approach.

If you want to use the log library in another dynamic link libary, the linkable approach is the way to go. You will lose a little performance in exchange for the configurability of the log library in the main application that uses your library.

## Usage

There are three functions you need to know:

- Log a message
- Log a message bound to a code block
- Log a message bound to a code block and catch any exceptions

### Log a message

```cpp
template < typename LogFn >
void
log(LogFn&& message_producer)
noexcept;
```

- **throw:** Never

#### Example 1

```cpp
#include <logsys/log.hpp>
#include <logsys/stdlog.hpp>

int main(){
    logsys::log(
        [](logsys::stdlog& log){
            log << "Hello World!";
        });
}
```

**Output**

```text
000000  ( no content     ) Hello World!
```

#### Example 2

```cpp
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
```

**Output**

```text
000000  ( no content     )  LOG FAILED: [std::logic_error] throw in log fn!; Probably incomplete log message: 'Hello '
```

### Log a message bound to a code block

```cpp
template < typename LogFn, typename BodyFn >
std::invoke_result_t< BodyFn& >
log(LogFn&& message_producer, BodyFn&& body_fn)
noexcept(std::is_nothrow_invocable_v< BodyFn& >);
```

- **throw:** Same as `body_fn()`

#### Example 1

```cpp
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
```

**Output**

```text
Hello World!
000000 2018-11-27 19:04:01 563.688 (        0.053ms ) calculate 5+5
```

#### Example 2

```cpp
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
```

**Output**

```text
000000 2018-11-27 19:04:03 133.813 (        0.001ms ) calculate 5+5
Value: 10
```

#### Example 3

```cpp
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
```

**Output**

```text
000000 2018-11-27 19:04:04 200.633 (        0.081ms ) calculate 5+5 (BODY FAILED: [std::runtime_error] something failed)
program terminated with exception: something failed
```

### Log a message bound to a code block and catch any exceptions

```cpp
template < typename LogFn, typename BodyFn >
logsys::optional< std::invoke_result_t< BodyFn& > >
exception_catching_log(LogFn&& message_producer, BodyFn&& body_fn)
noexcept;
```

#### Example 1

```cpp
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
```

**Output**

```text
Hello World!
000000 2018-11-27 19:05:24 104.544 (        0.061ms ) calculate 5+5
success: true
```

#### Example 2

```cpp
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
```

**Output**

```text
000000 2018-11-27 19:05:25 673.545 (        0.079ms ) calculate 5+5 (BODY FAILED: [std::runtime_error] something failed)
success: false
```

#### Example 3

```cpp
#include <logsys/log.hpp>
#include <logsys/stdlog.hpp>

int main(){
    std::optional< int > value = logsys::exception_catching_log(
        [](logsys::stdlog& log){
            log << "calculate 5+5";
        },
        []{
            return 5 + 5;
        });

    auto success = static_cast< bool >(value);
    std::cout << std::boolalpha << "success: " << success << '\n';

    if(success){
        std::cout << "Value: " << *value << '\n';
    }
}
```

**Output**

```text
Hello World!
000000 2018-11-27 19:05:26 690.952 (        0.000ms ) calculate 5+5
success: true
Value: 10
```

#### Example 4

```cpp
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
```

**Output**

```text
000000 2018-11-27 19:05:27 526.722 (        0.076ms ) calculate 5+5 (BODY FAILED: [std::runtime_error] something failed)
success: false
```

- **throw:** Never

### Optional type: `logsys::optional< BodyRT >`

The `BodyRT` is the return type of your body function.

- `logsys::optional< void >` is `bool`
- `logsys::optional< T >` is `std::optional< T >`
- `logsys::optional< T& >` is `logsys::optional_lvalue_reference< T >`
- `logsys::optional< T&& >` is `logsys::optional_rvalue_reference< T >`

The last two definitions are uncommon. If you really return a reference from your body function, checkout the definitions of `logsys::optional_lvalue_reference< T >` and `logsys::optional_rvalue_reference< T >` in [`optional.hpp`](include/logsys/optional.hpp). They have a similar interface to `std::optional`.

## License notice

This software was originally developed privately by Benjamin Buch. All changes are released under the Boost Software License - Version 1.0 and published on GitHub.

https://github.com/bebuch/logsys
