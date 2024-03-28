// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

ATank::ATank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
    Super::BeginPlay();

    // Add Input Mapping Context
    PlayerController = Cast<APlayerController>(GetController());
    if (PlayerController)
	{
        if(UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            InputSystem->AddMappingContext(TankMappingContext, 0);
        }
	}
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (PlayerController)
    {
        FHitResult HitResult;
        PlayerController->GetHitResultUnderCursor(
            ECollisionChannel::ECC_Visibility, 
            false, 
            HitResult);

        RotateTurret(HitResult.ImpactPoint);
    }
}

void ATank::Move(const FInputActionValue& Value)
{
    const float YDirectionValue = Value.Get<float>();
    //UE_LOG(LogTemp, Display, TEXT("IA_Move Triggered"));

    if (Controller && (YDirectionValue != 0.f))
    {
        FVector DeltaLocation(0.f); 
        DeltaLocation.X = YDirectionValue * UGameplayStatics::GetWorldDeltaSeconds(this) * Speed;
        AddActorLocalOffset(DeltaLocation, true);
    }
}

void ATank::Turn(const FInputActionValue& Value)
{
    const float XDirectionValue = Value.Get<float>();
    //UE_LOG(LogTemp, Display, TEXT("IA_Move Triggered"));

    if (Controller && (XDirectionValue != 0.f))
    {
        FRotator DeltaRotation = FRotator::ZeroRotator; 
        DeltaRotation.Yaw = XDirectionValue * UGameplayStatics::GetWorldDeltaSeconds(this) * TurnRate;
        AddActorLocalRotation(DeltaRotation, true);
    }
}

void ATank::Fire(const FInputActionValue& Value)
{
    FireProjectile();
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent); 

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::Move);
        EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATank::Turn);
        EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ATank::Fire);
    }
}