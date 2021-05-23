#include "CartPoleAgent.h"

#include "Sensor/VectorSensor.h"
#include "Sensor/VectorSensorComponent.h"
#include "PhysicsEngine/PhysicsConstraintActor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

UCartPoleAgent::UCartPoleAgent()
{
	SensorComponent = CreateDefaultSubobject<UVectorSensorComponent>(TEXT("SensorComponent"));
	SensorComponent->InitializeSensor();
}

void UCartPoleAgent::BeginPlay()
{
	Super::BeginPlay();

	MeshComponent = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());

	DefaultCartLoc = MeshComponent->GetComponentLocation();
	DefaultPoleLoc = Pole->GetStaticMeshComponent()->GetComponentLocation();
}

void UCartPoleAgent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UCartPoleAgent::OnBeginEpisode()
{
	Super::OnBeginEpisode();

	MeshComponent->SetWorldLocation
	(
		DefaultCartLoc,
		false,
		nullptr,
		ETeleportType::ResetPhysics
	);

	MeshComponent->SetAllPhysicsLinearVelocity(FVector(0.f, 0.f, 0.f));
	MeshComponent->SetAllPhysicsAngularVelocityInDegrees(FVector(0.f, 0.f, 0.f));

	Pole->SetActorLocationAndRotation
	(
		DefaultPoleLoc,
		FQuat::MakeFromEuler(FVector(0.f, 0.f, 0.f)),
		false,
		nullptr,
		ETeleportType::ResetPhysics
	);

	const int32 Direction = FMath::RandRange(0.f, 1.f) > 0.5f ? -1 : 1;

	Pole->GetStaticMeshComponent()->AddImpulse(FVector::RightVector * Direction * 3.f, NAME_None, true);
	Pole->GetStaticMeshComponent()->SetAllPhysicsLinearVelocity(FVector(0.f, 0.f, 0.f));
	Pole->GetStaticMeshComponent()->SetAllPhysicsAngularVelocityInDegrees(FVector(0.f, 0.f, 0.f));
}

void UCartPoleAgent::OnObserve()
{
	SensorComponent->GetSensor()->AddObservation(MeshComponent->GetComponentLocation().Y); //Cart location
	SensorComponent->GetSensor()->AddObservation(MeshComponent->GetPhysicsLinearVelocity().Y); //Cart velocity
	SensorComponent->GetSensor()->AddObservation(SwingSensor->GetConstraintComp()->GetCurrentTwist()); //Pole angle
	SensorComponent->GetSensor()->AddObservation(Pole->GetStaticMeshComponent()->GetPhysicsAngularVelocityInDegrees().X); //Pole velocity
}

void UCartPoleAgent::OnAct(const FAgentAction& Action)
{
	const float Delta = GetWorld()->GetDeltaSeconds();

	if (Action.DiscreteValues.Num() == 0)
	{
		return;
	}

	switch (Action.DiscreteValues[0])
	{
	case 0:
		//MeshComponent->AddForce(FVector::RightVector * 1.f * Force * (MeshComponent->GetMass() + Pole->GetStaticMeshComponent()->GetMass()));
		MeshComponent->AddImpulse(FVector::RightVector * 1.f * PushForce * Delta * (MeshComponent->GetMass() + Pole->GetStaticMeshComponent()->GetMass()));
		break;
	case 1:
		//MeshComponent->AddForce(FVector::RightVector * -1.f * Force * (MeshComponent->GetMass() + Pole->GetStaticMeshComponent()->GetMass()));
		MeshComponent->AddImpulse(FVector::RightVector * -1.f * PushForce * Delta * (MeshComponent->GetMass() + Pole->GetStaticMeshComponent()->GetMass()));
		break;
	default:
		break;
	}

	AddReward(1.f);

	float CartLoc = MeshComponent->GetComponentLocation().Y;
	float Twist = SwingSensor->GetConstraintComp()->GetCurrentTwist();
	if (FMath::Abs(Twist) > 15.f || FMath::Abs(CartLoc) > 50 || GetEpisodeStepCount() > 200)
	{
		EndEpisode();
	}
}