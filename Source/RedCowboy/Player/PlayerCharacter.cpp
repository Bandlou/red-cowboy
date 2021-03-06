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
#include "RedCowboy/AI/RoamController.h"
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
	MaxInteractionAngle = 90.f;
	MaxLockDistance = 500.f;
	MaxInteractionDistance = 1000.f;
	bIsAttacking = false;
	DialogDisplayTimer = 0.f;
	DialogDisplayTime = 1.5f;
	DialogResponseName = "";
	DialogResponsePhrase = "";

	// set our turn rates for input
	CameraBaseTurnRate = 45.f;
	CameraBaseLookUpRate = 45.f;

	// set movement variables
	MaxRunningSpeed = 375;
	MaxWalkingSpeed = 150;
	bCanRun = true;
	ActorBaseTurnRate = 270.f;
	ActorTurnThreshold = 10.f;

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
	DetectionSphere->InitSphereRadius(MaxInteractionDistance);

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
		bCanRun = false;
		StopRunning();
		GetCharacterMovement()->bOrientRotationToMovement = false; // stop automatic orientation

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
		bIsAttacking = false;
		if (LockableAICharacter != nullptr)
		{
			ARoamController* LockableAICharacterController = Cast<
				ARoamController>(LockableAICharacter->GetController());
			LockableAICharacterController->SetThreateningActor(nullptr);
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("Unable to access threated NPC controller!"));

		bIsAICharacterLocked = false;
		bCanRun = true;
		GetCharacterMovement()->bOrientRotationToMovement = true; // enable automatic orientation

		// Update NPCInteractionWidget (GUI)
		if (NPCInteractionWidget != nullptr)
			NPCInteractionWidget->SetIsLocked(false);
		else
			UE_LOG(LogTemp, Warning, TEXT("Unable to update NPCInteractionWidget!"));
	}
}

void APlayerCharacter::Antagonize()
{
	if (bIsAICharacterLocked)
	{
		if (LockableAICharacter != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Antagonize: WIP"));
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("Cannot antagonize locked character: not found!"));
	}
}

void APlayerCharacter::Defuse()
{
	if (bIsAICharacterLocked && !bIsAttacking) // cannot defuse while being a threat
	{
		if (LockableAICharacter != nullptr)
		{
			// Update NPCInteractionWidget (GUI)
			if (NPCInteractionWidget != nullptr)
			{
				DialogResponseName = LockableAICharacter->CharacterName;
				const FString Phrase = DialogResponseName == "Martha"
					                       ? "Hello."
					                       : DialogResponseName == "Micah"
					                       ? "You are the creepiest man I know."
					                       : "How you keeping?";
				DialogResponsePhrase = DialogResponseName == "Martha"
					                       ? "Hi, mister."
					                       : DialogResponseName == "Micah"
					                       ? "What are you talking about?"
					                       : "Very well, thank you.";

				NPCInteractionWidget->SetDialog("", Phrase);
				DialogDisplayTimer = DialogDisplayTime;
			}
			else
				UE_LOG(LogTemp, Warning, TEXT("Unable to update NPCInteractionWidget!"));

			ARoamController* LockableAICharacterController = Cast<ARoamController>(
				LockableAICharacter->GetController());
			LockableAICharacterController->SetInteractingActor(this);
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("Cannot defuse locked character: not found!"));
	}
}

void APlayerCharacter::Rob()
{
	if (bIsAICharacterLocked)
	{
		if (LockableAICharacter != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Rob: WIP"));
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("Cannot rob locked character: not found!"));
	}
}

void APlayerCharacter::Attack()
{
	if (bIsAICharacterLocked)
	{
		if (LockableAICharacter != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Attack")); // TODO: print subtitles
			bIsAttacking = true;

			ARoamController* LockableAICharacterController = Cast<
				ARoamController>(LockableAICharacter->GetController());
			LockableAICharacterController->SetThreateningActor(this);
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("Cannot attack locked character: not found!"));
	}
}

void APlayerCharacter::Run()
{
	if (bCanRun)
		GetCharacterMovement()->MaxWalkSpeed = MaxRunningSpeed;
}

void APlayerCharacter::StopRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkingSpeed;
}

void APlayerCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.0f && !bIsAttacking)
	{
		// find out which way is forward
		const FRotator ControllerYawRotation(0, Controller->GetControlRotation().Yaw, 0);
		// get forward vector
		const FVector Direction = FRotationMatrix(ControllerYawRotation).GetUnitAxis(EAxis::X);

		// add movement in that direction
		AddMovementInput(Direction, Value);
		// rotate towards controller rotation => switching automatic orient via bOrientRotationToMovement instead
		// RotateActor(ControllerYawRotation.Yaw + (Value > 0 ? 0 : 180));
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.0f && !bIsAttacking)
	{
		// find out which way is right
		const FRotator ControllerYawRotation(0, Controller->GetControlRotation().Yaw, 0);
		// get right vector 
		const FVector Direction = FRotationMatrix(ControllerYawRotation).GetUnitAxis(EAxis::Y);

		// add movement in that direction
		AddMovementInput(Direction, Value);
		// rotate towards controller rotation => switching automatic orient via bOrientRotationToMovement instead
		// RotateActor(ControllerYawRotation.Yaw + (Value > 0 ? 90 : -90));
	}
}

