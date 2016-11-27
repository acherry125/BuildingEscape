// Copyright Alexander Cherry

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	// How far the player can reach
	float Reach = 100.f;
	
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// Check that a physics handle component is attached
	void FindPhysicsHandleComponent();
	// setup attached input component
	void SetupInputComponent();

	const FHitResult GetFirstPhysicsBodyInReach();
	// Get the reach line start
	FVector GetReachLineStart();
	// Get the reach line end
	FVector GetReachLineEnd();

	// Ray-cast and grab what's in reach
	void Grab();
	// release the object
	void Release();
};
