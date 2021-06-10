// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "GameFramework/Actor.h"
#include "LTMPlatformerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "TeleportBox.generated.h"

/**
 * 
 */
UCLASS()
class LTMPLATFORMER_API ATeleportBox : public ATriggerBox
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	ATeleportBox();

	UFUNCTION()
		void EnterTeleporter(class AActor* overlappedActor, class AActor* otherActor);

	UFUNCTION()
		void ExitTeleporter(class AActor* overlappedActor, class AActor* otherActor);

	UPROPERTY(EditAnywhere, Category = "Teleporter")
		ATeleportBox* otherTeleporter;

	UPROPERTY()
		bool teleporting;
	
};
