#pragma once

#include "AgentAction.h"
#include "AgentStep.h"
#include "Agent/AgentAction.h"
#include "Agent/AgentState.h"
#include "Sensor/ISensor.h"

THIRD_PARTY_INCLUDES_START
#include "oatpp/core/data/mapping/ObjectMapper.hpp"
THIRD_PARTY_INCLUDES_END

class FDtoConverter
{
public:
	static TMap<FString, FAgentAction> ToAgentActionMap(const oatpp::String& Message, const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& Mapper)
	{	
		oatpp::Fields<oatpp::Object<FAgentActionDto>> ActionMapObject = Mapper->readFromString<oatpp::Fields<oatpp::Object<FAgentActionDto>>>(Message);

		TMap<FString, FAgentAction> AgentActionMap;
		for (int32 i = 0; i < ActionMapObject->size(); i++)
		{
			FAgentAction Action;

			for (int32 j= 0; j < ActionMapObject[i].second->discreteActions->size(); j++)
			{
				Action.DiscreteValues.Add(ActionMapObject[i].second->discreteActions[j]);
			}

			for (int32 j = 0; j < ActionMapObject[i].second->continuousActions->size(); j++)
			{
				Action.ContinuousValues.Add(ActionMapObject[i].second->continuousActions[j]);
			}

			AgentActionMap.Add(FString(ActionMapObject[i].first->c_str()), Action);
		}

		return AgentActionMap;
	}

	static std::shared_ptr<FEnvironmentCommand> ToEnvironmentCommand(const oatpp::String& Message, const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& Mapper)
	{
		oatpp::Object<FEnvironmentCommandDto> CommandObject = Mapper->readFromString<oatpp::Object<FEnvironmentCommandDto>>(Message);

		std::shared_ptr<FEnvironmentCommand> Command = std::make_shared<FEnvironmentCommand>();
		Command->Type = FString(CommandObject->type->c_str());
		Command->Data = FString(CommandObject->data->c_str());

		return Command;
	}

	static oatpp::data::mapping::type::DTOWrapper<FAgentStepDto> ToAgentStepDto(const TTuple<FAgentState, TArray<float>>* State)
	{
		oatpp::data::mapping::type::DTOWrapper<FAgentStepDto> AgentStep = FAgentStepDto::createShared();
		AgentStep->agentId = TCHAR_TO_UTF8(*State->Key.AgentId);
		AgentStep->done = State->Key.bIsDone;
		AgentStep->reward = State->Key.StepReward;
		AgentStep->info = oatpp::Fields<oatpp::String>();
		AgentStep->observations = oatpp::List<oatpp::Float32>::createShared();

		for (const float& Observation : State->Value)
		{
			AgentStep->observations->push_back(Observation);
		}

		return AgentStep;
	}
};
