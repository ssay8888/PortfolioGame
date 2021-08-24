#pragma once
class PacketHandler;

class WorldPacketHandlerManager
{
	WorldPacketHandlerManager();
	~WorldPacketHandlerManager();
public:
	static WorldPacketHandlerManager* GetInstance();

public:
	void LoadHandlers();
	void RegisterHandler(::opcode::ServerRecv, std::shared_ptr<PacketHandler>);
	std::shared_ptr<PacketHandler> GetHandler(const ::opcode::ServerRecv&);

private:
	static std::map<::opcode::ServerRecv, std::shared_ptr<PacketHandler>> _handlers;
};
