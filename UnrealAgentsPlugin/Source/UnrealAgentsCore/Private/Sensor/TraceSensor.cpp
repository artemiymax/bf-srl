#include "Sensor/TraceSensor.h"

#include "EngineUtils.h"
#include "Engine/World.h"
#include "EnvironmentHelper.h"

FTraceSensor::FTraceSensor(uint32 Buffer, float Length, float Radius, ECollisionChannel Channel, EDrawDebugTrace::Type DrawType, float CastDegree, float CastRadius, TArray<FName> Tags, AActor* AnchorActor) :
	ISensor(FObservationSpace({ static_cast<uint32>((Tags.Num() + 1) * FMath::Floor(CastRadius / CastDegree)) * Buffer }, ESpaceType::Continuous)),
	StepBuffer(Buffer),
	TraceLength(Length),
	TraceRadius(Radius),
	PerceptionDegree(CastDegree),
	PerceptionRadius(CastRadius),
	PerceptionTags(Tags)
{
	TraceChannel = UEngineTypes::ConvertToTraceType(Channel);
	TraceDrawType = DrawType;
	Actor = AnchorActor;
	Observations.Init(0, ObservationSpace.Size);
}

void FTraceSensor::Update()
{
	UWorld* World = FEnvironmentHelper::GetWorld();

	TArray<AActor*> AcToIgnore{ Actor };
	TArray<FHitResult> HitResults;

	const float PerceptionSectorDegree = PerceptionRadius / 2;

	for (float i = -PerceptionSectorDegree; i < PerceptionSectorDegree; i += PerceptionDegree)
	{
		FHitResult HitRes;

		FVector UpVec = Actor->GetActorUpVector();
		FVector StartVector = Actor->GetActorLocation() - UpVec;
		FVector EndVector = Actor->GetActorForwardVector().RotateAngleAxis(i, UpVec.GetSafeNormal()) * TraceLength + StartVector;
		bool const bHit = UKismetSystemLibrary::SphereTraceSingle(World, StartVector, EndVector, TraceRadius, TraceChannel, false, AcToIgnore, TraceDrawType, HitRes, true);

		for (const FName& Tag : PerceptionTags)
		{
			Observations.Add(HitRes.Actor != nullptr && HitRes.Actor->ActorHasTag(Tag) ? 1.f : 0.f);
		}

		Observations.Add(bHit ? HitRes.Distance / (HitRes.TraceEnd - HitRes.TraceStart).Size() : 1.f);
	}
}

void FTraceSensor::Read(TArray<float>& Reader)
{
	if (Observations.Num() > ObservationSpace.Size)
	{
		Observations.RemoveAt(0, Observations.Num() - ObservationSpace.Size);
	}
	else if (Observations.Num() < ObservationSpace.Size)
	{
		Observations.AddDefaulted(ObservationSpace.Size - Observations.Num());
	}

	for (float& Observation : Observations)
	{
		Reader.Add(Observation);
	}
}

void FTraceSensor::Reset()
{
	Observations.Init(0, ObservationSpace.Size);
}
