#pragma once

THIRD_PARTY_INCLUDES_START
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"
THIRD_PARTY_INCLUDES_END

#include OATPP_CODEGEN_BEGIN(DTO)

class FEnvironmentCommandDto : public oatpp::DTO
{
	DTO_INIT(FEnvironmentCommandDto, DTO)

	DTO_FIELD(String, type);
	DTO_FIELD(String, data);
};

#include OATPP_CODEGEN_END(DTO)

class FEnvironmentCommand
{
public:
	FString Type;

	FString Data;
};