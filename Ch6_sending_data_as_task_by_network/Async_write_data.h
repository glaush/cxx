#pragma once
#include <boost/asio/write.hpp>

#include "Task_processor.h"

template <class T>
struct task_wrapped_with_connection;

template <class Functor>
void async_write_data(connection_ptr&& c, const Functor& f)
{
    boost::asio::ip::tcp::socket& s = c->socket;
    std::string& d                  = c->data;

    boost::asio::async_write(s, boost::asio::buffer(d),
                             task_wrapped_with_connection<Functor>(std::move(c), f));
}