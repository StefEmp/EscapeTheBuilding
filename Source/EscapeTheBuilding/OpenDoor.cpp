// Copyright Stephane Emptage 2020


#include "OpenDoor.h"
#include "GameFramework/Actor.h"

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

	 InitialYaw = GetOwner()->GetActorRotation().Yaw;
	 CurrentYaw = InitialYaw;
	 TargetYaw += InitialYaw;
	 //TargetYaw = InitialYaw + TargetYaw;

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("The yaw is: %f"), GetOwner()->GetActorRotation().Yaw);
	


	CurrentYaw= FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 2);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	//set actor rotation
	GetOwner()->SetActorRotation(DoorRotation);

// 	float CurrentYaw = GetOwner()->GetActorRotation().Yaw;
// //FRotator OpenDoor

// 	FRotator OpenDoor(0.f, TargetYaw, 0.f);
// //Change Yaw of OpenDoor
// 	OpenDoor.Yaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 2 );
// 	// FRotator CurrentRotation = GetOwner()->GetActorRotation();
// 	// CurrentRotation.Yaw = 90.f;
// 	GetOwner()->SetActorRotation(OpenDoor);
}
