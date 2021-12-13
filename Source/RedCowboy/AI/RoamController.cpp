// Fill out your copyright notice in the Description page of Project Settings.


#include "RoamController.h"

#include "AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/CharacterMovementComponent.h"

ARoamController::ARoamController()
{
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
}

// FPathFollowingRequestResult ARoamController::MoveTo(const FAIMoveRequest& MoveRequest,
//                                                     FNavPathSharedPtr* NavigationPath)
// {
// 	auto res = Super::MoveTo(MoveRequest, NavigationPath);
//
// 	AAICharacter* AIPawn = Cast<AAICharacter>(GetPawn());
// 	if (AIPawn != nullptr)
// 	{
// 		// find out which way is forward
// 		const FRotator ControllerYawRotation(0, GetControlRotation().Yaw, 0);
//
// 		MoveRequest.GetDestination();
// 		
// 		// rotate towards controller rotation
// 		AIPawn->RotateActor(ControllerYawRotation.Yaw + (4 > 0 ? 0 : 180));
// 	}
//
// 	return res;
// }

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

void ARoamController::SetInteractingActor(AActor* Actor)
{
	AAICharacter* AIPawn = Cast<AAICharacter>(GetPawn());
	if (AIPawn != nullptr)
	{
		BBComponent->SetValueAsObject("InteractingActor", Actor);
		AIPawn->InteractionTarget = Actor; // set interacting target
		AIPawn->GetCharacterMovement()->bOrientRotationToMovement = Actor == nullptr; // switch automatic orientation
	}
}
