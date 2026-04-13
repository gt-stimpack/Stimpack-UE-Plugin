// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HAL/FileManager.h"
#include "CustomLogger.generated.h"

UCLASS()
class GAME2_API ACustomLogger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACustomLogger();

	FString fileDir = FPaths::ProjectConfigDir();
	FString logFile = fileDir;
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Log(FString string);
};
