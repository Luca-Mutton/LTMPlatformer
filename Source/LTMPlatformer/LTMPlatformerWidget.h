// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LTMPlatformerGameMode.h"
#include "LTMPlatformerWidget.generated.h"

/**
 * 
 */
UCLASS()
class LTMPLATFORMER_API ULTMPlatformerWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* Score;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* DistanceCounter;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* EnemyScore;

public:
	UFUNCTION(BlueprintCallable)
		void InitailizeHUD(ALTMPlatformerGameMode* GameMode);

	UFUNCTION(BlueprintCallable)
		void SetCoinsCount(int32 Count);
};
