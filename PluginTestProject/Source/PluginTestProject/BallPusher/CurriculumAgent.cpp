#include "CurriculumAgent.h"
#include "EngineUtils.h"

UCurriculumAgentComponent::UCurriculumAgentComponent()
{
}

void UCurriculumAgentComponent::BeginPlay()
{
	Super::BeginPlay();

	const FString Cmd = FCommandLine::Get();
	if (FParse::Param(*Cmd, TEXT("noCurriculum")))
	{
		bIsDisabled = true;
	}

	UWorld* World = GetWorld();
	TActorIterator<AActor> ActorItr = TActorIterator<AActor>(World);

	uint32 ActorIndex = 0;
	while (ActorItr)
	{
		UActorComponent* TargetComponent = ActorItr->GetComponentByClass(UBallPusherAgentComponent::StaticClass());
		if (TargetComponent)
		{
			Agents.Add(Cast<UBallPusherAgentComponent>(TargetComponent));
			LevelMap.Add(0);
		}

		ActorIndex++;
		++ActorItr;
	}
}

void UCurriculumAgentComponent::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	Agents.Reset();
	LevelMap.Reset();
}

void UCurriculumAgentComponent::OnObserve()
{
}

void UCurriculumAgentComponent::OnAct(const FAgentAction& Action)
{
	if (bIsDisabled)
	{
		return;
	}

	for (int32 i = 0; i < Action.DiscreteValues.Num(); i++)
	{
		if (LevelMap[i] < Action.DiscreteValues[i])
		{
			ChangeLevel(i, Action.DiscreteValues[i]);
		}
	}
}

void UCurriculumAgentComponent::ChangeLevel(int32 ActorIndex, uint32 Level)
{
	AStaticMeshActor* Arena = Agents[ActorIndex]->Arena;
	FVector ArenaScale = Arena->GetActorScale3D();

	FVector ArenaOrigin;
	FVector ArenaExtent;
	Arena->GetActorBounds(true, ArenaOrigin, ArenaExtent);

	ABlockingVolume* Goal = Agents[ActorIndex]->Goal;
	FVector GoalLocation = Goal->GetActorLocation();

	AStaticMeshActor* LeftWall = Agents[ActorIndex]->LeftWall;
	FVector LeftWallLocation = LeftWall->GetActorLocation();
	
	AStaticMeshActor* RightWall =  Agents[ActorIndex]->RightWall;
	FVector RightWallLocation = RightWall->GetActorLocation();

	switch (Level)
	{
	case 1:
		//float ScaleFac = 0.08f / 0.181f;
		Arena->GetStaticMeshComponent()->SetWorldScale3D(FVector(-0.1f, 0.08f, ArenaScale.Z));
		Arena->SetActorLocation(FVector(ArenaOrigin.X, 115.f, ArenaOrigin.Z + ArenaExtent.Z));
		Goal->SetActorLocation(FVector(GoalLocation.X, -42.46f, GoalLocation.Z));
		LeftWall->SetActorLocation(FVector(-1135, LeftWallLocation.Y, LeftWallLocation.Z));
		RightWall->SetActorLocation(FVector(-715, RightWallLocation.Y, RightWallLocation.Z));
		break;
	case 2:
		Arena->GetStaticMeshComponent()->SetWorldScale3D(FVector(-0.13f, 0.1002f, ArenaScale.Z));
		Arena->SetActorLocation(FVector(ArenaOrigin.X, 77.05f, ArenaOrigin.Z + ArenaExtent.Z));
		Goal->SetActorLocation(FVector(GoalLocation.X, -122.46f, GoalLocation.Z));
		LeftWall->SetActorLocation(FVector(-1195, LeftWallLocation.Y, LeftWallLocation.Z));
		RightWall->SetActorLocation(FVector(-655, RightWallLocation.Y, RightWallLocation.Z));
		break;
	case 3:
		Arena->GetStaticMeshComponent()->SetWorldScale3D(FVector(-0.16f, 0.1204f, ArenaScale.Z));
		Arena->SetActorLocation(FVector(ArenaOrigin.X, 32.05f, ArenaOrigin.Z + ArenaExtent.Z));
		Goal->SetActorLocation(FVector(GoalLocation.X, -202.46f, GoalLocation.Z));
		LeftWall->SetActorLocation(FVector(-1255, LeftWallLocation.Y, LeftWallLocation.Z));
		RightWall->SetActorLocation(FVector(-595, RightWallLocation.Y, RightWallLocation.Z));
		break;
	case 4:
		Arena->GetStaticMeshComponent()->SetWorldScale3D(FVector(-0.2f, 0.1406f, ArenaScale.Z));
		Arena->SetActorLocation(FVector(ArenaOrigin.X, -7.95f, ArenaOrigin.Z + ArenaExtent.Z));
		Goal->SetActorLocation(FVector(GoalLocation.X, -282.46f, GoalLocation.Z));
		LeftWall->SetActorLocation(FVector(-1335, LeftWallLocation.Y, LeftWallLocation.Z));
		RightWall->SetActorLocation(FVector(-515, RightWallLocation.Y, RightWallLocation.Z));
		break;
	case 5:
		Arena->GetStaticMeshComponent()->SetWorldScale3D(FVector(ArenaScale.X, 0.1608f, ArenaScale.Z));
		Arena->SetActorLocation(FVector(ArenaOrigin.X, -47.95f, ArenaOrigin.Z + ArenaExtent.Z));
		Goal->SetActorLocation(FVector(GoalLocation.X, -362.46f, GoalLocation.Z));
		break;
	case 6:
		Arena->GetStaticMeshComponent()->SetWorldScale3D(FVector(ArenaScale.X, 0.181f, ArenaScale.Z));
		Arena->SetActorLocation(FVector(ArenaOrigin.X, -87.9f, ArenaOrigin.Z + ArenaExtent.Z));
		Goal->SetActorLocation(FVector(GoalLocation.X, -457.46f, GoalLocation.Z));
		break;
	default:
		return;
	}

	LevelMap[ActorIndex] = Level;
	Agents[ActorIndex]->OnBeginEpisode();
}

void UCurriculumAgentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
