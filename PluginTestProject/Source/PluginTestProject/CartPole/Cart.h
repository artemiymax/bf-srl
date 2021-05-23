#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Cart.generated.h"

UCLASS()
class PLUGINTESTPROJECT_API ACart : public AStaticMeshActor
{
	GENERATED_BODY()

public:	
	ACart();

	void BeginPlay() override;
	
	void EndPlay(EEndPlayReason::Type EndPlayReason) override;
};
