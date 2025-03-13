#include "blog.h"

#include <spdlog/sinks/callback_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "webSocketClient.h"

bool Blog::init(const std::string& logger_name, const std::string& filename, size_t max_file_size, size_t max_files)
{
    if (file_impl_ || console_impl_)
    {
        return false;
    }
    file_impl_ = spdlog::rotating_logger_mt(logger_name, filename, max_file_size, max_files);
    if (!file_impl_)
    {
        return false;
    }
    console_impl_ = spdlog::stdout_color_mt("console");
    if (!console_impl_)
    {
        return false;
    }
    file_impl_->set_level(spdlog::level::info);
    console_impl_->set_level(spdlog::level::info);

    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e][thread %t][%s:%#][%l]: %v");

    client_impl_ = make_shared<webSocketClient>();
    client_impl_->connect("127.0.0.1", 8765);
    client_impl_->start();

    auto callback_sink = std::make_shared<spdlog::sinks::callback_sink_mt>(
        [this](const spdlog::details::log_msg& msg) {
            client_impl_->send_by_cache({msg.payload.data(), msg.payload.size()});
        });

    file_impl_->sinks().push_back(callback_sink);

    return true;
}

void Blog::set_level(loglevel log_level)
{
    file_impl_->set_level(static_cast<spdlog::level::level_enum>(log_level));
    console_impl_->set_level(static_cast<spdlog::level::level_enum>(log_level));
}

void Blog::log_i(const source_location& loc, loglevel level, const char* fmt, S s1)
{
    spdlog::source_loc sl{loc.file_name(), int(loc.line()), loc.function_name()};
    auto               rfmt = fmt::runtime(fmt);
    if (enable_file_)
    {
        file_impl_->log(static_cast<spdlog::level::level_enum>(level), rfmt, s1);
    }
    if (enable_console_)
    {
        console_impl_->log(static_cast<spdlog::level::level_enum>(level), rfmt, s1);
    }
}
void Blog::log_i(const source_location& loc, loglevel level, const char* fmt, S s1, S s2)
{
    spdlog::source_loc sl{loc.file_name(), int(loc.line()), loc.function_name()};
    auto               rfmt = fmt::runtime(fmt);
    if (enable_console_)
    {
        console_impl_->log(sl, static_cast<spdlog::level::level_enum>(level), rfmt, s1, s2);
    }
    if (enable_file_)
    {
        file_impl_->log(sl, static_cast<spdlog::level::level_enum>(level), rfmt, s1, s2);
    }
}
void Blog::log_i(const source_location& loc, loglevel level, const char* fmt, S s1, S s2, S s3)
{
    spdlog::source_loc sl{loc.file_name(), int(loc.line()), loc.function_name()};
    auto               rfmt = fmt::runtime(fmt);
    if (enable_console_)
    {
        console_impl_->log(sl, static_cast<spdlog::level::level_enum>(level), rfmt, s1, s2, s3);
    }
    if (enable_file_)
    {
        file_impl_->log(sl, static_cast<spdlog::level::level_enum>(level), rfmt, s1, s2, s3);
    }
}
void Blog::log_i(const source_location& loc, loglevel level, const char* fmt, S s1, S s2, S s3, S s4)
{
    spdlog::source_loc sl{loc.file_name(), int(loc.line()), loc.function_name()};
    auto               rfmt = fmt::runtime(fmt);
    if (enable_console_)
    {
        console_impl_->log(sl, static_cast<spdlog::level::level_enum>(level), rfmt, s1, s2, s3, s4);
    }
    if (enable_file_)
    {
        file_impl_->log(sl, static_cast<spdlog::level::level_enum>(level), rfmt, s1, s2, s3, s4);
    }
}
void Blog::log_i(const source_location& loc, loglevel level, const char* fmt, S s1, S s2, S s3, S s4, S s5)
{
    spdlog::source_loc sl{loc.file_name(), int(loc.line()), loc.function_name()};
    auto               rfmt = fmt::runtime(fmt);
    if (enable_console_)
    {
        console_impl_->log(sl, static_cast<spdlog::level::level_enum>(level), rfmt, s1, s2, s3, s4, s5);
    }
    if (enable_file_)
    {
        file_impl_->log(sl, static_cast<spdlog::level::level_enum>(level), rfmt, s1, s2, s3, s4, s5);
    }
}
void Blog::log_i(const source_location& loc, loglevel level, const char* fmt, S s1, S s2, S s3, S s4, S s5, S s6)
{
    spdlog::source_loc sl{loc.file_name(), int(loc.line()), loc.function_name()};
    auto               rfmt = fmt::runtime(fmt);
    if (enable_console_)
    {
        console_impl_->log(sl, static_cast<spdlog::level::level_enum>(level), rfmt, s1, s2, s3, s4, s5, s6);
    }
    if (enable_file_)
    {
        file_impl_->log(sl, static_cast<spdlog::level::level_enum>(level), rfmt, s1, s2, s3, s4, s5, s6);
    }
}

void Blog::log_i(const source_location& loc, loglevel level, const char* fmt, S s1, S s2, S s3, S s4, S s5, S s6, S s7)
{
    spdlog::source_loc sl{loc.file_name(), int(loc.line()), loc.function_name()};
    auto               rfmt = fmt::runtime(fmt);
    if (enable_console_)
    {
        console_impl_->log(sl, static_cast<spdlog::level::level_enum>(level), rfmt, s1, s2, s3, s4, s5, s6, s7);
    }
    if (enable_file_)
    {
        file_impl_->log(sl, static_cast<spdlog::level::level_enum>(level), rfmt, s1, s2, s3, s4, s5, s6, s7);
    }
}

void Blog::log_i(const source_location& loc, loglevel level, const char* fmt, S s1, S s2, S s3, S s4, S s5, S s6, S s7,
                 S s8)
{
    spdlog::source_loc sl{loc.file_name(), int(loc.line()), loc.function_name()};
    auto               rfmt = fmt::runtime(fmt);
    if (enable_console_)
    {
        console_impl_->log(sl, static_cast<spdlog::level::level_enum>(level), rfmt, s1, s2, s3, s4, s5, s6, s7, s8);
    }
    if (enable_file_)
    {
        file_impl_->log(sl, static_cast<spdlog::level::level_enum>(level), rfmt, s1, s2, s3, s4, s5, s6, s7, s8);
    }
}

void Blog::log(const source_location& loc, loglevel level, const std::string& msg)
{
    spdlog::source_loc sl{loc.file_name(), int(loc.line()), loc.function_name()};
    if (enable_console_)
    {
        console_impl_->log(sl, static_cast<spdlog::level::level_enum>(level), msg);
    }
    if (enable_file_)
    {
        file_impl_->log(sl, static_cast<spdlog::level::level_enum>(level), msg);
    }
}
void Blog::log(const source_location& loc, loglevel level, const std::string_view msg)
{
    spdlog::source_loc sl{loc.file_name(), int(loc.line()), loc.function_name()};
    if (enable_console_)
    {
        console_impl_->log(sl, static_cast<spdlog::level::level_enum>(level), msg);
    }
    if (enable_file_)
    {
        file_impl_->log(sl, static_cast<spdlog::level::level_enum>(level), msg);
    }
}

Blog::~Blog() {}

Blog& Blog::instance()
{
    static Blog blog;
    return blog;
}