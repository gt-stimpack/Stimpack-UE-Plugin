// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomLogger.h"

// Sets default values
ACustomLogger::ACustomLogger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACustomLogger::BeginPlay()
{
	Super::BeginPlay();
	if(logFile == fileDir) logFile.Append(TEXT("MyLog.csv"));
	if(FileManager.FileExists(*logFile))
	{
		FileManager.DeleteFile(*logFile);
	}
	FFileHelper::SaveStringToFile(TEXT("TS,RQ,FPS\n"), *logFile);
}

void ACustomLogger::Log(FString string)
{
	if(logFile == fileDir) logFile.Append(TEXT("MyLog.csv"));
	if(FileManager.FileExists(*logFile))
    {
		FFileHelper::SaveStringToFile(
			string, *logFile,
			FFileHelper::EEncodingOptions::AutoDetect,
			&IFileManager::Get(),
			FILEWRITE_Append
		);
    }
    else
    {
		FFileHelper::SaveStringToFile(
			string, *logFile
		);
	}
}

// Called every frame
void ACustomLogger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
