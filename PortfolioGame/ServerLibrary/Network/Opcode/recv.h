#pragma once
namespace opcode
{
	enum class ClientRecv { kLoginResponse, kCharacterInfo, kSpawnPlayer, kMovePlayer };
	enum class ServerRecv { kLoginRequest, kMovePlayer };
}