#pragma once

THIRD_PARTY_INCLUDES_START
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"
THIRD_PARTY_INCLUDES_END

#include OATPP_CODEGEN_BEGIN(DTO)

class FAgentActionDto : public oatpp::DTO
{
	DTO_INIT(FAgentActionDto, DTO)

	DTO_FIELD(String, agentId);
	DTO_FIELD(List<UInt32>, discreteActions);
	DTO_FIELD(List<Float32>, continuousActions);
};

#include OATPP_CODEGEN_END(DTO)