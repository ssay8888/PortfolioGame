#include "../pch.h"
#include "world_server.h"
#include "world_client_session.h"
#include "../../ServerLibrary/Network/out_packet.h"

#include "PacketHandlerManager/world_packet_handler_manager.h"
#include "PacketHandlerManager/packet_handler.h"

std::map<int64_t, WorldClientSession*> WorldServer::_session_list;
WorldServer::WorldServer() : _acceptor(nullptr)
{
}

WorldServer::~WorldServer()
{
	std::cout << "갑자기 왜소멸?" << std::endl;
}

void WorldServer::CreateAcceptor()
{
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), (unsigned short)1000);
	_acceptor = new boost::asio::ip::tcp::acceptor(_io_service, endpoint);
}

void WorldServer::HandleAcceptor(WorldClientSession* connected_socket,
	const std::error_code& error_code) {
	std::cout << "새로운 유저 입장 : " << connected_socket->GetSocket().remote_endpoint().address() << std::endl;
	connected_socket->Init();
	connected_socket->SetSocketDisconnectedCallBack(std::bind(&WorldServer::OnSocketDisconnected, this, std::placeholders::_1));
	this->_session_list.insert(std::make_pair(connected_socket->GetSocketId(), connected_socket));
	OutPacket* out = new OutPacket();
	out->Encode4(456123);
	connected_socket->SendPacket(out);

	this->AsyncStartAcceptor();
}

void WorldServer::AsyncStartAcceptor() {
	WorldClientSession* session(new WorldClientSession(GetIoService()));
	_acceptor->async_accept(session->GetSocket(),
		std::bind(&WorldServer::HandleAcceptor, this,
			session,
			std::placeholders::_1));
}

void WorldServer::OnSocketDisconnected(WorldClientSession* socket)
{
	const auto find_iterator = _session_list.find(socket->GetSocketId());

	if (find_iterator == _session_list.end()) {
		return;
	}

	OnNotifySocketDisconnected(socket);
	_session_list.erase(socket->GetSocketId());
	delete socket;
}

void WorldServer::OnNotifySocketDisconnected(WorldClientSession* socket)
{

}

void WorldServer::RunWorkThead()
{
	std::thread work_thread([this]()
	{
			while (true)
			{
				while (!_work_packet_list.empty())
				{
					auto packet = _work_packet_list.front();
					auto opcode = static_cast<::opcode::ServerRecv>(packet.second->Decode1());
					auto handler = WorldPacketHandlerManager::GetInstance()->GetHandler(opcode);

					if (handler != nullptr)
					{
						handler->HandlePacket(packet.first, packet.second);
					}
					delete packet.second;
					packet.second = nullptr;
					_work_packet_list.pop();
				}
				Sleep(1);
			}
	}
	);
	work_thread.detach();
}

void WorldServer::InsertWorkPacket(WorldClientSession* session, InPacket* in_packet)
{
	_work_packet_list.push(std::make_pair(session, in_packet));
}

boost::asio::io_service& WorldServer::GetIoService()
{
	return _io_service;
}
