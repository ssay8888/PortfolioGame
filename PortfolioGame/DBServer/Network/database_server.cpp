#include "../pch.h"
#include "database_server.h"
#include "database_client_session.h"

#include "PacketHandlerManager/database_packet_handler_manager.h"
#include "PacketHandlerManager/PacketHandler.h"

std::map<int64_t, std::shared_ptr<DatabaseClientSession> > DatabaseServer::_session_list;
DatabaseServer::DatabaseServer() : _acceptor(nullptr)
{
}

DatabaseServer::~DatabaseServer()
{
}

void DatabaseServer::CreateAcceptor()
{
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), (unsigned short)1001);
	_acceptor = new boost::asio::ip::tcp::acceptor(_io_service, endpoint);
}

void DatabaseServer::HandleAcceptor(std::shared_ptr<DatabaseClientSession>  connected_socket,
	const std::error_code& error_code) {
	std::cout << "새로운 유저 입장 : " << connected_socket->GetSocket().remote_endpoint().address() << std::endl;
	connected_socket->Init();
	connected_socket->SetSocketDisconnectedCallBack(std::bind(&DatabaseServer::OnSocketDisconnected, this, std::placeholders::_1));
	this->_session_list.insert(std::make_pair(connected_socket->GetSocketId(), connected_socket));
	this->AsyncStartAcceptor();
}

void DatabaseServer::AsyncStartAcceptor() {
	std::shared_ptr<DatabaseClientSession> session(std::make_shared<DatabaseClientSession>(GetIoService()));
	session->SetThis(session);
	_acceptor->async_accept(session->GetSocket(),
		std::bind(&DatabaseServer::HandleAcceptor, this,
			session,
			std::placeholders::_1));
}

void DatabaseServer::OnSocketDisconnected(std::shared_ptr<DatabaseClientSession>  socket)
{
	const auto find_iterator = _session_list.find(socket->GetSocketId());

	if (find_iterator == _session_list.end()) {
		return;
	}

	OnNotifySocketDisconnected(socket);
	_session_list.erase(socket->GetSocketId());
}

void DatabaseServer::OnNotifySocketDisconnected(std::shared_ptr<DatabaseClientSession> socket)
{
}

void DatabaseServer::RunWorkThead()
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
						auto opcode = static_cast<::opcode::ServerOpcode>(packet.second->Decode1());
						auto handler = DatabasePacketHandlerManager::GetInstance()->GetHandler(opcode);

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

void DatabaseServer::InsertWorkPacket(std::shared_ptr<DatabaseClientSession>  session, std::shared_ptr<InPacket> in_packet)
{
	std::lock_guard<std::mutex> lock(_mtx_lock);
	_work_packet_list.push(std::make_pair(session, in_packet));
}

void DatabaseServer::BroadCastMessage(std::shared_ptr<OutPacket> out_packet, std::shared_ptr<DatabaseClientSession> session)
{
	for (const auto& client : _session_list)
	{
		if (client.second->GetSocketId() != session->GetSocketId())
		{
			client.second->SendPacket(out_packet);
		}
	}
}

std::map<int64_t, std::shared_ptr<DatabaseClientSession> >& DatabaseServer::GetSessionList()
{
	return _session_list;
}

boost::asio::io_service& DatabaseServer::GetIoService()
{
	return _io_service;
}