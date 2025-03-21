#include <thread>

#include "blog.h"

int main()
{
    BLOG->init("demo_log", "log/demo.log", 1024 * 1024 * 100, 3);
    BLOG->set_level(loglevel::debug);
    //BLOG->enable_log_upload("127.0.0.1", 7654);

    int         a = 1;
    double      b = 2.0;
    float       c = 3.0;
    const char* d = "hello";
    std::string e = "world";

    BLOG->debug("parse line {}", "1");
    BLOG->debug("parse line {}", 1);

    std::this_thread::sleep_for(std::chrono::seconds(3));
    return 1;

    auto func = [&]()
    {
        int i = 0;
        while (i < 3 * 10000)
        {
            BLOG->trace("a: {}, b: {}, c: {}, d: {}, e: {}", a, b, c, d, e);
            BLOG->debug("a: {}, b: {}, c: {}, d: {}, e: {}", a, b, c, d, e);
            BLOG->info("a: {}, b: {}, c: {}, d: {}, e: {}", a, b, c, d, e);
            BLOG->warn("a: {}, b: {}, c: {}, d: {}, e: {}", a, b, c, d, e);
            BLOG->error("a: {}, b: {}, c: {}, d: {}, e: {}", a, b, c, d, e);
            BLOG->critical("a: {}, b: {}, c: {}, d: {}, e: {}", a, b, c, d, e);
            BLOG->critical("test {} {} {} {} {} {} {} {}", 1, 2, 3, 4, 5, 6, 7, 8);

            BLOG->trace(a);
            BLOG->debug(a);
            BLOG->info(a);
            BLOG->warn(a);
            BLOG->error(a);
            BLOG->critical(a);

            BLOG->trace(b);
            BLOG->debug(b);
            BLOG->info(b);
            BLOG->warn(b);
            BLOG->error(b);
            BLOG->critical(b);

            BLOG->trace(c);
            BLOG->debug(c);
            BLOG->info(c);
            BLOG->warn(c);
            BLOG->error(c);
            BLOG->critical(c);

            BLOG->trace(d);
            BLOG->debug(d);
            BLOG->info(d);
            BLOG->warn(d);
            BLOG->error(d);
            BLOG->critical(d);

            BLOG->trace(e);
            BLOG->debug(e);
            BLOG->info(e);
            BLOG->warn(e);
            BLOG->error(e);
            BLOG->critical(e);

            BLOG->log(loglevel::trace, "a: {}, b: {}, c: {}, d: {}, e: {}", a, b, c, d, e);
            BLOG->log(loglevel::debug, "a: {}, b: {}, c: {}, d: {}, e: {}", a, b, c, d, e);
            BLOG->log(loglevel::info, "a: {}, b: {}, c: {}, d: {}, e: {}", a, b, c, d, e);
            BLOG->log(loglevel::warn, "a: {}, b: {}, c: {}, d: {}, e: {}", a, b, c, d, e);
            BLOG->log(loglevel::err, "a: {}, b: {}, c: {}, d: {}, e: {}", a, b, c, d, e);
            BLOG->log(loglevel::critical, "a: {}, b: {}, c: {}, d: {}, e: {}", a, b, c, d, e);
            BLOG->log(loglevel::critical, "test {} {} {} {} {} {} {} {}", 1, 2, 3, 4, 5, 6, 7, 8);

            BLOG->log(loglevel::trace, a);
            BLOG->log(loglevel::debug, a);
            BLOG->log(loglevel::info, a);
            BLOG->log(loglevel::warn, a);
            BLOG->log(loglevel::err, a);
            BLOG->log(loglevel::critical, a);

            BLOG->log(loglevel::trace, b);
            BLOG->log(loglevel::debug, b);
            BLOG->log(loglevel::info, b);
            BLOG->log(loglevel::warn, b);
            BLOG->log(loglevel::err, b);
            BLOG->log(loglevel::critical, b);

            BLOG->log(loglevel::trace, c);
            BLOG->log(loglevel::debug, c);
            BLOG->log(loglevel::info, c);
            BLOG->log(loglevel::warn, c);
            BLOG->log(loglevel::err, c);
            BLOG->log(loglevel::critical, c);

            BLOG->log(loglevel::trace, d);
            BLOG->log(loglevel::debug, d);
            BLOG->log(loglevel::info, d);
            BLOG->log(loglevel::warn, d);
            BLOG->log(loglevel::err, d);
            BLOG->log(loglevel::critical, d);

            BLOG->log(loglevel::trace, e);
            BLOG->log(loglevel::debug, e);
            BLOG->log(loglevel::info, e);
            BLOG->log(loglevel::warn, e);
            BLOG->log(loglevel::err, e);
            BLOG->log(loglevel::critical, e);

            std::this_thread::sleep_for(std::chrono::microseconds(1));
            i++;
        }

        BLOG->info("end");

        std::this_thread::sleep_for(std::chrono::seconds(10));
    };


    std::thread t1(func);
    std::thread t2(func);
    std::thread t3(func);
    std::thread t4(func);
    std::thread t5(func);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    return 0;
}