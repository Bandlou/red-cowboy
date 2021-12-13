// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RoamController.generated.h"

UCLASS()
class REDCOWBOY_API ARoamController : public AAIController
{
	GENERATED_BODY()

public:
	ARoamController();
	
	/** Makes AI go toward specified destination
		 *  @param MoveRequest - details about move
		 *  @param OutPath - optional output param, filled in with assigned path
		 *  @return struct holding MoveId and enum code
		 */
	// virtual FPathFollowingRequestResult MoveTo(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr* OutPath) override;

	/** BB component */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=AI)
	class UBlackboardComponent* BBComponent;

	/** BT component */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=AI)
	class UBehaviorTreeComponent* BTComponent;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=AI)
	class UBehaviorTree* RoamBT;

protected:
	
	/** When possessed pawn changed */
	virtual void OnPossess(APawn* InPawn) override;

public:
	
	/** Set the interaction actor */
	UFUNCTION(BlueprintCallable)
	void SetInteractingActor(AActor* Actor);
	
	/** Set the threatening actor */
	UFUNCTION(BlueprintCallable)
	void SetThreateningActor(AActor* Actor);
};
