// Copyright Alexander Cherry

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void UOpenDoor::OpenDoor() {
	//GetOwner()->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
	OnOpenRequest.Broadcast();
}

void UOpenDoor::CloseDoor() {
	GetOwner()->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the Trigger Volume
	if (GetTotalMassOnPlate() > TriggerMass) { 
		OpenDoor();
		// reset door timer
		DoorLastOpenTime = GetWorld()->GetTimeSeconds();
	}

	// check if door timer has finished
	if (GetWorld()->GetTimeSeconds() - DoorLastOpenTime > DoorCloseDelay) {
		CloseDoor();
	}
	
}

float UOpenDoor::GetTotalMassOnPlate() {
	float TotalMass = 0.f;

	// Find overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass;  }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (const auto& Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

