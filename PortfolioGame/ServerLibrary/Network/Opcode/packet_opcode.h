#pragma once
namespace opcode
{
	enum class ClientOpcode { kLoginRequest, kMovePlayer };
	enum class ServerOpcode { kLoginResponse, kCharacterInfo, kSpawnPlayer, kMovePlayer, kRemoveCharacter };
  enum class DatabaseOpcode {};
}