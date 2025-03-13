#pragma once
#include <blockingconcurrentqueue.h>

#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <cstdlib>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>

namespace beast     = boost::beast;
namespace http      = beast::http;
namespace websocket = beast::websocket;
namespace net       = boost::asio;
using tcp           = boost::asio::ip::tcp;

using std::future;
using std::promise;
using std::shared_ptr;
using std::string;
using std::string_view;
using std::thread;

using std::make_shared;

using moodycamel::BlockingConcurrentQueue;

class Session : public std::enable_shared_from_this<Session>
{
   private:
    tcp::resolver                        resolver_;
    beast::flat_buffer                   buffer_;
    std::string                          host_;
    string                               send_buffer_;
    promise<void>                        connect_complete_promise_;
    websocket::stream<beast::tcp_stream> ws_;

   public:
    virtual void write_complete() {}
    virtual void connect_complete() {}
    virtual void on_message(const string_view message)
    {
        std::cout << "on_message: " << message << std::endl;
    }

    explicit Session(net::io_context& ioc) : resolver_(net::make_strand(ioc)), ws_(net::make_strand(ioc)) {}

    void connect(const string& host, const string& port)
    {
        host_ = host;
        resolver_.async_resolve(host, port, beast::bind_front_handler(&Session::on_resolve, shared_from_this()));
    }

    void wait_connect_complete()
    {
        connect_complete_promise_.get_future().wait();
    }

    void close()
    {
        ws_.async_close(websocket::close_code::normal,
                        beast::bind_front_handler(&Session::on_close, shared_from_this()));
    }

    future<bool> write_packet(string_view message)
    {
        auto pm = make_shared<promise<bool>>();
        send_buffer_.assign(message.data(), message.size());
        ws_.async_write(net::buffer(send_buffer_),
                        beast::bind_front_handler(&Session::on_write_packet, shared_from_this(), pm));
        return pm->get_future();
    }

    void on_resolve(beast::error_code ec, tcp::resolver::results_type results)
    {
        if (ec)
        {
            return fail(ec, "resolve");
        }

        beast::get_lowest_layer(ws_).expires_after(std::chrono::seconds(30));

        beast::get_lowest_layer(ws_).async_connect(results,
                                                   beast::bind_front_handler(&Session::on_connect, shared_from_this()));
    }

    void on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type ep)
    {
        if (ec)
        {
            return fail(ec, "connect");
        }
        beast::get_lowest_layer(ws_).expires_never();
        ws_.set_option(websocket::stream_base::timeout::suggested(beast::role_type::client));
        ws_.set_option(websocket::stream_base::decorator(
            [](websocket::request_type& req) {
                req.set(http::field::user_agent, std::string(BOOST_BEAST_VERSION_STRING) + " websocket-client-async");
            }));
        host_ += ':' + std::to_string(ep.port());
        ws_.async_handshake(host_, "/", beast::bind_front_handler(&Session::on_handshake, shared_from_this()));
    }

    void on_handshake(beast::error_code ec)
    {
        if (ec)
        {
            return fail(ec, "handshake");
        }
        ws_.async_read(buffer_, beast::bind_front_handler(&Session::on_read, shared_from_this()));
        connect_complete();
        connect_complete_promise_.set_value();
    }

    void on_write_packet(shared_ptr<promise<bool>> pm, beast::error_code ec, std::size_t bytes_transferred)
    {
        if (ec)
        {
            pm->set_value(false);
            return fail(ec, "write");
        }
        if (bytes_transferred == send_buffer_.size())
        {
            write_complete();
            pm->set_value(true);
        } else if (bytes_transferred < send_buffer_.size())
        {
            std::copy(send_buffer_.begin() + bytes_transferred, send_buffer_.end(), send_buffer_.begin());
            send_buffer_.resize(send_buffer_.size() - bytes_transferred);
            ws_.async_write(net::buffer(send_buffer_),
                            beast::bind_front_handler(&Session::on_write_packet, shared_from_this(), pm));
        } else
        {
            assert(true);
        }
    }

    void on_read(beast::error_code ec, std::size_t bytes_transferred)
    {
        if (ec)
        {
            return fail(ec, "read");
        }
        const auto& data = buffer_.data();
        on_message({reinterpret_cast<const char*>(data.data()), data.size()});
        buffer_.consume(bytes_transferred);
        ws_.async_read(buffer_, beast::bind_front_handler(&Session::on_read, shared_from_this()));
    }

    void on_close(beast::error_code ec)
    {
        if (ec)
        {
            return fail(ec, "close");
        }
    }

    void fail(beast::error_code ec, char const* what)
    {
        std::cerr << what << ": " << ec.message() << "\n";
    }
    virtual ~Session() {}
};

class webSocketClient
{
   public:
    webSocketClient() = default;
    ~webSocketClient();
    bool send_by_cache(const string_view message);
    void start();
    void stop();
    void connect(const string& host, uint16_t port);

   private:
    thread          io_loop_thread_;
    thread          send_thread_;
    net::io_context ioc_;
    bool            is_running_{false};

    shared_ptr<Session>             session_;
    BlockingConcurrentQueue<string> send_queue_;
};