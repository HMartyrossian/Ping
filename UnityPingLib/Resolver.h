#pragma once

#include <boost/asio.hpp>

#include "icmp_header.hpp"
#include "ipv4_header.hpp"

using boost::asio::ip::icmp;

class CResolver
{
public:
	CResolver(boost::asio::io_context& io_context, const char* destination) : resolver(io_context)
	{
	}

private:
	icmp::resolver resolver;
};
