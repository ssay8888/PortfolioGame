#pragma once
class PacketHandler;

class DatabasePacketHandlerManager
{
	DatabasePacketHandlerManager();
	~DatabasePacketHandlerManager();
public:
	static DatabasePacketHandlerManager* GetInstance();

public:
	void LoadHandlers();
	void RegisterHandler(::opcode::ServerRecv, std::shared_ptr<PacketHandler>);
	std::shared_ptr<PacketHandler> GetHandler(const ::opcode::ServerRecv&);

private:
	static std::map<::opcode::ServerRecv, std::shared_ptr<PacketHandler>> _handlers;
};

