// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UMeshUtils.generated.h"

/**
 * 
 */
UCLASS()
class MULTITHREAD_API UUMeshUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	public:

	UFUNCTION(BlueprintCallable, Category = "Texture Utils")
	static UStaticMesh* ConvertProceduralMeshComponentToSM(UProceduralMeshComponent* ProcMesh);
};
