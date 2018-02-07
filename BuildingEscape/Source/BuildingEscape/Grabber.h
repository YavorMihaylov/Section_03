// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	//How far the player can reach an object
	UPROPERTY(EditAnywhere)
	float ReachDistance = 100.f;

	UPhysicsHandleComponent *PhysicsHandle = nullptr;
	UInputComponent *InputComponent = nullptr;
	FVector PlayerLoc;
	FRotator PlayerRot;

	//Ray cast and Grab what is in reach
	void Grab();

	//Called when Grab is released
	void Release();

	//Find attached physics handle component
	void FindPhysicsHandleComponent();

	//setup attached input component
	void SetupInputComponent();

	//Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();
	
	//Get Line Trace end
	FVector GetLineTraceEnd();

	//Get the current player view point
	FRotator GetPlayerRot();
	FVector GetPlayerLoc();


};
