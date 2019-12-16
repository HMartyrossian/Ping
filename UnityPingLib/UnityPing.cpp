#include "stdafx.h"
#include "UnityPing.h"

using boost::asio::ip::icmp;
using boost::asio::steady_timer;

namespace chrono = boost::asio::chrono;

unsigned short CUnityPing::GetIdentifier(void)
{
#if defined(BOOST_ASIO_WINDOWS)
	return static_cast<unsigned short>(::GetCurrentProcessId());
#else
	return static_cast<unsigned short>(::getpid());
#endif
}

const std::string CUnityPing::body("\"Hello!\" from Asio ping.");

CUnityPing::CUnityPing(/*boost::asio::io_context& io_context, */const char* destination) : ioContext(),
	resolver(ioContext),
	socket(ioContext, icmp::v4()),
	timer(ioContext),
	destinationStr(destination) {}

CUnityPing::~CUnityPing() {}

bool CUnityPing::Resovle(void) {
	try {
		destination = *resolver.resolve(icmp::v4(), destinationStr, "").begin();

		return true;
	}
	catch (boost::wrapexcept<boost::system::system_error>&) {
		return false;
	}
}

void CUnityPing::Ping(void) {
	success = false;
	numRetries = 4;

	StartSend();
	StartReceive();

	ioContext.run();
}

void CUnityPing::StartSend(void) {
	// Create an ICMP header for an echo request.
	icmp_header echoRequest;
	echoRequest.type(icmp_header::echo_request);
	echoRequest.code(0);
	echoRequest.identifier(GetIdentifier());
	echoRequest.sequence_number(++sequenceNumber);
	compute_checksum(echoRequest, body.begin(), body.end());

	// Encode the request packet.
	boost::asio::streambuf requestBuffer;
	std::ostream os(&requestBuffer);
	os << echoRequest << body;

	// Send the request.
	timeSent = steady_timer::clock_type::now();
	socket.send_to(requestBuffer.data(), destination);

	// Wait up to five seconds for a reply.
	numReplies = 0;
	timer.expires_at(timeSent + chrono::seconds(5));
	timer.async_wait(boost::bind(&CUnityPing::HandleTimeout, this));
}

void CUnityPing::StartReceive(void) {
	// Discard any data already in the buffer.
	replyBuffer.consume(replyBuffer.size());

	// Wait for a reply. We prepare the buffer to receive up to 64KB.
	socket.async_receive(replyBuffer.prepare(65536), boost::bind(&CUnityPing::HandleReceive, this, _2));
}

void CUnityPing::HandleTimeout(void) {
	if (numReplies == 0) {
		std::cout << "Request timed out" << std::endl;
	}

	if (--numRetries == 0) {
		ioContext.stop();
	}
	else {
		// Requests must be sent no less than one second apart.
		timer.expires_at(timeSent + chrono::seconds(1));
		timer.async_wait(boost::bind(&CUnityPing::StartSend, this));
	}
}

void CUnityPing::HandleReceive(std::size_t length) {
	// The actual number of bytes received is committed to the buffer so that we
	// can extract it using a std::istream object.
	replyBuffer.commit(length);

	// Decode the reply packet.
	std::istream is(&replyBuffer);
	ipv4_header ipv4_hdr;
	icmp_header icmp_hdr;
	is >> ipv4_hdr >> icmp_hdr;

	// We can receive all ICMP packets received by the host, so we need to
	// filter out only the echo replies that match the our identifier and
	// expected sequence number.
	if (is && icmp_hdr.type() == icmp_header::echo_reply
		&& icmp_hdr.identifier() == GetIdentifier()
		&& icmp_hdr.sequence_number() == sequenceNumber)
	{
		// If this is the first reply, interrupt the five second timeout.
		if (numReplies++ == 0) {
			timer.cancel();
		}

		// Print out some information about the reply packet.
		chrono::steady_clock::time_point now = chrono::steady_clock::now();
		chrono::steady_clock::duration elapsed = now - timeSent;

		std::cout << length - ipv4_hdr.header_length()
			<< " bytes from " << ipv4_hdr.source_address()
			<< ": icmp_seq=" << icmp_hdr.sequence_number()
			<< ", ttl=" << ipv4_hdr.time_to_live()
			<< ", time="
			<< chrono::duration_cast<chrono::milliseconds>(elapsed).count()
			<< std::endl;

		time = chrono::duration_cast<chrono::milliseconds>(elapsed).count();

		ioContext.stop();
		success = true;
	}
}

bool CUnityPing::Success(void) {
	return success;
}

long long CUnityPing::Time(void) {
	return time;
}
