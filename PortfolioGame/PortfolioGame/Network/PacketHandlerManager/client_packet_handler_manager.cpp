#include "../../pch.h"
#include "client_packet_handler_manager.h"
#include "../PacketHandlers/login_packet_handler.h"
#include "../PacketHandlers/character_info_packet_handler.h"
#include "../PacketHandlers/spawn_player_packet_handelr.h"
#include "../PacketHandlers/move_player_packet_handler.h"
std::map<::opcode::ClientRecv, std::shared_ptr<ClientPacketHandler>> ClientPacketHandlerManager::_handlers;

ClientPacketHandlerManager::ClientPacketHandlerManager() = default;

ClientPacketHandlerManager::~ClientPacketHandlerManager() = default;

std::shared_ptr<ClientPacketHandler> ClientPacketHandlerManager::GetHandler(const opcode::ClientRecv& opcode)
{
	const auto handler = _handlers.find(opcode);

	if (handler == _handlers.end()) return nullptr;

	return handler->second;
}

ClientPacketHandlerManager* ClientPacketHandlerManager::GetInstance()
{
	static ClientPacketHandlerManager instance;
	return &instance;
}

void ClientPacketHandlerManager::LoadHandlers() const
{
	RegisterHandler(opcode::ClientRecv::kLoginResponse, std::make_shared<LoginPacketHandler>());
	RegisterHandler(opcode::ClientRecv::kCharacterInfo, std::make_shared<CharacterInfoPacketHandler>());
	RegisterHandler(opcode::ClientRecv::kSpawnPlayer, std::make_shared<SpawnPlayerPacketHandler>());
	RegisterHandler(opcode::ClientRecv::kMovePlayer, std::make_shared<MovePlayerPacketHandler>());

	std::cout << "Packet Handler Load Count: " << _handlers.size() << std::endl;

}

void ClientPacketHandlerManager::RegisterHandler(opcode::ClientRecv opcode, std::shared_ptr<ClientPacketHandler> handler)
{
	_handlers.insert(std::make_pair(opcode, handler));
}