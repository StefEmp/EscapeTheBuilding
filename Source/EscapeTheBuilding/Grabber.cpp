// Copyright Stephane Emptage 2020

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"


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

	SetupInputComponent();
	FindPhysicsHandle();
	
	


}


void UGrabber::Grab()
	{
		UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed"));

		FVector PlayerViewPointLocation;
		FRotator PlayerViewPointRotation;

		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
		FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() *Reach;
		
		FHitResult HitResult = GetFirstPhysicsBodyInReach();
		UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
		//If we hit something then attach the physics handle.
		if(HitResult.GetActor())
		{
			PhysicsHandle->GrabComponentAtLocation( ComponentToGrab, NAME_None, LineTraceEnd);
					
		}
	}
		
void UGrabber::GrabRelease()
	{
		UE_LOG(LogTemp, Warning, TEXT("Grabber Released"));
		PhysicsHandle->ReleaseComponent();
	}

//Checking for Physics Handle Component
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle)
	{
		//Physics is found
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No physics handle component found on %s"), *GetOwner()->GetName());
	}
}
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::GrabRelease);
	}
}



// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() *Reach;
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Get players viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() *Reach;
	
	// Ray-cast out to a certain distance (Reach)

	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	// See what it hits
	AActor* ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Error, TEXT("Line trace has hit: %s"), *(ActorHit->GetName()));
	}
	return Hit;
}