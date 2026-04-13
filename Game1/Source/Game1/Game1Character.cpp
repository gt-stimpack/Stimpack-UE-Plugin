// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game1Character.h"
#include "Game1Projectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"


//////////////////////////////////////////////////////////////////////////
// AGame1Character

AGame1Character::AGame1Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	TurnRateGamepad = 45.f;

	// Create a CameraComponent
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

}

void AGame1Character::BeginPlay()
{
	// Call the base class
	Super::BeginPlay();

	FindCapturer(); // For Screenshot
	emul_file.Append(TEXT("emul.txt"));
	FString emul_string;

	if (fileManager.FileExists(*emul_file))
	{
        FFileHelper::LoadFileToString(emul_string, *emul_file, FFileHelper::EHashOptions::None);

		if (emul_string.Len() < 5)
		{
			emul_rq1 = FCString::Atoi(*emul_string.Mid(0, 1));
			emul_fps1 = FCString::Atoi(*emul_string.Mid(1, 3));
			UE_LOG(LogTemp, Warning, TEXT("Emulation Mode: %d %d"), emul_rq1, emul_fps1);
		}
		else if( emul_string.Len() >= 8)
		{
			emul_rq1 = FCString::Atoi(*emul_string.Mid(0, 1));
			emul_fps1 = FCString::Atoi(*emul_string.Mid(1, 3));
			emul_rq2 = FCString::Atoi(*emul_string.Mid(5, 1));
			emul_fps2 = FCString::Atoi(*emul_string.Mid(6, 3));
			UE_LOG(LogTemp, Warning, TEXT("Emulation Mode: %d %d %d %d"), emul_rq1, emul_fps1, emul_rq2, emul_fps2);
		}

		emul_mode = true;
		// GEngine->bUseFixedFrameRate = false;
		// GEngine->bSmoothFrameRate = true;
		// GEngine->SmoothedFrameRateRange = FFloatRange(0, 60); // default 60
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AGame1Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &AGame1Character::OnPrimaryAction);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AGame1Character::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AGame1Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AGame1Character::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AGame1Character::LookUpAtRate);

	// Screenshot key
	PlayerInputComponent->BindAction("StartCapture", IE_Pressed, this, &AGame1Character::StartCapture);
	// Emulation Profile
	PlayerInputComponent->BindAxis("EmulationProfile", this, &AGame1Character::SetEmulationProfile);
}

void AGame1Character::FindCapturer()
{
	TArray<AActor*> sceneCapturers;
	if(UWorld *world = GetWorld())
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACapturer::StaticClass(), sceneCapturers);
	}

	for(AActor *sceneCapturer: sceneCapturers)
	{
		currentCapturer = Cast<ACapturer>(sceneCapturer);
	}

}

void AGame1Character::StartCapture()
{
	if(currentCapturer != nullptr)
	{
		if(GEngine) {GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Capture Start")));}
		currentCapturer->startCapture = true;
	}
	else
	{
		if(GEngine) {GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Capturer is not set")));}
	}
}

void AGame1Character::SetEmulationProfile(float profile_index)
{
	if(emul_mode)
	{
		if(profile_index == 1 && emul_rq1 != -1 && emul_fps1 != -1)
		{
			Scalability::FQualityLevels qualityLevel = Scalability::GetQualityLevels();
			qualityLevel.SetFromSingleQualityLevel((int)emul_rq1);
			Scalability::SetQualityLevels(qualityLevel, true);

			GEngine->bUseFixedFrameRate = false;
			GEngine->bSmoothFrameRate = true;
			GEngine->SmoothedFrameRateRange = FFloatRange(0, (int)emul_fps1);
		}
		else if(profile_index == 2 && emul_rq2 != -1 && emul_fps2 != -1)
		{
			Scalability::FQualityLevels qualityLevel = Scalability::GetQualityLevels();
			qualityLevel.SetFromSingleQualityLevel((int)emul_rq2);
			Scalability::SetQualityLevels(qualityLevel, true);

			GEngine->bUseFixedFrameRate = false;
			GEngine->bSmoothFrameRate = true;
			GEngine->SmoothedFrameRateRange = FFloatRange(0, (int)emul_fps2);
		}
	}
}

void AGame1Character::OnPrimaryAction()
{
	// Trigger the OnItemUsed Event
	OnUseItem.Broadcast();
}

void AGame1Character::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnPrimaryAction();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AGame1Character::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

void AGame1Character::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AGame1Character::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AGame1Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AGame1Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

bool AGame1Character::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AGame1Character::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AGame1Character::EndTouch);

		return true;
	}

	return false;
}
