#include "../../pch.h"
#include "world_packet_handler_manager.h"
#include "../../PacketHandlers/login_handler.h"
#include "../../PacketHandlers/move_player_handler.h"
std::map<::opcode::ServerRecv, std::shared_ptr<PacketHandler>> WorldPacketHandlerManager::_handlers;

WorldPacketHandlerManager::WorldPacketHandlerManager() {}

WorldPacketHandlerManager::~WorldPacketHandlerManager() {}

std::shared_ptr<PacketHandler> WorldPacketHandlerManager::GetHandler(const opcode::ServerRecv& opcode)
{
	const auto handler = _handlers.find(opcode);

	if (handler == _handlers.end()) return nullptr;

	return handler->second;
}

WorldPacketHandlerManager* WorldPacketHandlerManager::GetInstance()
{
	static WorldPacketHandlerManager instance;
	return &instance;
}

void WorldPacketHandlerManager::LoadHandlers()
{
	RegisterHandler(opcode::ServerRecv::kLoginRequest, std::make_shared<LoginHandler>());
	RegisterHandler(opcode::ServerRecv::kMovePlayer, std::make_shared<MovePlayerHandler>());

	std::cout << "Packet Handler Load Count: " << _handlers.size() << std::endl;
	
}

void WorldPacketHandlerManager::RegisterHandler(opcode::ServerRecv opcode, std::shared_ptr<PacketHandler> handler)
{
	_handlers.insert(std::make_pair(opcode, handler));
}
