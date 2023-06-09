// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyTubeActor.generated.h"

class UProceduralMeshComponent;

UCLASS(config=Game)
class MULTITHREAD_API AMyTubeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyTubeActor();
	void CreateMesh();
	void ComputeProperties(FRotator const& rotation, FVector const& pos );
protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;
	
	void GenerateIstancedMesh();
	
public:	
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* MeshComp;

	UPROPERTY()
	UStaticMesh* _StaticMesh;

	UPROPERTY()
	UInstancedStaticMeshComponent* _InstanceMeshComp;


	UPROPERTY(EditAnywhere, Category = "Tube")
	UMaterialInterface* myMaterial;

	
	UPROPERTY(EditAnywhere, Category = "Tube")
	int32 Slices = 32;
	UPROPERTY(EditAnywhere, Category = "Tube")
	float Height = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Tube")
	float Radius = 50.0f;

	FVector _UpdatePosition;
	FRotator _Rotation;
};
