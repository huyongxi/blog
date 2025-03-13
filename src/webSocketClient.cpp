#include "webSocketClient.h"

bool webSocketClient::send_by_cache(const string_view message)
{
    return send_queue_.enqueue(string(message));
}

webSocketClient::~webSocketClient()
{
    session_->close();
    if (is_running_)
    {
        stop();
    }
}

void webSocketClient::start()
{
    io_loop_thread_ = thread([this]() { ioc_.run(); });
    session_->wait_connect_complete();
    is_running_  = true;
    send_thread_ = thread(
        [this]()
        {
            while (is_running_)
            {
                string message;
                if (send_queue_.wait_dequeue_timed(message, std::chrono::milliseconds(100)))
                {
                    if (!session_->write_packet(message).get())
                    {
                        std::cout << "fail send message: " << message << std::endl;
                    }
                }
            }
        });
}
void webSocketClient::stop()
{
    is_running_ = false;
    ioc_.stop();
    io_loop_thread_.join();
    send_thread_.join();
}

void webSocketClient::connect(const string& host, uint16_t port)
{
    session_ = make_shared<Session>(ioc_);
    session_->connect(host, std::to_string(port));
}