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

	/** BB component */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=AI)
	class UBlackboardComponent* BBComponent;

	/** BT component */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=AI)
	class UBehaviorTreeComponent* BTComponent;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=AI)
	class UBehaviorTree* RoamBT;

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
