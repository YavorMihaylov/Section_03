// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber onlline"))

	///Look for attached Physics Component
	
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		
		UE_LOG(LogTemp, Warning, TEXT("Physics Handle found in %s"), *PhysicsHandle->GetOwner()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handle MISSING in %s"), *GetOwner()->GetName());
		
	}

	///Look for attached Input Component
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component found in %s"), *InputComponent->GetOwner()->GetName());
		///Bind the input action
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Input Component MISSING in %s"), *GetOwner()->GetName());

	}

		
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key pressed"));
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key released"));
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FRotator PlayerRot;
	FVector PlayerLoc;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerLoc,PlayerRot);

	FVector LineTraceEnd = PlayerLoc + (PlayerRot.Vector() * ReachDistance);

	///Draw a red trace representing the players reach
	DrawDebugLine(GetWorld(),PlayerLoc,LineTraceEnd,FColor(255, 0, 0), false, 0.f, 0, 5.f);
	
	///Set up query parameter for the ray cast
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	///Ray-cast out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(Hit, PlayerLoc, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	AActor *ActorHit = Hit.GetActor();

	///Debug text of what Hit contains
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Object Hit is %s"), *ActorHit->GetName())
	}
	
		
	///See what is hit

	// ...
}