void APlayerCharacter::RotateActor(float DesiredYawRotation)
{
	if (bIsAICharacterLocked)
	{
		if (LockableAICharacter != nullptr)
		{
			const FVector PlayerLocation = GetActorLocation();
			FVector PlayerToActor = LockableAICharacter->GetActorLocation() - PlayerLocation;

			// Extract yaw from vector and convert to degrees
			PlayerToActor.Normalize();
			DesiredYawRotation = FMath::RadiansToDegrees(atan2(PlayerToActor.Y, PlayerToActor.X));
			DesiredYawRotation = fmod(DesiredYawRotation + 360.f, 360.f);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Cannot rotate towards locked character: not found!"));
			return;
		}
	}

	const float ActorYawRotation = GetActorRotation().Yaw;
	const float DesiredYawDelta = fmod((DesiredYawRotation - ActorYawRotation) + 180.f, 360.f) - 180.f;

	// prevent small values from rotating
	if (abs(DesiredYawDelta) >= ActorTurnThreshold)
	{
		// maximal yaw rotation
		const float MaxYawDelta = ActorBaseTurnRate * GetWorld()->GetDeltaSeconds();
		// final yaw rotation added to actor
		const float YawDelta = abs(DesiredYawDelta) < MaxYawDelta
			                       ? DesiredYawDelta
			                       : (DesiredYawDelta > 0 ? MaxYawDelta : -MaxYawDelta);

		AddActorLocalRotation(FRotator(0, YawDelta, 0));
	}
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * CameraBaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * CameraBaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::HandleInteraction(bool bDebug = false)
{
	TArray<AActor*> OverlappingActors;
	DetectionSphere->GetOverlappingActors(OverlappingActors);

	if (bDebug) // DEBUG: detection sphere & max interaction sphere
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), DetectionSphere->GetScaledSphereRadius(),
		                50, FColor::Orange, false, -1, 0, 0);
		DrawDebugSphere(GetWorld(), GetActorLocation(), MaxLockDistance,
		                50, FColor::Yellow, false, -1, 0, 0);
	}

	const FVector PlayerForward = GetActorForwardVector();
	const FVector PlayerLocation = GetActorLocation();

	// Check if locked char is still valid
	if (LockableAICharacter != nullptr && bIsAICharacterLocked)
	{
		const FVector PlayerToActor = LockableAICharacter->GetActorLocation() - PlayerLocation;
		const float Angle = GetAngle(PlayerForward, PlayerToActor);
		const float Distance = PlayerToActor.Size();

		// If no more valid => unlock
		if (Angle > MaxInteractionAngle || Distance > MaxInteractionDistance)
			UnlockCharacter();
		else
		{
			RotateActor(0); // desired yaw is computed in RotateActor, TODO: restructure

			if (bDebug) // DEBUG: locked actor to player line
			{
				const FColor DebugLineColor = Distance <= MaxLockDistance ? FColor::Green : FColor::Yellow;
				DrawDebugLine(GetWorld(), LockableAICharacter->GetActorLocation(), GetActorLocation(),
				              DebugLineColor, false, -1, 0, 2);
			}
		}
	}

	// Find the best lockable character
	if (!bIsAICharacterLocked)
	{
		AActor* LockableActor = nullptr;
		float LockableActorScore = 0;
		for (const auto Actor : OverlappingActors)
		{
			if (Actor != this)
			{
				FVector PlayerToActor = Actor->GetActorLocation() - PlayerLocation;
				const float Angle = GetAngle(PlayerForward, PlayerToActor);
				const float Distance = PlayerToActor.Size();

				FColor DebugLineColor;

				// Only lockable if within max interaction angle & distance
				if (Angle <= MaxInteractionAngle && Distance <= MaxLockDistance)
				{
					const float DistanceScore = 1 - (Distance / MaxLockDistance); // ??? [0;1]
					const float AngleScore = 1 - (Angle / MaxInteractionAngle); // ??? [0;1]
					const float TotalScore = 1.5f * AngleScore + DistanceScore;

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

		// Update lockable character
		LockableAICharacter = Cast<AAICharacter>(LockableActor);

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
	}
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

float APlayerCharacter::GetAngle(FVector A, FVector B)
{
	const float RadAngle = acosf(FVector::DotProduct(A, B) / (A.Size() * B.Size()));
	return FMath::RadiansToDegrees(RadAngle);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleInteraction();

	// Handle dialogs
	if (DialogDisplayTimer > 0)
	{
		DialogDisplayTimer -= DeltaTime;

		if (DialogDisplayTimer <= 0)
		{
			// Update NPCInteractionWidget (GUI)
			if (NPCInteractionWidget != nullptr)
			{
				if (DialogResponsePhrase == "")
					NPCInteractionWidget->StopDialog();
				else
				{
					NPCInteractionWidget->SetDialog(DialogResponseName, DialogResponsePhrase);
					DialogResponsePhrase = "";
					DialogDisplayTimer = DialogDisplayTime;
				}
			}
			else
				UE_LOG(LogTemp, Warning, TEXT("Unable to update NPCInteractionWidget!"));
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Antagonize", IE_Pressed, this, &APlayerCharacter::Antagonize);
	PlayerInputComponent->BindAction("Defuse", IE_Pressed, this, &APlayerCharacter::Defuse);
	PlayerInputComponent->BindAction("Rob", IE_Pressed, this, &APlayerCharacter::Rob);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack);

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
