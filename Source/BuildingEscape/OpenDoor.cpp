// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h" 

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	ActorThatOpensDoor = GetWorld()->GetFirstPlayerController()->GetPawn();
	BegginingRotation = GetOwner()->GetActorRotation();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume every frame
	if (PressurePlate->IsOverlappingActor(ActorThatOpensDoor))
	{
		//if the player is in the volume, open the door
		OpenDoor();
	}

	if (GetWorld()->GetTimeSeconds() > (LastDoorOpenTime + DoorCloseDelay))
	{
		CloseDoor();
	}
}


void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(BegginingRotation + FRotator(0.0f, OpenAngle, 0.0f));
	LastDoorOpenTime = GetWorld()->GetTimeSeconds();
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(BegginingRotation);
	LastDoorOpenTime = GetWorld()->GetTimeSeconds();
}



