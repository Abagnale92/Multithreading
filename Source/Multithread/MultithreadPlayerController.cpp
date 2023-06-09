// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultithreadPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "MultithreadCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MyTubeActor.h"
#include <Kismet/KismetMathLibrary.h>
#include "Kismet/GameplayStatics.h"
#include "MySphereActor.h"

AMultithreadPlayerController::AMultithreadPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void AMultithreadPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	queue = new AsyncQueue();
	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	MyWidget = CreateWidget<UMyUserWidget>(this, MyWidgetClass);
	check(MyWidget);
	//MyWidget->AddToPlayerScreen();
	
}

void AMultithreadPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	PointIsValid();
}

void AMultithreadPlayerController::PointIsValid()
{
	FVector2D MousePosition;
	this->GetMousePosition(MousePosition.X, MousePosition.Y);
		// Effettuare il raycast dallo schermo verso il mondo
		FVector WorldLocation, WorldDirection;
		if (UGameplayStatics::DeprojectScreenToWorld(this, MousePosition, WorldLocation, WorldDirection))
		{
			// Effettuare il raycast per trovare l'oggetto colpito
			FHitResult HitResult;
			if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, WorldLocation + WorldDirection * TraceDirection, ECC_Visibility))
			{
				
				AActor* HitActor = HitResult.GetActor();
				if (HitActor)
				{
					FString ActorName = HitActor->GetActorLabel();
					// Controllare se il nome corrisponde al piano
					if (ActorName == "SM_Plane")
					{
						MyWidget->_img2->SetVisibility(ESlateVisibility::Hidden);
						MyWidget->_img->SetVisibility(ESlateVisibility::Visible);
						this->SetMouseCursorWidget(EMouseCursor::Default, MyWidget);
						bIsValidPoint = true;
					}
					else
					{
						MyWidget->_img2->SetVisibility(ESlateVisibility::Visible);
						MyWidget->_img->SetVisibility(ESlateVisibility::Hidden);
						this->SetMouseCursorWidget(EMouseCursor::Crosshairs, MyWidget);
						bIsValidPoint = false;
					}
				}
			}
		}
}

void AMultithreadPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AMultithreadPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AMultithreadPlayerController::OnSetDestinationStarted);
	}
}


// Triggered every frame when the input is held down
void AMultithreadPlayerController::OnSetDestinationTriggered()
{
	
	if (bIsValidPoint) {
		if (bCanAddTube) {
			FHitResult Hit;
			
			bool bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
			if (bHitSuccessful)
			{
				if (!bFirstClick)
				{
					pointA = Hit.Location;
					bFirstClick = true;
				}
				else
				{
					bCanAddTube = false;
					pointB = Hit.Location;
					//SpawnActor();
					queue->AddASync(FCallbackDelegate::CreateLambda([this]()-> void {
						FPlatformProcess::Sleep(1);
						AsyncTask(ENamedThreads::GameThread, [this]() {
							SpawnActor();
							pointA = pointB;
							this->bCanAddTube = true;
							});
						}));
				}
			}
		}
		
	}
}

void AMultithreadPlayerController::OnSetDestinationStarted()
{
	FHitResult Hit;
	bool bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	if (bIsValidPoint) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, Hit.Location, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}
}

void AMultithreadPlayerController::SpawnActor()
{
	float _NewLength = FVector::Dist(pointA, pointB);
	FVector _middlePosition = pointA + (pointB - pointA) / 2.f;
	FVector _UpdatePosition = FVector(_middlePosition.X, _middlePosition.Y, 50.f);
	FRotator _Rotation = UKismetMathLibrary::FindLookAtRotation(pointA, pointB);
	AMyTubeActor* tubeActor;
	tubeActor = GetWorld()->SpawnActor<AMyTubeActor>(AMyTubeActor::StaticClass(), _UpdatePosition, _Rotation);
	tubeActor->Height = _NewLength;
	tubeActor->CreateMesh();
	tubeActor->ComputeProperties(_Rotation, _UpdatePosition);
	AMySphereActor* sphereActor;
	sphereActor = GetWorld()->SpawnActor<AMySphereActor>(AMySphereActor::StaticClass(), pointB + FVector(0,0,50.f), _Rotation);
	sphereActor->CreateSphere();
}
