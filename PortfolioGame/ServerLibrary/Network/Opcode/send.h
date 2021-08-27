#pragma once
namespace opcode
{
	enum class ClientSend { kLoginRequest, kMovePlayer };
	enum class ServerSend { kLoginResponse, kCharacterInfo, kSpawnPlayer, kMovePlayer, kRemoveCharacter };
}