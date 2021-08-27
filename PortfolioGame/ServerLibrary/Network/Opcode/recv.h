#pragma once
namespace opcode
{
	enum class ClientRecv { kLoginResponse, kCharacterInfo, kSpawnPlayer, kMovePlayer, kRemoveCharacter };
	enum class ServerRecv { kLoginRequest, kMovePlayer };
}