#pragma once
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>

#if __has_builtin(__builtin_source_location)
#include <source_location>
using std::source_location;
#else
#include <experimental/source_location>
using std::experimental::source_location;
#endif

#define LEVEL_TRACE 0
#define LEVEL_DEBUG 1
#define LEVEL_INFO 2
#define LEVEL_WARN 3
#define LEVEL_ERROR 4
#define LEVEL_CRITICAL 5
#define LEVEL_OFF 6

#define LOG Blog::instance()

namespace spdlog
{
class logger;
}  // namespace spdlog

class webSocketClient;

enum loglevel : int
{
    trace    = LEVEL_TRACE,
    debug    = LEVEL_DEBUG,
    info     = LEVEL_INFO,
    warn     = LEVEL_WARN,
    err      = LEVEL_ERROR,
    critical = LEVEL_CRITICAL,
    off      = LEVEL_OFF,
    n_levels
};

template <typename T>
std::string to_string_(const T& value)
{
    std::ostringstream oss;
    oss << value;  // 依赖类型是否支持 operator<<
    return oss.str();
}

struct ArgsWithLocation
{
    source_location        loc;
    const char*            fmt{nullptr};
    std::string            msg;
    const std::string_view msg_view;
    template <typename T>
    ArgsWithLocation(const T& logmsg, const source_location& loc = source_location::current())
        : loc(std::move(loc)), msg(to_string_(logmsg)), msg_view(msg)
    {
    }
    ArgsWithLocation(const std::string& msg, const source_location& loc = source_location::current())
        : loc(std::move(loc)), msg_view(msg)
    {
    }
    ArgsWithLocation(const char* fmt, const source_location& loc = source_location::current())
        : loc(std::move(loc)), fmt(fmt), msg_view(fmt)
    {
    }
};

class Blog
{
   public:
    Blog() = default;

    bool init(const std::string& logger_name, const std::string& filename, size_t max_file_size = 1024 * 1024 * 5,
              size_t max_files = 3);
    void set_level(loglevel log_level);
    void set_enable_console(bool enable_console)
    {
        enable_console_ = enable_console;
    }
    void set_enable_file(bool enable_file)
    {
        enable_file_ = enable_file;
    }

    static Blog& instance();

    template <typename... Args>
    void log(loglevel level, const ArgsWithLocation& arg, Args&&... args)
    {
        static_assert(sizeof...(args) <= 8, "Only up to 8 arguments are supported");
        log_i(arg.loc, level, arg.fmt, to_string_(std::forward<Args>(args))...);
    }

    void log(loglevel level, const ArgsWithLocation& arg)
    {
        log(arg.loc, level, arg.msg_view);
    }

    template <typename... Args>
    void trace(ArgsWithLocation fmt, Args&&... args)
    {
        log(loglevel::trace, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void debug(ArgsWithLocation fmt, Args&&... args)
    {
        log(loglevel::debug, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void info(ArgsWithLocation fmt, Args&&... args)
    {
        log(loglevel::info, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void warn(ArgsWithLocation fmt, Args&&... args)
    {
        log(loglevel::warn, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void error(ArgsWithLocation fmt, Args&&... args)
    {
        log(loglevel::err, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void critical(ArgsWithLocation fmt, Args&&... args)
    {
        log(loglevel::critical, fmt, std::forward<Args>(args)...);
    }

    template <typename T>
    void trace(ArgsWithLocation msg)
    {
        log(loglevel::trace, msg);
    }

    template <typename T>
    void debug(ArgsWithLocation msg)
    {
        log(loglevel::debug, msg);
    }

    template <typename T>
    void info(ArgsWithLocation msg)
    {
        log(loglevel::info, msg);
    }

    template <typename T>
    void warn(ArgsWithLocation msg)
    {
        log(loglevel::warn, msg);
    }

    template <typename T>
    void error(ArgsWithLocation msg)
    {
        log(loglevel::err, msg);
    }

    template <typename T>
    void critical(ArgsWithLocation msg)
    {
        log(loglevel::critical, msg);
    }

    ~Blog();

   private:
    void log(const source_location&, loglevel, const std::string&);
    void log(const source_location&, loglevel, const std::string_view);

    using S = const std::string&;
    void log_i(const source_location&, loglevel, const char*, S);
    void log_i(const source_location&, loglevel, const char*, S, S);
    void log_i(const source_location&, loglevel, const char*, S, S, S);
    void log_i(const source_location&, loglevel, const char*, S, S, S, S);
    void log_i(const source_location&, loglevel, const char*, S, S, S, S, S);
    void log_i(const source_location&, loglevel, const char*, S, S, S, S, S, S);
    void log_i(const source_location&, loglevel, const char*, S, S, S, S, S, S, S);
    void log_i(const source_location&, loglevel, const char*, S, S, S, S, S, S, S, S);

   private:
    std::shared_ptr<spdlog::logger>  file_impl_{nullptr};
    std::shared_ptr<spdlog::logger>  console_impl_{nullptr};
    std::shared_ptr<webSocketClient> client_impl_{nullptr};
    bool                             enable_console_{true};
    bool                             enable_file_{true};
};
