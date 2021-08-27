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
	void RegisterHandler(::opcode::ClientOpcode, std::shared_ptr<PacketHandler>);
	std::shared_ptr<PacketHandler> GetHandler(const ::opcode::ClientOpcode&);

private:
	static std::map<::opcode::ClientOpcode, std::shared_ptr<PacketHandler>> _handlers;
};
