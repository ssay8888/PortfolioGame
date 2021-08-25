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
	static void RegisterHandler(::opcode::ClientRecv, std::shared_ptr<ClientPacketHandler>);
	static std::shared_ptr<ClientPacketHandler> GetHandler(const ::opcode::ClientRecv&);

private:
	static std::map<::opcode::ClientRecv, std::shared_ptr<ClientPacketHandler>> _handlers;
};
