#pragma once

THIRD_PARTY_INCLUDES_START
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"
THIRD_PARTY_INCLUDES_END

#include OATPP_CODEGEN_BEGIN(DTO)

class FAgentStepDto : public oatpp::DTO
{
	DTO_INIT(FAgentStepDto, DTO)

	DTO_FIELD(String, agentId);
	DTO_FIELD(List<Float32>, observations);
	DTO_FIELD(Float32, reward);
	DTO_FIELD(Boolean, done);
	DTO_FIELD(Fields<String>, info);
};

#include OATPP_CODEGEN_END(DTO)