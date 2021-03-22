#pragma once
#define BOOST_ASIO_DESABLE_TYPE_REQUIREMENTS
#include <boost/asio/write.hpp>

#include "Connection_with_data.h"

typedef std::unique_ptr<connection_with_data> connection_ptr;

template <class T>
struct task_wrapped_with_connection
{
private:
    connection_ptr c_;
    T task_unwrapped_;

public:
    explicit task_wrapped_with_connection(connection_ptr&& c, const T& f)
        : c_(std::move(c)), task_unwrapped_(f)
    {
    }

    void operator()(const boost::system::error_code& error, std::size_t bytes_count)
    { /*
             c_->data.resize(bytes_count);
             task_unwrapped_(std::move(c_), error);*/
        const auto lambda = [this, &error, bytes_count]() {
            this->c_->data.resize(bytes_count);
            this->task_unwrapped_(std::move(this->c_), error);
        };

        const auto task = detail::make_task_wrapped(lambda);
        task();
    }
};