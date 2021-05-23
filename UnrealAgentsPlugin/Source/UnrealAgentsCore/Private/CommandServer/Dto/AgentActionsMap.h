#pragma once

#include "AgentAction.h"

THIRD_PARTY_INCLUDES_START
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"
THIRD_PARTY_INCLUDES_END

#include OATPP_CODEGEN_BEGIN(DTO)

class FAgentActionsMapDto : public oatpp::DTO
{
	DTO_INIT(FAgentActionsMapDto, DTO)

	DTO_FIELD(Fields<FAgentActionDto>, actions);
};

#include OATPP_CODEGEN_END(DTO)