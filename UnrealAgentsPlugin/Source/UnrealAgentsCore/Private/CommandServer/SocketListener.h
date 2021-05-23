#pragma once

#include "EnvironmentCommand.h"

THIRD_PARTY_INCLUDES_START
#include "oatpp-websocket/ConnectionHandler.hpp"
#include "oatpp-websocket/WebSocket.hpp"
#include "oatpp/core/data/mapping/ObjectMapper.hpp"
THIRD_PARTY_INCLUDES_END

class SocketListener : public oatpp::websocket::WebSocket::Listener
{
private:
	static constexpr const char* TAG = "Server_SocketListener";
private:
	oatpp::data::stream::ChunkedBuffer MessageBuffer;
public:
	void onPing(const WebSocket& socket, const oatpp::String& message) override;

	void onPong(const WebSocket& socket, const oatpp::String& message) override;

	void onClose(const WebSocket& socket, v_uint16 code, const oatpp::String& message) override;

	void readMessage(const WebSocket& socket, v_uint8 opcode, p_char8 data, oatpp::v_io_size size) override;

	virtual void OnMessageReceive(const WebSocket& socket, const oatpp::String& Message) = 0;
};

class ConnectionInstanceListener : public oatpp::websocket::ConnectionHandler::SocketInstanceListener
{
private:
	static constexpr const char* TAG = "Server_SocketInstanceListener";
public:	
	virtual ~ConnectionInstanceListener() = default;
	
	static std::atomic<v_int32> SOCKETS;

	std::shared_ptr<const WebSocket> Client = nullptr;
	
	TQueue<std::shared_ptr<FEnvironmentCommand>, EQueueMode::Mpsc>* CommandQueue = nullptr;

	void onAfterCreate(const oatpp::websocket::WebSocket& socket, const std::shared_ptr<const ParameterMap>& params) override;

	void onBeforeDestroy(const oatpp::websocket::WebSocket& socket) override;	
};