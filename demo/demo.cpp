#include "blog.h"
#include "webSocketClient.h"

int main()
{
    Blog blog;
    blog.init("test", "test.log");
    blog.set_level(loglevel::debug);
    blog.debug("xxx {} ss {} {} {} {}", "1", 2, 3, 4, 5, 6, 7, 8);

    blog.info("xxx {} ss {}", "aaa", 33);
    blog.warn("werew");
    std::string        s  = "xxx {} ss {}";
    const std::string& ss = s;
    blog.info(s);
    blog.error(ss);
    blog.error(34);
    blog.error(3.4);

    // webSocketClient client;
    // client.connect("127.0.0.1", 8765);
    // client.start();
    // client.send_by_cache("hello");
    // client.send_by_cache("world");
    // int i = 0;
    // while (i < 100000)
    // {
    //     ++i;
    //     client.send_by_cache("hello" + std::to_string(i));
    // }

    std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;
}