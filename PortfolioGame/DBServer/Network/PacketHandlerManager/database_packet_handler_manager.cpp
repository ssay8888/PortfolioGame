#include "../../pch.h"
#include "database_packet_handler_manager.h"

std::map<::opcode::ServerOpcode, std::shared_ptr<PacketHandler>> DatabasePacketHandlerManager::_handlers;

DatabasePacketHandlerManager::DatabasePacketHandlerManager() {}

DatabasePacketHandlerManager::~DatabasePacketHandlerManager() {}

std::shared_ptr<PacketHandler> DatabasePacketHandlerManager::GetHandler(const opcode::ServerOpcode& opcode)
{
	const auto handler = _handlers.find(opcode);

	if (handler == _handlers.end()) return nullptr;

	return handler->second;
}

DatabasePacketHandlerManager* DatabasePacketHandlerManager::GetInstance()
{
	static DatabasePacketHandlerManager instance;
	return &instance;
}

void DatabasePacketHandlerManager::LoadHandlers()
{
	std::cout << "Packet Handler Load Count: " << _handlers.size() << std::endl;

}

void DatabasePacketHandlerManager::RegisterHandler(opcode::ServerOpcode opcode, std::shared_ptr<PacketHandler> handler)
{
	_handlers.insert(std::make_pair(opcode, handler));
}
