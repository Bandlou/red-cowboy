// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "AIModule/Classes/Perception/AIPerceptionStimuliSourceComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/SphereComponent.h"
#include "Perception/AISense_Sight.h"
#include "RedCowboy/AI/AICharacter.h"
#include "RedCowboy/GUI/NPCInteractionWidget.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set interaction variables
	LockableAICharacter = nullptr;
	MaxInteractionAngle = 70.f;
	MaxInteractionDistance = 500.f;

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

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create an AI perception stimuli source
	AIPerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(
		TEXT("AIPerceptionStimuliSource"));
	AIPerceptionStimuliSource->RegisterForSense(UAISense_Sight::StaticClass());

	// Create a detection sphere
	DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
	DetectionSphere->InitSphereRadius(1000);

	// Find the Widget and assigned to NPCInteractionBPClass
	static ConstructorHelpers::FClassFinder<UUserWidget> NPCInteractionWbpClass(
		TEXT("/Game/RedCowboy/Blueprints/GUI/NPCInteractionWBP"));
	if (NPCInteractionWbpClass.Class != nullptr)
		NPCInteractionClass = NPCInteractionWbpClass.Class;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Create GUI widgets and add them to viewport
	if (IsValid(NPCInteractionClass))
	{
		NPCInteractionWidget = CreateWidget<UNPCInteractionWidget>(GetWorld(), NPCInteractionClass);

		if (NPCInteractionWidget != nullptr)
		{
			NPCInteractionWidget->AddToViewport();
			NPCInteractionWidget->SetInputType(false);
			NPCInteractionWidget->SetCanLock(false);
			NPCInteractionWidget->SetIsLocked(bIsAICharacterLocked);
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("Unable to add NPCInteractionWidget to viewport!"));
	}
}

void APlayerCharacter::LockCharacter(FKey Key)
{
	CheckInputType(Key);
	if (LockableAICharacter != nullptr)
	{
		bIsAICharacterLocked = true;

		// Update NPCInteractionWidget (GUI)
		if (NPCInteractionWidget != nullptr)
			NPCInteractionWidget->SetIsLocked(true);
		else
			UE_LOG(LogTemp, Warning, TEXT("Unable to update NPCInteractionWidget!"));
	}
}

void APlayerCharacter::UnlockCharacter()
{
	if (bIsAICharacterLocked) // If not already unlocked
	{
		bIsAICharacterLocked = false;

		// Update NPCInteractionWidget (GUI)
		if (NPCInteractionWidget != nullptr)
			NPCInteractionWidget->SetIsLocked(false);
		else
			UE_LOG(LogTemp, Warning, TEXT("Unable to update NPCInteractionWidget!"));
	}
}

void APlayerCharacter::Run()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxRunningSpeed;
}

void APlayerCharacter::StopRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkingSpeed;
}

void APlayerCharacter::MoveForward(float Value)
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

void APlayerCharacter::MoveRight(float Value)
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

void APlayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::HandleInteraction(bool bDebug = false)
{
	TArray<AActor*> OverlappingActors;
	DetectionSphere->GetOverlappingActors(OverlappingActors);

	if (bDebug) // DEBUG: detection sphere & max interaction sphere
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), DetectionSphere->GetScaledSphereRadius(),
		                50, FColor::Orange, false, -1, 0, 0);
		DrawDebugSphere(GetWorld(), GetActorLocation(), MaxInteractionDistance,
		                50, FColor::Yellow, false, -1, 0, 0);
	}

	const FVector PlayerForward = GetActorForwardVector();
	const FVector PlayerLocation = GetActorLocation();
	AActor* LockableActor = nullptr;
	float LockableActorScore = 0;
	for (const auto Actor : OverlappingActors)
	{
		if (Actor != this)
		{
			FVector PlayerToActor = Actor->GetActorLocation() - PlayerLocation;
			const float Angle = FMath::RadiansToDegrees(
				acosf(
					FVector::DotProduct(PlayerForward, PlayerToActor) /
					(PlayerForward.Size() * PlayerToActor.Size())
				));
			const float Distance = PlayerToActor.Size();

			FColor DebugLineColor;

			// Only lockable if within max interaction angle & distance
			if (Angle <= MaxInteractionAngle && Distance <= MaxInteractionDistance)
			{
				const float DistanceScore = 1 - (Distance / MaxInteractionDistance); // € [0;1]
				const float AngleScore = 1 - (Angle / MaxInteractionAngle); // € [0;1]
				const float TotalScore = 2.f * AngleScore + DistanceScore;

				if (TotalScore > LockableActorScore)
				{
					LockableActorScore = TotalScore;
					LockableActor = Actor;
				}

				DebugLineColor = FColor::Green;
			}
			else if (Angle <= MaxInteractionAngle)
				DebugLineColor = FColor::Yellow;
			else
				DebugLineColor = FColor::Orange;

			if (bDebug) // DEBUG: non-lockable actor to player line
				DrawDebugLine(GetWorld(), Actor->GetActorLocation(), GetActorLocation(),
				              DebugLineColor, false, -1, 0, 2);
		}
	}

	// Update lockable character and unlock previous one if necessary
	LockableAICharacter = Cast<AAICharacter>(LockableActor);
	if (LockableAICharacter == nullptr && bIsAICharacterLocked) UnlockCharacter();

	// Update NPCInteractionWidget (GUI)
	if (NPCInteractionWidget != nullptr)
	{
		bool bCanLock = LockableAICharacter != nullptr;
		NPCInteractionWidget->SetCanLock(bCanLock);
		if (bCanLock)
			NPCInteractionWidget->SetName(LockableAICharacter->CharacterName);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Unable to update NPCInteractionWidget!"));

	if (bDebug && LockableAICharacter != nullptr) // DEBUG: lockable actor name+score log
		UE_LOG(LogTemp, Warning, TEXT("[Lockable] %s (%f)"), *LockableAICharacter->CharacterName, LockableActorScore);
}

void APlayerCharacter::CheckInputType(FKey Key)
{
	// Update NPCInteractionWidget input type
	if (NPCInteractionWidget != nullptr)
	{
		bool bIsGamepad = !Key.IsMouseButton() && Key.IsGamepadKey();
		NPCInteractionWidget->SetInputType(bIsGamepad);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Unable to set NPCInteractionWidget input type!"));
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleInteraction(true);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Lock", IE_Pressed, this, &APlayerCharacter::LockCharacter);
	PlayerInputComponent->BindAction("Lock", IE_Released, this, &APlayerCharacter::UnlockCharacter);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::StopRunning);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);
}
