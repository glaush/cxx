#include <iostream>
#include "Async_read_data.h"
#include "Async_write_data.h"

//with using this functions you can create your own
//client-server app

//Client-side
//on_send - client-side callback
//send_auth - func for sending data by socket

//Server-side
//process_server_response - server-side callback
//receive_auth_responce - func for receiving data by socket

void on_send()
{
	std::cout << "I'm a callack from India.\n I love India very much))))))\n";
}

void send_auth()
{
	connection_ptr sock = tasks_processor::create_connection("127.0.0.1", 9000);
	sock->data = "Hello from India))))))))\n";
	//async_write_data(std::move(sock), &on_send);
}

void process_server_response(connection_ptr&& sock, const boost::system::error_code& err)
{
	std::cout << "I'm a callack from Belarus.\n I love cats very much))))))\n";

	if (err && err != boost::asio::error::eof)
	{
		std::cerr << "Client error on receive: " << err.message() << "\n";
		assert(false);
	}

	if (sock->data.size() != 2)
	{
		std::cerr << "Wrong bytes count\n";
		assert(false);
	}

	if (sock->data != "OK")
	{
		std::cerr << "Wrong response: " << sock->data << "\n";
		assert(false);
	}

	sock->shutDown();
	tasks_processor::stop();
}

void receive_auth_responce(connection_ptr&& sock, const::boost::system::error_code& err)
{
	if (err)
	{
		std::cerr << "Error on sending data. Meow: " << err.message() << "\n";
		assert(false);
	}

	async_read_data(std::move(sock), &process_server_response, 2);
}

int main()
{

	return 0;
}