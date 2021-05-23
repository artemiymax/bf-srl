#pragma once

#include "CoreMinimal.h"
#include "IPolicy.h"
#include "Components/ActorComponent.h"
#include "Agent/SpaceType.h"
#include "BehaviorComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALAGENTSCORE_API UBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBehaviorComponent();

	UPROPERTY(Category = "Agent|Behavior", EditInstanceOnly)
	TEnumAsByte<ESpaceType::Type> ActionType;

	UPROPERTY(Category = "Agent|Behavior", EditInstanceOnly)
	TArray<uint32> ActionShape;

	UPROPERTY(Category = "Agent|Behavior", EditInstanceOnly, Instanced)
	UPolicyBase* PolicyType;
};

