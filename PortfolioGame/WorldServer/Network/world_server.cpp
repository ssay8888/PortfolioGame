#include "../pch.h"
#include "world_server.h"
#include "world_client_session.h"
#include "../../ServerLibrary/Network/out_packet.h"

#include "../Field/Character/character.h"

#include "PacketHandlerManager/world_packet_handler_manager.h"
#include "PacketHandlerManager/packet_handler.h"

std::map<int64_t, std::shared_ptr<WorldClientSession> > WorldServer::_session_list;
WorldServer::WorldServer() : _acceptor(nullptr)
{
}

WorldServer::~WorldServer()
{
}

void WorldServer::CreateAcceptor()
{
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), (unsigned short)1000);
	_acceptor = new boost::asio::ip::tcp::acceptor(_io_service, endpoint);
}

void WorldServer::HandleAcceptor(std::shared_ptr<WorldClientSession>  connected_socket,
	const std::error_code& error_code) {
	std::cout << "새로운 유저 입장 : " << connected_socket->GetSocket().remote_endpoint().address() << std::endl;
	connected_socket->Init();
	connected_socket->SetSocketDisconnectedCallBack(std::bind(&WorldServer::OnSocketDisconnected, this, std::placeholders::_1));
	this->_session_list.insert(std::make_pair(connected_socket->GetSocketId(), connected_socket));
	this->AsyncStartAcceptor();
}

void WorldServer::AsyncStartAcceptor() {
	std::shared_ptr<WorldClientSession> session(std::make_shared<WorldClientSession>(GetIoService()));
	session->SetThis(session);
	_acceptor->async_accept(session->GetSocket(),
		std::bind(&WorldServer::HandleAcceptor, this,
			session,
			std::placeholders::_1));
}

void WorldServer::OnSocketDisconnected(std::shared_ptr<WorldClientSession>  socket)
{
	const auto find_iterator = _session_list.find(socket->GetSocketId());

	if (find_iterator == _session_list.end()) {
		return;
	}

	OnNotifySocketDisconnected(socket);
	_session_list.erase(socket->GetSocketId());
}

void WorldServer::OnNotifySocketDisconnected(std::shared_ptr<WorldClientSession> socket)
{
	WorldServer::GetInstance()->BroadCastMessage(DisconnectCharacter(socket), socket);
}

void WorldServer::RunWorkThead()
{
	std::thread work_thread([this]()
	{
			while (true)
			{
				while (!_work_packet_list.empty())
				{
					std::lock_guard<std::mutex> lock(_mtx_lock);
					auto packet = _work_packet_list.front();
					_work_packet_list.pop();
				  if (packet.second)
				  {
					  auto opcode = static_cast<::opcode::ClientOpcode>(packet.second->Decode1());
					  auto handler = WorldPacketHandlerManager::GetInstance()->GetHandler(opcode);

					  if (handler != nullptr)
					  {
						  handler->HandlePacket(packet.first, packet.second);
					  }
				  }
				}
				Sleep(1);
			}
	}
	);
	work_thread.detach();
}

void WorldServer::InsertWorkPacket(std::shared_ptr<WorldClientSession>  session, std::shared_ptr<InPacket> in_packet)
{
	std::lock_guard<std::mutex> lock(_mtx_lock);
	_work_packet_list.push(std::make_pair(session, in_packet));
}

void WorldServer::BroadCastMessage(std::shared_ptr<OutPacket> out_packet, std::shared_ptr<WorldClientSession> session)
{
	for (const auto& client : _session_list)
	{
		if (client.second->GetSocketId() != session->GetSocketId())
		{
			client.second->SendPacket(out_packet);
		}
	}
}

std::map<int64_t, std::shared_ptr<WorldClientSession> >& WorldServer::GetSessionList()
{
	return _session_list;
}

std::shared_ptr<OutPacket> WorldServer::DisconnectCharacter(
	std::shared_ptr<WorldClientSession> socket) const
{
	std::shared_ptr<OutPacket> out_packet = std::make_shared<OutPacket>();
	out_packet->Encode1(static_cast<int>(::opcode::ServerOpcode::kRemoveCharacter));
	out_packet->Encode4(socket->GetCharacter()->GetId());
	return out_packet;
}

boost::asio::io_service& WorldServer::GetIoService()
{
	return _io_service;
}
