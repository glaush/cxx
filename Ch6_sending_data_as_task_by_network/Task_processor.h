#pragma once
#include <memory>  // std::unique_ptr

#include "Task_wrapped.h"
#include "Task_wrapped_with_connection.h"

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
   
    template <class Functor>
    static void add_listener(unsigned short port_num, const Functor& f)
    {
        std::unique_ptr<tcp_listener> listener(new tcp_listener(get_ios(), port_num, f));

        start_accepting_connection(std::move(listener));
    }

private:

    typedef boost::asio::ip::tcp::acceptor acceptor_t;

    typedef boost::function<void(connection_ptr, const boost::system::error_code&)>
        on_accpet_func_t;

     struct tcp_listener
    {
        acceptor_t acceptor_;
        const on_accpet_func_t func_;
        connection_ptr new_c_;

        template <class Functor>
        tcp_listener(boost::asio::io_service& io_service, unsigned short port,
                     const Functor& task_unwrapped)
            : acceptor_(io_service,
                        boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
              func_(task_unwrapped)
        {
        }
    };

    typedef std::unique_ptr<tcp_listener> listener_ptr;

      static void start_accepting_connection(listener_ptr&& listener)
    {
        if (!listener->acceptor_.is_open())
        {
            return;
        }

        listener->new_c_.reset(new connection_with_data(listener->acceptor_.get_executor()));

        boost::asio::ip::tcp::socket& s = listener->new_c_->socket;
        acceptor_t& a                   = listener->acceptor_;
        a.async_accept(s, tasks_processor::handle_accept(std::move(listener)));
    }

    struct handle_accept
    {
        listener_ptr listener;

        explicit handle_accept(listener_ptr&& l) : listener(std::move(l)) {}

        void operator()(const boost::system::error_code& error)
        {
            task_wrapped_with_connection<on_accpet_func_t> task(std::move(listener->new_c_),
                                                                listener->func_);

            start_accepting_connection(std::move(listener));
            task(error, 0);
        }
    };

 
protected:
    static boost::asio::io_service& get_ios()
    {
        static boost::asio::io_service ios;
        static boost::asio::io_service::work work(ios);

        return ios;
    }
};