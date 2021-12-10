// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class REDCOWBOY_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** AI perception stimuli source */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSource;

	/** Detection sphere for interactions */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* DetectionSphere;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Interaction)
	class AAICharacter* LockableAICharacter;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Interaction)
	bool bIsAICharacterLocked;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Interaction)
	float MaxInteractionAngle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Interaction)
	float MaxInteractionDistance;

	/** Exposes NPCInteraction BP class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UUserWidget> NPCInteractionBP;

	// UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=Interaction)
	// class UNPCInteractionWidget* NPCInteractionWidget;
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement)
	float MaxRunningSpeed;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Movement)
	float MaxWalkingSpeed;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Called at start	*/
	virtual void BeginPlay() override;

	/** Called when lock key is pressed */
	void LockCharacter();

	/** Called when lock key is released and when lockable character changes */
	void UnlockCharacter();
	
	/** Called when Run is pressed */
	void Run();

	/** Called when Run is released */
	void StopRunning();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/**
	 * Called via tick to handle interaction
	 */
	void HandleInteraction(bool bDebug);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns AIPerceptionStimuliSource subobject **/
	FORCEINLINE class UAIPerceptionStimuliSourceComponent* GetAIPerceptionStimuliSource() const	{ return AIPerceptionStimuliSource; }
	/** Returns DetectionSphere subobject **/
	FORCEINLINE class USphereComponent* GetDetectionSphere() const { return DetectionSphere; }

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
