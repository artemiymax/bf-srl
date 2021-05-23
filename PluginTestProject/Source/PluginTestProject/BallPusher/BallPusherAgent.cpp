#include "BallPusherAgent.h"

#include "EngineUtils.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sensor/TraceSensorComponent.h"

UBallPusherAgentComponent::UBallPusherAgentComponent()
{
	SensorComponent = CreateDefaultSubobject<UTraceSensorComponent>(TEXT("SensorComponent"));
}

void UBallPusherAgentComponent::BeginPlay()
{	
	Super::BeginPlay();

	DefaultLoc = GetOwner()->GetActorLocation();
	DefaultRot = GetOwner()->GetActorRotation();

	DefaultBallLoc = Ball->GetActorLocation();
	DefaultBallRot = Ball->GetActorRotation();

	DefaultGoalLoc = Goal->GetActorLocation();
	DefaultGoalRot = Goal->GetActorRotation();

	Goal->OnActorBeginOverlap.AddDynamic(this, &UBallPusherAgentComponent::OnGoalHit);
}

void UBallPusherAgentComponent::OnBeginEpisode()
{
	Super::OnBeginEpisode();

	FVector ArenaOrigin;
	FVector ArenaExtent;
	Arena->GetActorBounds(true, ArenaOrigin, ArenaExtent);

	FVector NewBallLoc = FVector(FMath::FRandRange(ArenaOrigin.X - ArenaExtent.X + 85, ArenaOrigin.X + ArenaExtent.X - 85), FMath::FRandRange(ArenaOrigin.Y - ArenaExtent.Y + 85, ArenaOrigin.Y + ArenaExtent.Y - 85), DefaultBallLoc.Z);
	Ball->SetActorLocationAndRotation
	(
		NewBallLoc,
		DefaultBallRot,
		false,
		nullptr,
		ETeleportType::ResetPhysics
	);

	Ball->GetStaticMeshComponent()->SetAllPhysicsLinearVelocity(FVector(0.f, 0.f, 0.f));
	Ball->GetStaticMeshComponent()->SetAllPhysicsAngularVelocityInDegrees(FVector(0.f, 0.f, 0.f));

	FVector NewAgentLoc;
	do
	{
		NewAgentLoc = FVector(FMath::FRandRange(ArenaOrigin.X - ArenaExtent.X + 85, ArenaOrigin.X + ArenaExtent.X - 85), FMath::FRandRange(ArenaOrigin.Y - ArenaExtent.Y + 85, ArenaOrigin.Y + ArenaExtent.Y - 85), DefaultLoc.Z);
	} while (FMath::Abs(NewBallLoc.X - NewAgentLoc.X) < 80 && FMath::Abs(NewBallLoc.Y - NewAgentLoc.Y) < 80);

	GetOwner()->SetActorLocationAndRotation
	(
		NewAgentLoc,
		FRotator(DefaultRot.Pitch, FMath::FRandRange(-180.f, 180.f), DefaultRot.Roll),
		false,
		nullptr,
		ETeleportType::ResetPhysics
	);

	AStaticMeshActor* Actor = Cast<AStaticMeshActor>(GetOwner());
	Actor->GetStaticMeshComponent()->SetAllPhysicsLinearVelocity(FVector(0.f, 0.f, 0.f));
	Actor->GetStaticMeshComponent()->SetAllPhysicsAngularVelocityInDegrees(FVector(0.f, 0.f, 0.f));
}

void UBallPusherAgentComponent::OnObserve()
{
}

void UBallPusherAgentComponent::OnAct(const FAgentAction& Action)
{
	uint32 TargetAction = Action.DiscreteValues.Num() != 0 ? Action.DiscreteValues[0] : -1;

	AStaticMeshActor* Actor = Cast<AStaticMeshActor>(GetOwner());
	UStaticMeshComponent* Mesh = Actor->GetStaticMeshComponent();
	FVector Direction = FVector::ZeroVector;
	FVector RotationDirection = FVector::ZeroVector;
	
	switch (TargetAction)
	{
	case 0:
		Direction += Actor->GetActorForwardVector();
		break;
	case 1:
		Direction -= Actor->GetActorForwardVector();
		break;
	case 2:
		Direction += Actor->GetActorRightVector();
		break;
	case 3:
		Direction -= Actor->GetActorRightVector();
		break;
	case 4:
		RotationDirection += Actor->GetActorUpVector();
		break;
	case 5:
		RotationDirection -= Actor->GetActorUpVector();
		break;
	default:
		break;
	}

	FRotator ActorRotation = UKismetMathLibrary::RotatorFromAxisAndAngle(RotationDirection, RotateScale * GetWorld()->GetDeltaSeconds());
	Actor->AddActorWorldRotation(ActorRotation);
	Mesh->AddForce(Direction * MoveScale, NAME_None, true);

	AddReward(-1.f / 4000);

	if (GetEpisodeStepCount() >= 4000)
	{
		EndEpisode();
	}
}

void UBallPusherAgentComponent::EndPlay(EEndPlayReason::Type EndPlayReason)
{
}

void UBallPusherAgentComponent::OnGoalHit(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(FName("Ball")))
	{
		AddReward(3.f);
		EndEpisode();
	}
}