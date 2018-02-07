// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"


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
	FindPhysicsHandleComponent();
	SetupInputComponent();		
}

///Look for attached Physics Component
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handle MISSING in %s"), *GetOwner()->GetName());
	}
}

///Look for attached Input Component
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
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
	///Line Trace and see if we reach any actors with physics body collision channel set
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	///If hit attach physics handle 
	if (ActorHit)
	{
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true /*allow rotation*/);
	}	
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if the physics handle is attached 
	if (PhysicsHandle->GrabbedComponent)
	{
		//move the object we are holding each frame
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
}


const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	///Set up query parameter for the ray cast
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	///Ray-cast out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(Hit, GetPlayerLoc(), GetLineTraceEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	AActor *ActorHit = Hit.GetActor();

	///Debug text of what Hit contains
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Object Hit is %s"), *ActorHit->GetName())
	}

	return Hit;
}

FVector UGrabber::GetLineTraceEnd()
{
	FVector LineTraceEnd = GetPlayerLoc() + (GetPlayerRot().Vector() * ReachDistance);

	///Draw a red trace representing the players reach. Leave because its nice feedback something has been done
	DrawDebugLine(GetWorld(), GetPlayerLoc(), LineTraceEnd, FColor(255, 0, 0), false, 0.f, 0, 5.f);

	return LineTraceEnd;
}

FRotator UGrabber::GetPlayerRot()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerLoc, PlayerRot);
	return PlayerRot;
}

FVector UGrabber::GetPlayerLoc()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerLoc, PlayerRot);
	return PlayerLoc;
}



