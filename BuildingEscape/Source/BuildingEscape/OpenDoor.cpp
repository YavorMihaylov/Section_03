// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/TriggerVolume.h"



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
	if (!PressurePlate) { UE_LOG(LogTemp, Error, TEXT("Preassure Plate not attached in %s"), *Owner->GetName()) return; };
	IsDoorOpen = false;
	// ...
	
}

void UOpenDoor::OpenDoor()
{
	//Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
	OnOpenRequest.Broadcast();
	IsDoorOpen = true;
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//poll every frame
	if (GetTotalMassInTrigger() > TriggerMass)
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}


	if (GetWorld()->GetTimeSeconds() > (DoorCloseDelay + LastDoorOpenTime))//Check if the door was opened longer that the close delay
	{
		CloseDoor();
	}
}

void UOpenDoor::CloseDoor()
{
	if (!Owner) { return; };
	Owner->SetActorRotation(FRotator(0.0f, 0.f, 0.0f));
	IsDoorOpen = false;
}

float UOpenDoor::GetTotalMassInTrigger()
{
	float totalmass = 0.f;

	TArray<AActor*> OverlapingActors;
	if (!PressurePlate) { return totalmass; };
	PressurePlate->GetOverlappingActors(OverlapingActors);

	for (const auto* Actr : OverlapingActors)
	{
		totalmass += Actr->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("Object in trigger zone %d with mass %f"), *Actr->GetName(), totalmass)
	}

	return totalmass;
}