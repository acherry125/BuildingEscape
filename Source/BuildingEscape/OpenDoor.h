// Copyright Alexander Cherry

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenRequest);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OpenDoor();

	void CloseDoor();

	// returns total mass in kg
	float GetTotalMassOnPlate();

	UPROPERTY(BlueprintAssignable)
	FOnOpenRequest OnOpenRequest;
	

private:
	UPROPERTY(EditAnywhere)
	float OpenAngle = -90.f;
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 1.f;
	float DoorLastOpenTime;
	float TriggerMass = 30.f;
};
