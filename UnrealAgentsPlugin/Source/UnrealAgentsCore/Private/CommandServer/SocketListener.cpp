#include "SocketListener.h"
#include "MessageListener.h"

THIRD_PARTY_INCLUDES_START
#include "oatpp/core/macro/component.hpp"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"
#include "oatpp-websocket/Connector.hpp"
THIRD_PARTY_INCLUDES_END

void SocketListener::onPing(const WebSocket& socket, const oatpp::String& message)
{
	socket.sendPong(message);
}

void SocketListener::onPong(const WebSocket& socket, const oatpp::String& message)
{
}

void SocketListener::onClose(const WebSocket& socket, v_uint16 code, const oatpp::String& message)
{
}

void SocketListener::readMessage(const WebSocket& socket, v_uint8 opcode, p_char8 data, oatpp::v_io_size size)
{
	if (size == 0)
	{
		oatpp::String Message = MessageBuffer.toString();
		MessageBuffer.clear();

		OnMessageReceive(socket, Message);
	}
	else if (size > 0)
	{
		MessageBuffer.writeSimple(data, size);
	}
}

std::atomic<v_int32> ConnectionInstanceListener::SOCKETS(0);

void ConnectionInstanceListener::onAfterCreate(const oatpp::websocket::WebSocket& socket, const std::shared_ptr<const ParameterMap>& params)
{
	SOCKETS++;
	
	OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);

	Client = std::shared_ptr<const WebSocket>(&socket);

	socket.setListener(std::make_shared<FMessageListener>(objectMapper, CommandQueue));
}

void ConnectionInstanceListener::onBeforeDestroy(const oatpp::websocket::WebSocket& socket)
{
	SOCKETS--;
}