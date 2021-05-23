#include "SceneRecorder.h"

#include <chrono>
#include "ImageUtils.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Misc/FileHelper.h"

ASceneRecorder::ASceneRecorder()
{
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	SceneCaptureComponent->SetupAttachment(this->GetCameraComponent());

	SceneCaptureComponent->ProjectionType = ECameraProjectionMode::Type::Perspective;
	SceneCaptureComponent->CaptureSource = SCS_FinalColorLDR;
	SceneCaptureComponent->bCaptureOnMovement = false;
	SceneCaptureComponent->bCaptureEveryFrame = false;
	SceneCaptureComponent->ShowFlags.SetEyeAdaptation(false);
	
	PrimaryActorTick.bCanEverTick = false;
}

void ASceneRecorder::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASceneRecorder::BeginPlay()
{
	Super::BeginPlay();

	RenderTarget = NewObject<UTextureRenderTarget2D>();
	RenderTarget->InitAutoFormat(720, 480);
	RenderTarget->UpdateResourceImmediate(true);

	SceneCaptureComponent->TextureTarget = RenderTarget;
}

void ASceneRecorder::Capture()
{	
	TArray<FColor> Bitmap;
	TArray<uint8> CompressedBitmap;
	FTextureRenderTargetResource* Resource = RenderTarget->GameThread_GetRenderTargetResource();
	FIntPoint CaptureSize = Resource->GetSizeXY();

	FReadSurfaceDataFlags Flags(RCM_UNorm, CubeFace_MAX);
	Flags.SetLinearToGamma(false);

	SceneCaptureComponent->CaptureScene();
	Resource->ReadPixels(Bitmap, Flags);
	for (auto& Color : Bitmap)
	{
		Color.A = 255;
	}

	uint64_t Epochs = static_cast<uint64_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	FString FileName = FString::Printf(TEXT("C:/Tema/Temp/%llu.png"), Epochs);
	
	FImageUtils::CompressImageArray(CaptureSize.X, CaptureSize.Y, Bitmap, CompressedBitmap);
	FFileHelper::SaveArrayToFile(CompressedBitmap, *FileName);
}

void ASceneRecorder::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	
}

void ASceneRecorder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

