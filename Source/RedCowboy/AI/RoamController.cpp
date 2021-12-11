// Fill out your copyright notice in the Description page of Project Settings.


#include "RoamController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

ARoamController::ARoamController()
{
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
}

void ARoamController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RoamBT = Cast<UBehaviorTree>(StaticLoadObject(UBehaviorTree::StaticClass(),
	                                              nullptr,
	                                              TEXT("/Game/RedCowboy/AI/BT_Roam")));
	if (RoamBT)
	{
		BBComponent->InitializeBlackboard(*RoamBT->BlackboardAsset);
		BTComponent->StartTree(*RoamBT, EBTExecutionMode::Looped);
	}
}
