// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LTMPlatformerGameMode.generated.h"

class UUserWidget;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoinsCountChanged, int32, Score);

UCLASS(minimalapi)
class ALTMPlatformerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Config")
		TSubclassOf<UUserWidget> GameHUDClass;

	//UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	//	class ULTMPlatformerWidget* GameMode;

	UFUNCTION(BlueprintCallable)
		void AddCoin();

	UPROPERTY(VisibleAnywhere)
		int32 TotalCoins = 0;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Delegates")
		FOnCoinsCountChanged OnCoinsCountChanged;

protected:
	virtual void BeginPlay() override;

	ALTMPlatformerGameMode();
};



