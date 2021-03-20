#pragma once
#include <memory>  // std::unique_ptr

#include "Connection_with_data.h"
#include "Task_wrapped.h"

typedef std::unique_ptr<connection_with_data> connection_ptr;

class tasks_processor
{
public:
    template <class T>
    static void push_task(const T& task_unwrapped)
    {
        get_ios().post(detail::make_task_wrapped(task_unwrapped));
    }

    static void start() { get_ios().run(); }

    static void stop() { get_ios().stop(); }

    static connection_ptr create_connection(const char* addr, unsigned short port_num)
    {
        connection_ptr c(new connection_with_data(get_ios()));
        c->socket.connect(boost::asio::ip::tcp::endpoint(
            boost::asio::ip::address_v4::from_string(addr), port_num));

        return c;
    }

protected:
    static boost::asio::io_service& get_ios()
    {
        static boost::asio::io_service ios;
        static boost::asio::io_service::work work(ios);

        return ios;
    }
};