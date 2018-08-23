// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"

#define OUT

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
	//ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume
	// If the actor that opens is in the volume
	//if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		OnOpen.Broadcast();
	}
	else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

		TArray<AActor*> OverlappingActors;

		//find all overlapping actors
		if (PressurePlate)
		{
			PressurePlate->GetOverlappingActors(OUT OverlappingActors);
			//return total mass

			//auto reference
			//const emphasizes that the actor does not change during the for loop
			//https://community.gamedev.tv/t/iterating-over-tarray-with-for/9404
			//Must use * instead of & 
			for (const auto* Actor : OverlappingActors)
			{
				//Mass is stored inside of the actor primitive component
				TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();

				//UE_LOG(LogTemp, Warning, TEXT("Overlapping Actor : %s"), *Actor->GetName());
			}
		}
	return TotalMass;
}
