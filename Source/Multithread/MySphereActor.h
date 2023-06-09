// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MySphereActor.generated.h"

class UProceduralMeshComponent;
UCLASS()
class MULTITHREAD_API AMySphereActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMySphereActor();
	void CreateSphere();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, Category = "Sphere")
	UMaterialInterface* myMaterial;

	UPROPERTY(EditAnywhere, Category = "Sphere")
	int32 Slices = 32;// fette orizzontali
	UPROPERTY(EditAnywhere, Category = "Sphere")
	int32 Stacks = 16; //fette verticali
	UPROPERTY(EditAnywhere, Category = "Sphere")
	float Radius = 50.0f;

};
