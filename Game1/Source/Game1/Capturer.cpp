// Fill out your copyright notice in the Description page of Project Settings.


#include "Capturer.h"

// Sets default values
ACapturer::ACapturer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACapturer::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACapturer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(startCapture == true)
	{
		TakeScreenshot();
		startCapture = false;
	}
}

void ACapturer::TakeScreenshot()
{
	FString fileName = FPaths::ProjectConfigDir() + "/screenshot/" + "frame_" + ToStringWithZeros(frameIndex) + ".png";
	FScreenshotRequest::RequestScreenshot(fileName, false, false);
	frameIndex++;
}


FString ACapturer::ToStringWithZeros(int32 index)
{
	FString strIndex = FString::FromInt(index);
	int32 indexLen = strIndex.Len();
	int32 zeros = fileNameDigits - indexLen;

	if(zeros < 0){
		UE_LOG(LogTemp, Error, TEXT("MaxDigits of ImageCounter Overflow!"));
		return strIndex;
	}

	FString strZeros = "";
	for(size_t i=0;i<zeros;i++){
		strZeros += "0";
	}
	strIndex = strZeros + strIndex;

	return strIndex;
}
