#include "../pch.h"
#include "client_session.h"
#include "../../ServerLibrary/Network/in_packet.h"
#include "../../ServerLibrary/Network/out_packet.h"

ClientSession::ClientSession(boost::asio::io_service& io_service): _io_service(io_service),
                                _socket(io_service) {}

ClientSession::~ClientSession()
{
	_socket.close();
}

void ClientSession::Connect(boost::asio::ip::tcp::endpoint endpoint)
{

    _socket.async_connect(endpoint, std::bind(&ClientSession::HandleConnect, this, std::placeholders::_1));
}

void ClientSession::HandleConnect(const boost::system::error_code& error_code)
{
	if (!error_code)
	{
		const boost::asio::ip::tcp::no_delay option(true);
		_socket.set_option(option);
		std::cout << "서버연결 성공" << std::endl;
		PacketHeaderReceive();
	}
	else
	{
	}
}

void ClientSession::PacketHeaderReceive() noexcept {
  auto packet_header = new uint8_t[4];
  ZeroMemory(packet_header, 4);
  boost::asio::async_read(_socket,
                          boost::asio::buffer(packet_header, 4),
                          std::bind(&ClientSession::PacketBodyReceive, this,
                                    std::placeholders::_1,
                                    std::placeholders::_2, packet_header));
}

void ClientSession::PacketBodyReceive(const std::error_code& error_code, const size_t bytes_transferred, uint8_t* buffer) noexcept
{
  if (!error_code)
  {
	  std::cout << "Data Receive" << std::endl;

	  int packet_size = *reinterpret_cast<int*>(buffer);
	  delete buffer;
	  auto packet_body = new uint8_t[packet_size];
	  ZeroMemory(packet_body, packet_size);

	  boost::asio::async_read(_socket,
		  boost::asio::buffer(packet_body, packet_size),
		  std::bind(&ClientSession::ProcessPacket, this,
			  std::placeholders::_1,
			  std::placeholders::_2, packet_body, packet_size));
	  return;
  }

  std::cout << "서버와 연결이 끊어짐." << std::endl;
}

void ClientSession::ProcessPacket(const std::error_code& error_code, const size_t bytes_transferred,
  uint8_t* buffer, int32_t packet_len) noexcept
{
	InPacket in_packet(buffer, packet_len);
	std::cout << in_packet.Decode4() << std::endl;
}

void ClientSession::OnSendPacketFinished(const std::error_code& ec, std::size_t bytes_transferred,
  OutPacket* out_packet)
{
	std::cout << "보내기" << std::endl;
	delete out_packet;
}

void ClientSession::SendPacket(OutPacket* out_packet)
{
	std::lock_guard<std::mutex> lock(_mtx_lock);

	if (!_socket.is_open())
	{
		return;
	}
	auto buffer = out_packet->GetPacket();
	auto packet_header = buffer - OutPacket::kPacketHeaderSize;

	int32_t packet_size = out_packet->GetPacketSize();

	auto asd = buffer - packet_size;
	*reinterpret_cast<decltype(packet_size)*>(buffer - OutPacket::kPacketHeaderSize) = packet_size - OutPacket::kPacketHeaderSize;

	boost::asio::async_write(_socket,
		boost::asio::buffer(buffer - OutPacket::kPacketHeaderSize, out_packet->GetPacketSize()),
		std::bind(&ClientSession::OnSendPacketFinished,
			this,
			std::placeholders::_1,
			std::placeholders::_2,
			out_packet));
}
