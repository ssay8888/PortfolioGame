#include "../pch.h"
#include "database_client_session.h"

#include "database_server.h"

DatabaseClientSession::DatabaseClientSession(boost::asio::io_service& server_service) : _socket_id(),
                                                                                        _socket(server_service),
                                                                                        _status() {}

DatabaseClientSession::~DatabaseClientSession()
{
}


boost::asio::ip::tcp::socket& DatabaseClientSession::GetSocket()
{
	return _socket;
}

void DatabaseClientSession::SessionHandleRecv(const std::error_code& error_code,
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
		std::bind(&DatabaseClientSession::ProcessPacket, this,
			std::placeholders::_1,
			std::placeholders::_2, packet, packet_size));
}

void DatabaseClientSession::ProcessPacket(const std::error_code& error_code, std::size_t bytes_transferred,
	uint8_t* buffer, int nPacketLen)
{
	if (error_code)
	{
		OnDisconnect();
		return;
	}
	std::shared_ptr<InPacket> in_packet = std::make_shared<InPacket>(buffer, nPacketLen);
	DatabaseServer::GetInstance()->InsertWorkPacket(GetThis(), in_packet);
	WaitRecvPacket();
}

void DatabaseClientSession::OnSendPacketFinished(const std::error_code& ec,
	std::size_t bytes_transferred, std::shared_ptr<OutPacket> out_packet)
{
}

void DatabaseClientSession::WaitRecvPacket() noexcept
{
	auto packet_header = new uint8_t[4];
	ZeroMemory(packet_header, 4);
	boost::asio::async_read(_socket,
		boost::asio::buffer(packet_header, 4),
		std::bind(&DatabaseClientSession::SessionHandleRecv, this,
			std::placeholders::_1,
			std::placeholders::_2, packet_header));
}

void DatabaseClientSession::OnDisconnect()
{
	if (_status != ServerStatus::kClose)
	{
		_status = ServerStatus::kClose;
		_socket.close();
		_socket_disconnected_callback(GetThis());
		_this = nullptr;
	}
}

void DatabaseClientSession::Init()
{
	const boost::asio::ip::tcp::no_delay option(true);
	_socket.set_option(option);
	_socket_id = rand();
	_status = ServerStatus::kConnect;
	this->WaitRecvPacket();
}

void DatabaseClientSession::SendPacket(std::shared_ptr<OutPacket> out_packet, bool is_broad_cast)
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

	if (is_broad_cast)
	{
		DatabaseServer::GetInstance()->BroadCastMessage(out_packet, GetThis());
	}
	else
	{
		boost::asio::async_write(_socket,
			boost::asio::buffer(buffer - OutPacket::kPacketHeaderSize, out_packet->GetPacketSize()),
			std::bind(&DatabaseClientSession::OnSendPacketFinished,
				this,
				std::placeholders::_1,
				std::placeholders::_2,
				out_packet));
	}
}
uint64_t DatabaseClientSession::GetSocketId() const
{
	return _socket_id;
}

void DatabaseClientSession::SetSocketDisconnectedCallBack(
	const std::function<void(std::shared_ptr<DatabaseClientSession>)>& callback)
{
	_socket_disconnected_callback = callback;
}

void DatabaseClientSession::SetCharacter(const std::shared_ptr<Character> character)
{
	_character = character;
}
std::shared_ptr<Character> DatabaseClientSession::GetCharacter() const
{
	return _character;
}

std::shared_ptr<DatabaseClientSession> DatabaseClientSession::GetThis()
{
	return _this;
}
void DatabaseClientSession::SetThis(std::shared_ptr<DatabaseClientSession> ptr)
{
	_this = ptr;
}
