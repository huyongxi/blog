#include "webSocketClient.h"

bool webSocketClient::send_by_cache(const string_view message)
{
    if (!is_running_)
    {
        return false;
    }
    return send_queue_.enqueue(string(message));
}

webSocketClient::~webSocketClient()
{
    session_->close();
    stop();
}

void webSocketClient::start()
{
    io_loop_thread_ = thread([this]() { ioc_.run(); });
    is_running_     = true;
    send_thread_    = thread(
        [this]()
        {
            if (session_->wait_connect_complete())
            {
                std::cout << "connect success" << std::endl;
            } else
            {
                std::cout << "connect fail" << std::endl;
                is_running_ = false;
            }
            while (is_running_)
            {
                string message;
                if (send_queue_.wait_dequeue_timed(message, std::chrono::microseconds(100)))
                {
                    if (!session_->write_packet(message).get())
                    {
                        std::cout << "fail send message: " << message << std::endl;
                        is_running_ = false;
                    }
                }
            }
        });
}
void webSocketClient::stop()
{
    is_running_ = false;
    if (send_thread_.joinable())
    {
        send_thread_.join();
    }
    ioc_.stop();
    if (io_loop_thread_.joinable())
    {
        io_loop_thread_.join();
    }
}

void webSocketClient::connect(const string& host, uint16_t port)
{
    session_ = make_shared<Session>(ioc_);
    session_->connect(host, std::to_string(port));
}