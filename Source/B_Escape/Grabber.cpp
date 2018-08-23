// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

//Blank macro of our own
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandle();
	FindInputComponent();
}

void UGrabber::FindInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	/// Look for attached input component
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("InputComponent"));
		/// Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No InputComponent found on %s. Adding one now"), *GetOwner()->GetFName().ToString());
	}
}

void UGrabber::FindPhysicsHandle()
{
	/// Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics handle found on %s. Adding one now"), *GetOwner()->GetFName().ToString());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector		PlayerViepointLocation;
	FRotator	PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViepointLocation,
		OUT PlayerViewpointRotation);
	FVector LineTraceEnd = PlayerViepointLocation + (PlayerViewpointRotation.Vector() * ReachDistance);
	
	//If physics handle points to nullptr, skip the rest of the function to prevent crashing
	if (!PhysicsHandle) { return; }
	//if physics handle attached
	if (PhysicsHandle->GetGrabbedComponent())
	{
		//move the object that we're holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

//Raycast and grab whats in reach
void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key pressed"));

	//LINE TRACE and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	//If we hit something, then attach a physics handle
	if (ActorHit)	//SAame as if(actorhit == nullptr)
	{
		//Attach physics handle
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true //allow rotation
		);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key released"));
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FHitResult HitResult;

	//Set up query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachStart(),
		GetReachEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	return HitResult;
}

FVector UGrabber::GetReachStart()
{
	// Get Player viewpoint
	FVector		PlayerViepointLocation;
	FRotator	PlayerViewpointRotation;

	//Lecture 80
	//The get playerviewpoint function modifies the values of the getter variables.
	//To prevent this, we're using a blank OUT macro which does nothing
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViepointLocation,
		OUT PlayerViewpointRotation);

	return PlayerViepointLocation;
}

FVector UGrabber::GetReachEnd()
{
	// Get Player viewpoint
	FVector		PlayerViepointLocation;
	FRotator	PlayerViewpointRotation;

	//Lecture 80
	//The get playerviewpoint function modifies the values of the getter variables.
	//To prevent this, we're using a blank OUT macro which does nothing
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViepointLocation,
		OUT PlayerViewpointRotation);

	//Raycast out to reach distance
	FVector LineTraceEnd = PlayerViepointLocation + (PlayerViewpointRotation.Vector() * ReachDistance);

	return LineTraceEnd;
}
