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
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FRotator PlayerRot;
	FVector PlayerLoc;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerLoc,PlayerRot);

	UE_LOG(LogTemp, Warning, TEXT("LineTraceDirection vector output = %s"), *PlayerRot.Vector().ToString())

	FVector LineTraceEnd = PlayerLoc + (PlayerRot.Vector() * ReachDistance);

	DrawDebugLine(GetWorld(),PlayerLoc,LineTraceEnd,FColor(255, 0, 0), false, 0.f, 0, 5.f);
	
	//Draw a red trance for the vector

	//Ray-cast out to reach distance

	//See what is hit

	// ...
}

