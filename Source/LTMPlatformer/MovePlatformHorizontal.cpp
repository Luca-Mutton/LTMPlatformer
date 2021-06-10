// Fill out your copyright notice in the Description page of Project Settings.


#include "MovePlatformHorizontal.h"

// Sets default values
AMovePlatformHorizontal::AMovePlatformHorizontal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("MyMesh");

	direction = 5;
}

// Called when the game starts or when spawned
void AMovePlatformHorizontal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMovePlatformHorizontal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation();

	if (NewLocation.Y > 500)
	{
		direction = direction * -1;
	}

	if (NewLocation.Y < 100)
	{
		direction = direction * -1;
	}

	NewLocation.Y += direction * 0.4f;

	SetActorLocation(NewLocation);
}

