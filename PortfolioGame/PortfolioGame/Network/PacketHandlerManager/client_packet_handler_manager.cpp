#include "../../pch.h"
#include "client_packet_handler_manager.h"
#include "../PacketHandlers/login_packet_handler.h"
#include "../PacketHandlers/character_info_packet_handler.h"
#include "../PacketHandlers/spawn_player_packet_handelr.h"
#include "../PacketHandlers/move_player_packet_handler.h"
#include "../PacketHandlers/remove_character_packet_handler.h"
std::map<::opcode::ServerOpcode, std::shared_ptr<ClientPacketHandler>> ClientPacketHandlerManager::_handlers;

ClientPacketHandlerManager::ClientPacketHandlerManager() = default;

ClientPacketHandlerManager::~ClientPacketHandlerManager() = default;

std::shared_ptr<ClientPacketHandler> ClientPacketHandlerManager::GetHandler(const opcode::ServerOpcode & opcode)
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
	RegisterHandler(opcode::ServerOpcode::kLoginResponse, std::make_shared<LoginPacketHandler>());
	RegisterHandler(opcode::ServerOpcode::kCharacterInfo, std::make_shared<CharacterInfoPacketHandler>());
	RegisterHandler(opcode::ServerOpcode::kSpawnPlayer, std::make_shared<SpawnPlayerPacketHandler>());
	RegisterHandler(opcode::ServerOpcode::kMovePlayer, std::make_shared<MovePlayerPacketHandler>());
	RegisterHandler(opcode::ServerOpcode::kRemoveCharacter, std::make_shared<RemoveCharacterPacketHandler>());

	std::cout << "Packet Handler Load Count: " << _handlers.size() << std::endl;

}

void ClientPacketHandlerManager::RegisterHandler(opcode::ServerOpcode opcode, std::shared_ptr<ClientPacketHandler> handler)
{
	_handlers.insert(std::make_pair(opcode, handler));
}