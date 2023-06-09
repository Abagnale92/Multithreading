// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "MyUserWidget.h"
#include "AsyncQueue.h"
#include "MultithreadPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class AMultithreadPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMultithreadPlayerController();

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* SetDestinationClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widget)
	TSubclassOf < class UMyUserWidget> MyWidgetClass;

	UPROPERTY()
	UMyUserWidget* MyWidget;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	void SetupInputComponent() override;
	
	// To add mapping context
	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;
	void PointIsValid();
	void OnSetDestinationTriggered();
	void OnSetDestinationStarted();
	void SpawnActor();
private:
	FVector CachedDestination;
	float FollowTime;
	bool bFirstClick = false;
	FVector pointA;
	FVector pointB;
	float TraceDirection = 10000.f;
	bool bIsValidPoint = false;
	AsyncQueue* queue;
	std::atomic_bool bCanAddTube = true;
};