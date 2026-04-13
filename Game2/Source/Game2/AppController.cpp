// Fill out your copyright notice in the Description page of Project Settings.


#include "AppController.h"

AAppController::AAppController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


void AAppController::BeginPlay()
{
	Super::BeginPlay();

	/*** Logger Setting ***/
	TArray<AActor*> foundLoggers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACustomLogger::StaticClass(), foundLoggers);
	if(foundLoggers.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("No loggers in the world."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Logger Found!"));
		myLogger = dynamic_cast<ACustomLogger*>(foundLoggers[0]);
		// FString test( TEXT("This ain't a song for the broken hearted.\n"));
		// if(myLogger) myLogger->Log(test);
	}

	/*** Init RQ 0 ***/
	Scalability::FQualityLevels qualityLevel = Scalability::GetQualityLevels();
	qualityLevel.SetFromSingleQualityLevel(currentRQ);
	Scalability::SetQualityLevels(qualityLevel, true);

	/*** Read User Info: codec/CQ/FL_SLO ***/
	userFile.Append(TEXT("userinfo.txt"));
	FString userInfoString;
	if(fileManager.FileExists(*userFile))
	{
        FFileHelper::LoadFileToString(userInfoString, *userFile, FFileHelper::EHashOptions::None);
		UE_LOG(LogTemp, Warning, TEXT("User Info: %s"), *userInfoString);
	}
	else
	{
		if(myLogger) myLogger->Log(FString::Printf(TEXT("userinfo.txt is not found...\n")));
		UE_LOG(LogTemp, Warning, TEXT("userinfo.txt is not found..."));
	}

	/*** Contact Faker manager ***/
	std::string userInfo(TCHAR_TO_UTF8(*userInfoString));
	initSocket.connect("tcp://localhost:10000");

	// Send user info "h264XX"
	zmq::message_t message(9), reply;
	memcpy(message.data(), userInfo.c_str(), 9);
	initSocket.send(message, 0);

	// FL Port
	initSocket.recv(&reply, 0);
    const char *flPort = reply.data<const char>();
	int fl = atoi(flPort);
	FString flPortString = FString::FromInt(fl);
	initSocket.send(message, 0); // ACK

	initSocket.recv(&reply, 0);
    const char *rqPort = reply.data<const char>();
	int rq = atoi(rqPort);
	FString rqPortString = FString::FromInt(rq);

	initSocket.close();

	if(myLogger) myLogger->Log(FString::Printf(TEXT("FL Port %s, RQ Port %s\n"), *flPortString, *rqPortString));
	UE_LOG(LogTemp, Warning, TEXT("FL Port %s, RQ Port %s"), *flPortString, *rqPortString);

	FString flPortAddr = FString(TEXT("tcp://*:")) + flPortString;
	FString rqPortAddr = FString(TEXT("tcp://*:")) + rqPortString;
	UE_LOG(LogTemp, Warning, TEXT("flAddr %s, rqAddr %s"), *flPortAddr, *rqPortAddr);

	std::string flPortAddrStr(TCHAR_TO_UTF8(*flPortAddr));
	std::string rqPortAddrStr(TCHAR_TO_UTF8(*rqPortAddr));

	flSocket.bind(flPortAddrStr.c_str());
	rqSocket.bind(rqPortAddrStr.c_str());

	if(myLogger) myLogger->Log(FString::Printf(TEXT("TS\tRQ\tFL\tFPS\n")));
}

void AAppController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	rqSocket.close();
	flSocket.close();

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void AAppController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*** Count FL every 100 ms ***/
	tickCounter++;
	fpsTimer += DeltaTime;
	if(fpsTimer > 0.1f)
	{
		float avgDelta = fpsTimer / tickCounter;
		fps = (int)(1.0f/avgDelta);
		fpsHistory.push_back(fps);

		long int ts = static_cast<long int>(std::time(nullptr));
		if(myLogger) myLogger->Log(FString::Printf(TEXT("%ld\t%d\t%d\t%d\n"), ts, currentRQ, (int)(avgDelta*1000), fps));

		fpsTimer = 0;
		tickCounter = 0;
	}

	/*** Report FL every 1 s to Faker Manager ***/
	flTimer += DeltaTime;
	if(flTimer >= 1)
	{
		flTimer = 0;
		int sum = 0;
		for(int elem : fpsHistory)
		{
			sum += elem;
		}
		int fpsForFL = (int)((float)sum/fpsHistory.size());
		int flToSend = (int)(1.0f/fpsForFL*1000);
		fpsHistory.clear();
		UE_LOG(LogTemp, Warning, TEXT("flToSend: %d, histroy: %d, deltaTime %f"), flToSend, fpsHistory.size(), DeltaTime);

		// Send flToSend...
		zmq::message_t flMsg(4);
		memcpy(flMsg.data(), &flToSend, sizeof(flToSend));
		flSocket.send(flMsg, 0);
	}

	/*** Check received RQ every 5 s from Faker Manager ***/
	rqTimer += DeltaTime;
	if(rqTimer >= 1.5)
	{
		rqTimer = 0;
		zmq::message_t rqMsg;
		int rq = -1;
		char tempRq;

		int res = rqSocket.recv(&rqMsg, 1); // ZMQ_DONTWAIT 1
		UE_LOG(LogTemp, Warning, TEXT("Try to get RQ, %d"), res);
		if(res > 0)
		{
			rqSocket.send(rqMsg, 0); // ACK
			memcpy(&tempRq, rqMsg.data(), 1);
			rq = (int)tempRq;
			UE_LOG(LogTemp, Warning, TEXT("RQ, %d"), rq);

			Scalability::FQualityLevels qualityLevel = Scalability::GetQualityLevels();
			qualityLevel.SetFromSingleQualityLevel(rq);
			Scalability::SetQualityLevels(qualityLevel, true);

			currentRQ = rq;
		}
	}
}
