// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game3GameMode.h"
#include "Game3Character.h"
#include "UObject/ConstructorHelpers.h"

AGame3GameMode::AGame3GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
