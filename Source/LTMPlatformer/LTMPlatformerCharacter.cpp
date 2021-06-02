// Copyright Epic Games, Inc. All Rights Reserved.

#include "LTMPlatformerCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/GameFrameWork/CharacterMovementComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"

//////////////////////////////////////////////////////////////////////////
// ALTMPlatformerCharacter

ALTMPlatformerCharacter::ALTMPlatformerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	JumpHeight = 600.f;
	WalkSpeed = 600.f;
	RunSpeed = 1000.f;

	canDash = true;
	DashDistance = 4000.f;
	DashCoolDown = 1.f;
	DashStop = 0.1f;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ALTMPlatformerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ALTMPlatformerCharacter::DoubleJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ALTMPlatformerCharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ALTMPlatformerCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ALTMPlatformerCharacter::Walk);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ALTMPlatformerCharacter::Dash);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALTMPlatformerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALTMPlatformerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ALTMPlatformerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ALTMPlatformerCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ALTMPlatformerCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ALTMPlatformerCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ALTMPlatformerCharacter::OnResetVR);
}


void ALTMPlatformerCharacter::OnResetVR()
{
	// If LTMPlatformer is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in LTMPlatformer.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ALTMPlatformerCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ALTMPlatformerCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ALTMPlatformerCharacter::Landed(const FHitResult& Hit)
{
	DoubleJumpCounter = 0;
}

void ALTMPlatformerCharacter::DoubleJump()
{
	if (DoubleJumpCounter <= 1)
	{
		ALTMPlatformerCharacter::LaunchCharacter(FVector(0, 0, JumpHeight), false, true);
		DoubleJumpCounter++;
	}
}

void ALTMPlatformerCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void ALTMPlatformerCharacter::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ALTMPlatformerCharacter::Dash()
{
	if (canDash)
	{
		GetCharacterMovement()->BrakingFrictionFactor = 0.f;
		LaunchCharacter(FVector(FollowCamera->GetForwardVector().X, FollowCamera->GetForwardVector().Y, 0).GetSafeNormal() * DashDistance, true, true);
		canDash = false;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &ALTMPlatformerCharacter::StopDash, DashStop, false);
	}
}

void ALTMPlatformerCharacter::StopDash()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ALTMPlatformerCharacter::ResetDash, DashCoolDown, false);
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;
}

void ALTMPlatformerCharacter::ResetDash()
{
	canDash = true;
}

void ALTMPlatformerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ALTMPlatformerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ALTMPlatformerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ALTMPlatformerCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
