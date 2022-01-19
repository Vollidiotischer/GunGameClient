#pragma once

#include <iostream>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp> 



struct ClientClass {

	std::string ip;
	int port;

	asio::ip::tcp::socket* socket;
	asio::io_context* ctx;
	asio::error_code* ec;
	asio::ip::tcp::endpoint* endpoint;

	ClientClass(std::string ip, int port) {
		this->ip = ip;
		this->port = port;
		connect();
	}


	void connect() {

		ec = new asio::error_code();
		ctx = new asio::io_context();

		endpoint = new asio::ip::tcp::endpoint(asio::ip::make_address(ip, *ec), port);

		socket = new asio::ip::tcp::socket(*ctx);

		socket->connect(*endpoint, *ec);

		std::cout << "Error_Code: " << (*ec).message() << "\nConnection Status: " << socket->is_open() << std::endl;

	}

	void send(std::string s) {

		socket->write_some(asio::buffer(s.data(), s.size()), *ec);

	}

	std::string recieve() { // std::string& message

		asio::streambuf buf;
		asio::read_until(*socket, buf, '!');

		std::ostringstream data; 
		data << &buf;

		std::string s = data.str(); 
		//std::string data = asio::buffer_cast<const char*>(buf.data()); 

		send("rec!"); 

		return s; 

	}

	void clientCleanup() {
		delete socket;
		delete ec;
		delete ctx;
		delete endpoint;
	}

};