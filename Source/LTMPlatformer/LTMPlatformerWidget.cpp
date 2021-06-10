// Fill out your copyright notice in the Description page of Project Settings.


#include "LTMPlatformerWidget.h"
#include "Components/TextBlock.h"

void ULTMPlatformerWidget::InitailizeHUD(ALTMPlatformerGameMode* GameMode)
{
	if (GameMode)
	{
		Score->SetText(FText::AsNumber(0));
		GameMode->OnCoinsCountChanged.AddDynamic(this, &ULTMPlatformerWidget::SetCoinsCount);
	}
}

void ULTMPlatformerWidget::SetCoinsCount(int32 Count)
{
	Score->SetText(FText::AsNumber(Count));
}


