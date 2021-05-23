#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "SceneRecorder.generated.h"

UCLASS()
class UNREALAGENTSCORE_API ASceneRecorder : public ACameraActor
{
	GENERATED_BODY()
	
public:	
	ASceneRecorder();

	void Capture();
	
	void Tick(float DeltaTime) override;
	
	UPROPERTY(Category = Recording, VisibleAnywhere)
	USceneCaptureComponent2D* SceneCaptureComponent;

protected:
	void PostInitializeComponents() override;
	
	void BeginPlay() override;
	
	void EndPlay(EEndPlayReason::Type EndPlayReason) override;

private:
	UTextureRenderTarget2D* RenderTarget;
	
	FTextureRenderTargetResource* RenderResource;
};
