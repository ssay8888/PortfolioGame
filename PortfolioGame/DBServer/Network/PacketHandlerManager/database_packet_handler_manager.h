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
	void RegisterHandler(::opcode::ServerOpcode, std::shared_ptr<PacketHandler>);
	std::shared_ptr<PacketHandler> GetHandler(const ::opcode::ServerOpcode&);

private:
	static std::map<::opcode::ServerOpcode, std::shared_ptr<PacketHandler>> _handlers;
};

