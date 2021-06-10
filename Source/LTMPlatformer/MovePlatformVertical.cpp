// Fill out your copyright notice in the Description page of Project Settings.


#include "MovePlatformVertical.h"

// Sets default values
AMovePlatformVertical::AMovePlatformVertical()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("MyMesh");

	direction = 5;

}

// Called when the game starts or when spawned
void AMovePlatformVertical::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMovePlatformVertical::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation();

	if (NewLocation.Z > 600)
	{
		direction = direction * -1;
	}

	if (NewLocation.Z < 200)
	{
		direction = direction * -1;
	}

	NewLocation.Z += direction * 0.4f;

	SetActorLocation(NewLocation);
}

