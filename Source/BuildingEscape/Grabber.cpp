// Copyright Alexander Cherry

#include "BuildingEscape.h"
#include "Grabber.h"

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
	/// Ensure Physics Handler Component is attached
	FindPhysicsHandleComponent();
	/// Bind Input Component to Grab action
	SetupInputComponent();
		
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if physics handle is attached
	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

/// look for the attached physics handle
void UGrabber::FindPhysicsHandleComponent() {
	/// Make sure Physics Handle Component is attached
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		/// Throw Error
		UE_LOG(LogTemp, Error, TEXT("%s missing Physics Handle Component"), *GetOwner()->GetName());
	}
}

/// Look for Attached Input Component and Bind it to Grabber
void UGrabber::SetupInputComponent() {
	// Make sure physics component is attached
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent != nullptr) {
		/// Bind the Grab Action to the Grabber and Release functions
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing Input Component"), *GetOwner()->GetName());
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() {
	/// Line trace out to reach distance
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	/// Line Trace (ray-cast) out to reach distance
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return HitResult;
}

// Grab the object
void UGrabber::Grab() {
	/// LINE TRACE Try and reach and actors with physics body
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); // for us, mesh component
	auto ActorHit = HitResult.GetActor();

	/// if we hit, then attach physics handle
	if(!ActorHit || !PhysicsHandle) {return;}
	if (true) {
		PhysicsHandle->GrabComponent (
			ComponentToGrab,
			NAME_None, // no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
	}
}

// Release the object
void UGrabber::Release() {
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

// Get the Reach Line start
FVector UGrabber::GetReachLineStart() {
	/// Get player viewpoint location/rotation
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);
	return PlayerViewpointLocation;
}

// Get the Reach Line end
FVector UGrabber::GetReachLineEnd() {
	/// Get player viewpoint location/rotation
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);
	return PlayerViewpointLocation + (Reach * PlayerViewpointRotation.Vector());
}
