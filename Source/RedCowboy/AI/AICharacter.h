// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

UCLASS()
class REDCOWBOY_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn Sensing => using AI perception in controller BP instead */
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	// class UPawnSensingComponent* PawnSensing;

public:
	// Sets default values for this character's properties
	AAICharacter();
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Interaction)
	FString CharacterName;

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

	/** Called when Run is pressed */
	UFUNCTION(BlueprintCallable)
	void Run();

	/** Called when Run is released */
	UFUNCTION(BlueprintCallable)
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
	 * Called via PawnSensor when seeing a pawn. 
	 * @param Pawn	The pawn being seen
	 */
	// UFUNCTION()
	// void OnSeePawn(APawn* Pawn);

	// FVector TargetPosition;
	// bool HasTarget;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	/** Returns PawnSensing subobject **/
	// FORCEINLINE class UPawnSensingComponent* GetPawnSensing() const { return PawnSensing; }

};
