#pragma once

THIRD_PARTY_INCLUDES_START
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp-websocket/Handshaker.hpp"
#include "oatpp/network/ConnectionHandler.hpp"
THIRD_PARTY_INCLUDES_END

#include OATPP_CODEGEN_BEGIN(ApiController)

class FSocketController : public oatpp::web::server::api::ApiController
{
private:
	OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, websocketConnectionHandler, "websocket");
public:
	FSocketController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
		: oatpp::web::server::api::ApiController(objectMapper)
	{
	}
public:

	ENDPOINT("GET", "ws", ws, REQUEST(std::shared_ptr<IncomingRequest>, request))
	{
		return oatpp::websocket::Handshaker::serversideHandshake(request->getHeaders(), websocketConnectionHandler);
	};
};

#include OATPP_CODEGEN_END(ApiController)