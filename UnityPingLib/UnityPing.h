#pragma once

#include <istream>
#include <iostream>
#include <string>
#include <ostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "icmp_header.hpp"
#include "ipv4_header.hpp"


class CUnityPing
{
public:
	CUnityPing(/*boost::asio::io_context& io_context, */const char* destination);
	virtual ~CUnityPing();

	// Public interface:
	bool Resovle(void);
	void Ping(void);

	// Getters:
	bool Success(void);
	long long Time(void);

private:
	static unsigned short GetIdentifier(void);

	static const std::string body;

	boost::asio::io_context ioContext;

	boost::asio::ip::icmp::resolver resolver;
	boost::asio::ip::icmp::socket socket;
	boost::asio::steady_timer timer;

	boost::asio::ip::icmp::endpoint destination;

	const char* destinationStr;

	void StartSend(void);
	void StartReceive(void);

	void HandleTimeout(void);
	void HandleReceive(std::size_t length);

	unsigned short sequenceNumber = 0;

	std::size_t numReplies;
	std::size_t numRetries = 4;

	boost::asio::streambuf replyBuffer;
	boost::asio::chrono::steady_clock::time_point timeSent;

	bool success = false;
	long long time = 0;
};
