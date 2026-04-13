// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

#include <zmq.hpp>
#include "CustomLogger.h"
#include "Scalability.h"

#include "AppController.generated.h"

UCLASS()
class GAME2_API AAppController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAppController();
	ACustomLogger* myLogger = nullptr;

	IPlatformFile& fileManager = FPlatformFileManager::Get().GetPlatformFile();
	FString fileDir = FPaths::ProjectConfigDir();
	FString userFile = fileDir;

	zmq::context_t context;
	zmq::socket_t initSocket{context, zmq::socket_type::req};
	zmq::socket_t rqSocket{context, zmq::socket_type::rep};
	zmq::socket_t flSocket{context, zmq::socket_type::push};

	float flTimer     = 0;
	float rqTimer     = 0;
	float fpsTimer    = 0;
	int tickCounter   = 0;
	int fps           = 0;
	std::vector<int> fpsHistory;

	int currentRQ = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
