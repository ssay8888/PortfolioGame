#include "../pch.h"
#include "world_client_session.h"

#include "world_server.h"

WorldClientSession::WorldClientSession(boost::asio::io_service& server_service): _socket_id(0),
                                                                                 _socket(server_service) {
}

WorldClientSession::~WorldClientSession()
{
}


boost::asio::ip::tcp::socket& WorldClientSession::GetSocket()
{
	return _socket;
}

void WorldClientSession::SessionHandleRecv(const std::error_code& error_code,
	const size_t bytes_transferred, uint8_t* buffer)
{
  if (error_code)
  {
	  OnDisconnect();
	  return;
  }

  int packet_size = *reinterpret_cast<int*>(buffer);
	delete buffer;

	auto packet = new uint8_t[packet_size];
	boost::asio::async_read(_socket,
		boost::asio::buffer(packet, packet_size),
		std::bind(&WorldClientSession::ProcessPacket, this,
			std::placeholders::_1,
			std::placeholders::_2, packet, packet_size));
}

void WorldClientSession::ProcessPacket(const std::error_code& error_code, std::size_t bytes_transferred,
	uint8_t* buffer, int nPacketLen)
{
	if (error_code)
	{
		OnDisconnect();
		return;
	}
	InPacket* in_packet = new InPacket(buffer, nPacketLen);
	WorldServer::GetInstance()->InsertWorkPacket(this, in_packet);
	WaitRecvPacket();
}

void WorldClientSession::OnSendPacketFinished(const std::error_code& ec,
  std::size_t bytes_transferred, OutPacket* out_packet)
{
	delete out_packet;
	out_packet = nullptr;
}

void WorldClientSession::WaitRecvPacket() noexcept
{
	auto packet_header = new uint8_t[4];
	ZeroMemory(packet_header, 4);
	boost::asio::async_read(_socket,
		boost::asio::buffer(packet_header, 4),
		std::bind(&WorldClientSession::SessionHandleRecv, this,
			std::placeholders::_1,
			std::placeholders::_2, packet_header));
}

void WorldClientSession::OnDisconnect()
{
	_socket.close();
	_socket_disconnected_callback(this);
}

void WorldClientSession::Init()
{
	const boost::asio::ip::tcp::no_delay option(true);
	_socket.set_option(option);


	this->WaitRecvPacket();
}

void WorldClientSession::SendPacket(OutPacket* out_packet)
{
	std::lock_guard<std::mutex> lock(_mtx_lock);

	if (!_socket.is_open())
	{
			OnDisconnect();
			return;
	}
	auto buffer = out_packet->GetPacket();
	auto packet_header = buffer - OutPacket::kPacketHeaderSize;

	int32_t packet_size = out_packet->GetPacketSize();

	auto asd = buffer - packet_size;
	*reinterpret_cast<decltype(packet_size)*>(buffer - OutPacket::kPacketHeaderSize) = packet_size - OutPacket::kPacketHeaderSize;

  boost::asio::async_write(_socket,
                           boost::asio::buffer(buffer - OutPacket::kPacketHeaderSize, out_packet->GetPacketSize()),
                           std::bind(&WorldClientSession::OnSendPacketFinished,
                                     this,
                                     std::placeholders::_1,
                                     std::placeholders::_2,
							                       out_packet));
}

uint64_t WorldClientSession::GetSocketId() const
{
	return _socket_id;
}

void WorldClientSession::SetSocketDisconnectedCallBack(
  const std::function<void(WorldClientSession*)>& callback)
{
	_socket_disconnected_callback = callback;
}
