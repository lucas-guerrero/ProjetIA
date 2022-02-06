// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiculePlayer.h"
#include "VehiculeIA.h"
#include "IAPathPlayerController.h"

#include <Components/InputComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

AVehiculePlayer::AVehiculePlayer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AVehiculePlayer::BeginPlay()
{
	Super::BeginPlay();
	BindInput();
}

void AVehiculePlayer::BindInput()
{
	InputComponent = NewObject<UInputComponent>(this);
	InputComponent->RegisterComponent();
	if (InputComponent)
	{
		InputComponent->BindAction("ChangeAlgo", IE_Pressed, this, &AVehiculePlayer::Change);
		InputComponent->BindAxis("MoveForward", this, &AVehiculePlayer::MoveForward);
		InputComponent->BindAxis("MoveRight", this, &AVehiculePlayer::MoveRight);
		InputComponent->BindAxis("MoveUp", this, &AVehiculePlayer::MoveUp);
		InputComponent->BindAction("Quit", IE_Pressed, this, &AVehiculePlayer::Quit);

		EnableInput(GetWorld()->GetFirstPlayerController());
	}
}

void AVehiculePlayer::Tick(float DeltaTime)
{
	FVector SteeringForce = Truncate(Direction, MaxForce);
	FVector Acceleration = SteeringForce / Mass;
	Velocity = Truncate(Velocity + Acceleration, MaxSpeed);
	SetActorLocation(GetActorLocation() + Velocity);

	SetActorRotation(FRotator(Velocity.Rotation()));
}

void AVehiculePlayer::Quit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}

void AVehiculePlayer::Change()
{
	AVehiculeIA* IA = Cast<AVehiculeIA>(Vehicule);
	if (IA)
	{
		IA->ChangeAlgo();

		APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);

		AIAPathPlayerController* PlayerController = Cast<AIAPathPlayerController>(Controller);

		if (PlayerController != nullptr)
		{
			PlayerController->UpdateTextAlgo(IA->GetNameAlgo());
		}
	}
}

void AVehiculePlayer::MoveForward(float Value)
{
	Direction = FVector(Value, Direction.Y, Direction.Z);
}

void AVehiculePlayer::MoveRight(float Value)
{
	Direction = FVector(Direction.X, Value, Direction.Z);
}

void AVehiculePlayer::MoveUp(float Value)
{
	Direction = FVector(Direction.X, Direction.Y, Value);
}