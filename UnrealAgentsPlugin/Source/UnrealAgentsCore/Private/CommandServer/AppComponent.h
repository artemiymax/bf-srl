#pragma once

#include "SocketListener.h"

THIRD_PARTY_INCLUDES_START
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp-websocket/ConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"
THIRD_PARTY_INCLUDES_END

class AppComponent
{
public:
	explicit AppComponent(const uint16 Port)
		: ServerConnectionProvider(oatpp::network::tcp::server::ConnectionProvider::createShared(oatpp::network::Address("localhost", Port, oatpp::network::Address::IP_4)))
	{
	}

	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, HTTPRouter)
		([]
			{
				return oatpp::web::server::HttpRouter::createShared();
			}());

	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, ServerConnectionHandler)
		([]
			{
				OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, Router);
				return oatpp::web::server::HttpConnectionHandler::createShared(Router);
			}());

	oatpp::base::Environment::Component<std::shared_ptr<oatpp::network::ServerConnectionProvider>> ServerConnectionProvider;

	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, ApiObjectMapper)
		([]
			{
				return oatpp::parser::json::mapping::ObjectMapper::createShared();
			}());

	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::websocket::ConnectionHandler::SocketInstanceListener>, SocketInstanceListener)
		([]
			{
				return std::make_shared<ConnectionInstanceListener>();
			}());

	OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, WebsocketConnectionHandler)("websocket", []
		{
			OATPP_COMPONENT(std::shared_ptr<oatpp::websocket::ConnectionHandler::SocketInstanceListener>, Listener);
			auto connectionHandler = oatpp::websocket::ConnectionHandler::createShared();
			connectionHandler->setSocketInstanceListener(Listener);
			return connectionHandler;
		}());
};