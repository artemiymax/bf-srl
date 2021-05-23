#include "Cart.h"

#include "PhysicsEngine/PhysicsConstraintComponent.h"

ACart::ACart()
{	
	UStaticMeshComponent* MeshComponent = GetStaticMeshComponent();
	UStaticMesh* CubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;
	MeshComponent->SetStaticMesh(CubeMesh);
	MeshComponent->SetMobility(EComponentMobility::Movable);
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->BodyInstance.bLockXRotation = true;
	MeshComponent->BodyInstance.bLockYRotation = true;
	MeshComponent->BodyInstance.bLockZRotation = true;
	MeshComponent->BodyInstance.bLockXTranslation = true;
	MeshComponent->BodyInstance.bLockZTranslation = true;

	SetActorScale3D(FVector(0.125, 0.375, 0.125));
}

void ACart::BeginPlay()
{
	Super::BeginPlay();
}

void ACart::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

