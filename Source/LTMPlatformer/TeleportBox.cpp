// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportBox.h"

ATeleportBox::ATeleportBox()
{
	OnActorBeginOverlap.AddDynamic(this, &ATeleportBox::EnterTeleporter);
	OnActorEndOverlap.AddDynamic(this, &ATeleportBox::ExitTeleporter);
	teleporting = false;
}

void ATeleportBox::BeginPlay()
{
	Super::BeginPlay();
}

void ATeleportBox::EnterTeleporter(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor && otherActor != this)
	{
		if (otherTeleporter)
		{
			ALTMPlatformerCharacter* character = Cast<ALTMPlatformerCharacter>(otherActor);
			if (character && !otherTeleporter->teleporting)
			{
				teleporting = true;
				character->SetActorRotation(otherTeleporter->GetActorRotation());
				character->GetController()->SetControlRotation(character->GetActorRotation());
				character->SetActorLocation(otherTeleporter->GetActorLocation());
			}
		}
	}
}

void ATeleportBox::ExitTeleporter(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor && otherActor != this)
	{
		if (otherTeleporter && !teleporting)
		{
			otherTeleporter->teleporting = false;
		}
	}
}
