// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "RedCowboy/Player/PlayerCharacter.h"

// Sets default values
AAICharacter::AAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set interaction variables
	CharacterName = "Stranger";

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// set movement speeds
	MaxRunningSpeed = 375;
	MaxWalkingSpeed = 150;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkingSpeed;

	// Create a pawn sensing => using AI perception in controller BP instead
	// PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	// PawnSensing->SetPeripheralVisionAngle(40);
	// PawnSensing->SetSensingUpdatesEnabled(true);
	// PawnSensing->bOnlySensePlayers = false; // we want it to react to other NPCs too

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	// Pawn sensing bindings (done within the controller with a BT instead)
	// PawnSensing->OnSeePawn.AddDynamic(this, &AAICharacter::OnSeePawn);
}

void AAICharacter::Run()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxRunningSpeed;
}

void AAICharacter::StopRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkingSpeed;
}

void AAICharacter::MoveForward(float Value)
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

void AAICharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
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

void AAICharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAICharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

// void AAICharacter::OnSeePawn(APawn* Pawn)
// {
// 	APlayerCharacter* player = dynamic_cast<APlayerCharacter*>(Pawn);
// 	if (player != nullptr)
// 	{
// 		HasTarget = true;
// 		TargetPosition = player->GetActorLocation();
// 	}
// }


// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
