// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Image.h>
//#include "Engine/Texture2D.h"

#include "MyUserWidget.generated.h"
/**
 * 
 */
UCLASS()
class MULTITHREAD_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadonly, meta = (BindWidget))
    TObjectPtr<UImage> _img = nullptr;
    UPROPERTY(BlueprintReadonly, meta = (BindWidget))
    TObjectPtr<UImage> _img2 = nullptr;
};
