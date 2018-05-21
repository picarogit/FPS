// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TP_ThirdPersonGameMode.h"
#include "TP_ThirdPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATP_ThirdPersonGameMode::ATP_ThirdPersonGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Dynamic/Character/BP_Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
