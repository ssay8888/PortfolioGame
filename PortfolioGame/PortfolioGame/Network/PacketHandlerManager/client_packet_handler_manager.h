#pragma once
class ClientPacketHandler;
class ClientPacketHandlerManager

{
	ClientPacketHandlerManager();
	~ClientPacketHandlerManager();
public:
	static ClientPacketHandlerManager* GetInstance();

public:
	void LoadHandlers() const;
	static void RegisterHandler(opcode::ServerOpcode, std::shared_ptr<ClientPacketHandler>);
	static std::shared_ptr<ClientPacketHandler> GetHandler(const opcode::ServerOpcode&);

private:
	static std::map<opcode::ServerOpcode, std::shared_ptr<ClientPacketHandler>> _handlers;
};
