#pragma once
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/core/noncopyable.hpp>

#define BOOST_ASIO_DISABLE_HANDLER_TYPE_REQUIREMENTS

struct connection_with_data : boost::noncopyable
{
    boost::asio::ip::tcp::socket socket;
    std::string data;

    explicit connection_with_data(boost::asio::io_service& ios) : socket(ios) {}

    void shutDown()
    {
        if (!socket.is_open())
        {
            return;
        }

        boost::system::error_code ignore;
        socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignore);

        socket.close(ignore);
    }

    ~connection_with_data() { shutDown(); }
};
