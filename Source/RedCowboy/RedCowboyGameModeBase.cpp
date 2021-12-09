// Copyright Epic Games, Inc. All Rights Reserved.


#include "RedCowboyGameModeBase.h"

ARedCowboyGameModeBase::ARedCowboyGameModeBase()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
		TEXT("/Game/RedCowboy/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != nullptr) DefaultPawnClass = PlayerPawnBPClass.Class;
}
