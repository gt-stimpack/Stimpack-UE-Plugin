// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game1GameMode.h"
#include "Game1Character.h"
#include "UObject/ConstructorHelpers.h"

AGame1GameMode::AGame1GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
