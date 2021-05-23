#pragma once

#include "CoreMinimal.h"
#include "ISensor.h"
#include "Kismet/KismetSystemLibrary.h"

class UNREALAGENTSCORE_API FTraceSensor : public ISensor
{
public:
	FTraceSensor(uint32 Buffer, float Length, float Radius, ECollisionChannel CollisionChannel, EDrawDebugTrace::Type DrawType, float CastDegree, float CastRadius, TArray<FName> Tags, AActor* AnchorActor);

	virtual void Update() override;
	
	virtual void Read(TArray<float>& Reader) override;

	virtual void Reset() override;

private:
	uint32 StepBuffer;
	
	float TraceLength;
	
	float TraceRadius;

	ETraceTypeQuery TraceChannel;

	EDrawDebugTrace::Type TraceDrawType;

	float PerceptionDegree;

	float PerceptionRadius;

	TArray<FName> PerceptionTags;

	AActor* Actor;
	
	TArray<float> Observations;
};

