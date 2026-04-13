// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game2GameMode.h"
#include "Game2Character.h"
#include "UObject/ConstructorHelpers.h"

AGame2GameMode::AGame2GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
