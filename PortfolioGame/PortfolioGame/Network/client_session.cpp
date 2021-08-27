#include "../pch.h"
#include "client_session.h"
#include "../../ServerLibrary/Network/in_packet.h"
#include "../../ServerLibrary/Network/out_packet.h"

#include "PacketHandlerManager/client_packet_handler_manager.h"
#include "PacketHandlerManager/client_packet_handler.h"

ClientSession::ClientSession(boost::asio::io_service& io_service): _io_service(io_service),
                                                                   _socket(io_service),
                                                                   _player(nullptr) {}

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
		std::cout << "�������� ����" << std::endl;
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

  std::cout << "������ ������ ������." << std::endl;
}

void ClientSession::ProcessPacket(const std::error_code& error_code, const size_t bytes_transferred,
  uint8_t* buffer, int32_t packet_len) noexcept
{
	std::lock_guard<std::mutex> lock(_mtx_lock);
	InPacket* in_packet =  new InPacket(buffer, packet_len);
	_work_packet_list.push(in_packet);
	PacketHeaderReceive();
}

void ClientSession::OnSendPacketFinished(const std::error_code& ec, std::size_t bytes_transferred,
  OutPacket* out_packet)
{
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

void ClientSession::RunWorkThead()
{
	std::lock_guard<std::mutex> lock(_mtx_lock);
	if (!_work_packet_list.empty())
	{
		auto packet = _work_packet_list.front();
		auto opcode = static_cast<::opcode::ClientRecv>(packet->Decode1());
		auto handler = ClientPacketHandlerManager::GetInstance()->GetHandler(opcode);

		if (handler != nullptr)
		{
			handler->HandlePacket(this, packet);
		}
		else
		{
			std::cout << "�� �� ���� ��Ŷ (" << std::to_string(static_cast<char>(opcode)) << ")" << std::endl;
		}
		delete packet;
		packet = nullptr;
		_work_packet_list.pop();
	}
}

void ClientSession::SetPlayer(Player* player)
{
	_player = player;
}
Player* ClientSession::GetPlayer() const
{
	return _player;
}