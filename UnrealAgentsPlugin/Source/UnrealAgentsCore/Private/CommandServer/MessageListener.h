#pragma once

#include "CommandListener.h"
#include "AgentAction.h"
#include "DtoConverter.h"
#include "EnvironmentCommand.h"
#include "SocketListener.h"

THIRD_PARTY_INCLUDES_START
#include "oatpp/core/data/mapping/ObjectMapper.hpp"
THIRD_PARTY_INCLUDES_END

class FMessageListener : public SocketListener
{
public:
	explicit FMessageListener(const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& Mapper, TQueue<std::shared_ptr<FEnvironmentCommand>, EQueueMode::Mpsc>* Queue) : MessageMapper(Mapper)
	{
		CommandQueue = Queue;
	}

	virtual void OnMessageReceive(const WebSocket& Socket, const oatpp::String& Message) override
	{
		CommandQueue->Enqueue(FDtoConverter::ToEnvironmentCommand(Message, MessageMapper));
	}

private:
	std::shared_ptr<oatpp::data::mapping::ObjectMapper> MessageMapper;

	TQueue<std::shared_ptr<FEnvironmentCommand>, EQueueMode::Mpsc>* CommandQueue;
};
