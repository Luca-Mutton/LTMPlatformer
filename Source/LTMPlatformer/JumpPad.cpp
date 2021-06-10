// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpPad.h"
#include "Components/SphereComponent.h"
#include "LTMPlatformerCharacter.h"
#include "Math/Vector.h"

// Sets default values
AJumpPad::AJumpPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->SetupAttachment(SceneComponent);
	SphereCollider->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	//creates a static mesh componwnt
	PadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadMesh"));
	//PadMesh->SetupAttachment(SphereCollider);
	PadMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

}

// Called when the game starts or when spawned
void AJumpPad::BeginPlay()
{
	Super::BeginPlay();
	SphereCollider->OnComponentBeginOverlap.AddUniqueDynamic(this, &AJumpPad::OnOverlapBegin);
}

// Called every frame
void AJumpPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AJumpPad::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ALTMPlatformerCharacter* Character = Cast<ALTMPlatformerCharacter>(OtherActor);
		

	FVector velocity(0.0f, 0.0f, 1000.0f);

	if (Character)
	{
		Character->LaunchCharacter(velocity, true, true);
	}
}

