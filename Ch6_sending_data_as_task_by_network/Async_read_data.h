#pragma once
#include <boost/asio/read.hpp>
#include <boost/function.hpp>
#include <boost/lambda/construct.hpp>

#include "Task_processor.h"
#include "Task_wrapped_with_connection.h"

template <class Functor>
void async_read_data(connection_ptr&& c, const Functor& f, std::size_t at_least_bytes)
{
    c->data.resize(at_least_bytes);
    boost::asio::ip::tcp::socket& s = c->socket;

    std::string& d = c->data;
    char* p        = (d.empty() ? 0 : &d[0]);

    boost::asio::async_read(s, boost::asio::buffer(p, d.size()),
                            task_wrapped_with_connection<Functor>(std::move(c), f));
}

template <class Functor>
void async_read_data_at_least(connection_ptr&& c, const Functor& f, std::size_t at_least_bytes,
                              std::size_t at_most)
{
    std::string& d = c->data;
    c->data.resize(at_most);

    char* p = (at_most == 0 ? 0 : &d[0]);

    boost::asio::ip::tcp::socket& s = c->socket;

    boost::asio::async_read(s, boost::asio::buffer(p, at_most),
                            boost::asio::transfer_at_least(at_least_bytes),
                            task_wrapped_with_connection<Functor>(std::move(c), f));
}