// Fill out your copyright notice in the Description page of Project Settings.


#include "GasButton.h"
#include "Kismet/GameplayStatics.h"
#include "Mine_PlayerCharacter.h"



// Sets default values
AGasButton::AGasButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//animCurve = CreateDefaultSubobject<UCurveFloat>(TEXT("Anim Curve"));
	AnimButton = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline Component"));
	AnimButton->SetTimelineLength(1.0f);
	InterpFunction.BindUFunction(this, FName("ButtonTimelineUpdate"));
	TimelineFinished.BindUFunction(this, FName("ButtonTimelineFinished"));
	ButtonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Button Mesh"));
	//OnButtonPressed.BindUFunction(OwnerTerminal, FName(""))
}

// Called when the game starts or when spawned
void AGasButton::BeginPlay()
{
	Super::BeginPlay();
	//Setup of AnimButton Timeline, goes forward only if animCurve is valid
	Position = GetActorLocation();
	if (animCurve)
	{
		//Adds float curve to Anim Button, from FCurveFloat animCurve
		AnimButton->AddInterpFloat(animCurve, InterpFunction, FName("Alpha"));
		AnimButton->SetTimelineFinishedFunc(TimelineFinished);
		

		AnimButton->SetLooping(false);
		AnimButton->SetIgnoreTimeDilation(true);

		
	}
}

// Called every frame
void AGasButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



FVector AGasButton::GetButtonPosition()
{
	return Position;
}

void AGasButton::SetButtonPosition(FVector InPosition)
{

	Position = InPosition;
}

int AGasButton::ModifyGasValues()
{
	return ButtonNumber;
}

void AGasButton::MoveButtonDown()
{
			
	return;
}

void AGasButton::MoveButtonUp()
{
}

void AGasButton::InteractWithPlayer()
{

	
		
		isPressed == true ? AnimButton->Reverse() : AnimButton->Play();
	
	
}


//Delegate that is called on every update of timeline
void AGasButton::ButtonTimelineUpdate(float InTime)
{
	InTime = AnimButton->GetPlaybackPosition();
	float lerpedValue = FMath::Lerp(Position.Z, Position.Z - 10, InTime);
	SetActorLocation(FVector(Position.X, Position.Y, lerpedValue));
	
	
}
//Delegate that is called when timeline is Finished
void AGasButton::ButtonTimelineFinished()
{
	isPressed == true ? isPressed = false : isPressed = true;
	OnButtonPressed.Execute(isPressed, ButtonNumber);
}

void AGasButton::PlayerButtonMove()
{
	isPressed == true ? AnimButton->Reverse() : AnimButton->Play();
}

void AGasButton::SetButtonMesh(UStaticMeshComponent* InStaticMesh)
{
	ButtonMesh = InStaticMesh;
}





