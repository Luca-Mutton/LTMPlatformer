// Copyright Epic Games, Inc. All Rights Reserved.

#include "LTMPlatformerGameMode.h"
#include "LTMPlatformerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "LTMPlatformerWidget.h"



void ALTMPlatformerGameMode::BeginPlay()
{
	//UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;

	//GameMode = Cast<ULTMPlatformerWidget>(CreateWidget(GetWorld(), GameHUDClass));
	///*check(GameMode);*/

	//GameMode->InitailizeHUD(this);
	//GameMode->AddToViewport();

}

void ALTMPlatformerGameMode::AddCoin()
	{
		TotalCoins += 1;

		OnCoinsCountChanged.Broadcast(TotalCoins);
	}

ALTMPlatformerGameMode::ALTMPlatformerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	
}
