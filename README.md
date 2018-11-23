# logsys

A C++17 high performance log messages library.

You can use this libery in two different ways. As header-only library or as linkable library.

If you want to use the log library in another dynamic link libary, the linkable approach is the way to go. You will lose a little performance in exchange for the configurability of the log library in the main application that uses your library.

If you want to use the log library in a application project you can configure the log library completely at compile time and use the header-only approach.

## Configuration

## Usage

There are three functions you need to know:

- Log a message
- Log a message bound to a code block
- Log a message bound to a code block and catch any exceptions

### Log a message

```
auto log(LogCallable&& message_producer)noexcept
-> void;
```

### Log a message bound to a code block

```
auto log(LogCallable&& message_producer, Body&& body)
-> std::invoke_result_t< Body&& >;
```

### Log a message bound to a code block and catch any exceptions

```
auto exception_catching_log(LogCallable&& message_producer, Body&& body)noexcept
-> std::conditional_t< std::is_void_v< std::invoke_result_t< Body&& > >,
    bool, optional< std::invoke_result_t< Body&& > > >
```

## License notice

This software was originally developed privately by Benjamin Buch. All changes are released under the Boost Software License - Version 1.0 and published on GitHub.

https://github.com/bebuch/logsys
