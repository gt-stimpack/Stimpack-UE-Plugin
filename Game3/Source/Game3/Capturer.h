// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Engine/GameViewportClient.h"
#include "Misc/Paths.h"

#include "Capturer.generated.h"

UCLASS()
class GAME3_API ACapturer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACapturer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// For Screenshoot
	bool startCapture = false;
	int fileNameDigits = 6;
	int frameIndex = 0;
	FString ToStringWithZeros(int32 index);

	void TakeScreenshot();
};
