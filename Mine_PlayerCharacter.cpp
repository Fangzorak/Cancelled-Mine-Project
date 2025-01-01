// Fill out your copyright notice in the Description page of Project Settings.


#include "Mine_PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/MovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
//#include "Mine_GameMode.h"



// Sets default values
AMine_PlayerCharacter::AMine_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//Setup of the Capsule
	GetCapsuleComponent()->InitCapsuleSize(40.0f, 95.0f);


	PrimaryActorTick.bCanEverTick = true;
	
	//Setup of the camera -> this is necessary so it can be seen among other components
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	//This attaches camer to the capsule, thus making the capsule its parent
	PlayerCamera->SetupAttachment(GetCapsuleComponent());
	//Fixes placement of camera, then makes it use pawn control rotation
	PlayerCamera->AddRelativeLocation(FVector(0.0f, 1.75f, 64.0f));
	PlayerCamera->bUsePawnControlRotation = true;

	//Setup of starting variables
	bMovementBlocked = false;
	bIsRunning = false;
	playerStamina = 1.0f;

	//Sets up the player as a source for sight stimulus
	SetupStimulusSource();
	//CurrentPlayerStance = EPlayerStance::Standing;
	}

// Called when the game starts or when spawned
void AMine_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();



}


//This function makes movement for player possible
void AMine_PlayerCharacter::EnhancedInputMove(const FInputActionValue& Value)
{
	//Sets up constant FVectors and FRorators for player movement from FInputActionValue& Value
	
	//It takes said Value and usess Get function for FVector2D to setup moveVector
	const FVector2d moveVector = Value.Get<FVector2D>();

	//It then makes its constant FRotator from zeroed X and Z axis, while getting its Y rotation from Controller->GetControlRotation()
	const FRotator moveRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

	//Both of those ifs can be called together, which enables the player to move forward/back and to the sides at the same time

	//This if checks if player moves forward, and if movement isnt blocked
	if (moveVector.Y != 0 && bMovementBlocked != true)
	{
		//This sets up FVector directionVector to moveRotation RotateVector Parameter, and takes its Forward Vector from splittin FVector
		FVector directionVector = moveRotation.RotateVector(FVector::ForwardVector);
		//This then adds movement input in the directionVector, and adds its power from Y axis ov moveVector
		AddMovementInput(directionVector, moveVector.Y);
	}
	
	//This if checks if player moves sideways, and if movement isnt blocked
	if (moveVector.X != 0 && bMovementBlocked != true)
	{
		//This sets up directionVector for to moveRotation RotateVector, and takes its Right Vector from splitting FVector
		FVector directionVector = moveRotation.RotateVector(FVector::RightVector);
		//This tehn adds movement inpput in the direction od directionVector, and takes its power from moveVector X axis
		AddMovementInput(directionVector, moveVector.X);
	}

}
//This functon checks player states to determine what to do when they try to crawl
void AMine_PlayerCharacter::EnhancedInputCrawl()
{
	//this if checks if player isnt changing the state, movement isnt blocked, and crawling isnt blocked
	if (bIsChangingState == false && bMovementBlocked != true && BlockCrawl != true)
	{

		//this if the checks if player is crouched, if it is, it attempts to uncrouch them
		if (bIsCrouched == true) { UnCrouch(); }
		
		//If player isnt crouched by this point, proper function executs
		if (bIsCrouched == false)
		{
			//this is a tenary operator that checks if the player is prone. If they are, it UnCrawls them with value false passed in them
			//which means that the player doesnt want to crouch, but wants to get upright. if they arent crawling, they are instead set to crawl.
			(bIsProne == true) ? UnCrawl(false) : Crawl();

			//bool is changed to true since state is being changed, and then timer for StateChange is being reset
			bIsChangingState = true;
			ResetChangeTimer();
		}

	
	}
}

//This function checks player states, to determine what to do when they want to crouch.
void AMine_PlayerCharacter::EnhancedInputCrouch()
{
	//This if checks if player is in a ChangingState, if the movment isnt blocked, and crouching isnt blocked
	if (bIsChangingState == false && bMovementBlocked != true && BlockCrouch != true) 
	{

		//This checks if player is prone prior to crouch attempt. If they are, it UnCrawls them with true, which makes the obstruction check smaller
		//to accomodate for smaller player size in crouch
		if (bIsProne == true)
		{
			UnCrawl(true);
		}

		//If player isnt prone by this point, the proper function executes
		if (bIsProne == false)
		{
		
			//This tenary statements checks if the player is already crouched, and if they are it attempts to UnCrouch them. If the player isnt already
			//crouched, they are set to Crouch.
			(bIsCrouched == true) ? UnCrouch() : Crouch();

			//After a change of state bool is set to true, and after timer goes down, state is being reset, and another change may occur again.
			bIsChangingState = true;
			ResetChangeTimer();
		}

	}
}

//This function chekcs player states, and handles his request for sprint
void AMine_PlayerCharacter::EnhancedInputSprint()
{

	//At the start of every function, currentState is reset to zero, in order to make the code execute cleanly each time
	currentState = 0;

	//These two functions check player state, and setup CurrentState for switchCase, 0 being neutral state, 1 being either crouched or crawling,
	//and 2 being currently Running
	if (bIsProne == true || bIsCrouched == true) { currentState = 1; }
	else if (bIsRunning == true) { currentState = 2; }

	//This checks if movement isnt currently blocked
	if (bMovementBlocked != true)
	{
		//This switch statement checks which state the player is currently in
		switch (currentState)
		{

		//In this case, player is either Prone or Crawling. This then checks which of these two the player is currently in, and attemtps to make
		//them stand up
		case 1:
			bIsProne == true ? UnCrawl(true) : UnCrouch();
						
			break;
		//If the player is currently sprinting, the sprinting is stopped.
		case 2:
			StopSprint();
			break;
		//If the player is both standing up and not currently running, the player starts sprinting.
		default:
			Sprint();
			break;
		}
		//This then checks if the player is not prone, crouched, isnt running and isnt changing states, and makes them run. (Possible redundancy?)
		if (bIsProne == false && bIsCrouched == false && bIsRunning == false && bIsChangingState == false)
		{
			Sprint();
		}
		
	}
}

// Called every frame
void AMine_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input and is called at the players begin play
void AMine_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Uses Parents SetupPlayerInputComponentFunction
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//This creates a pointer variable EnhancedInputComponnet and tries to Cast PlayerInputComponent Through UEnhancedInputComponent to verify
	//if the InputComponent is valid to bind to EnhancedInputComponent
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	//These events Bind BP selected EnhancedInputActions XInputaAction to code made functions EnhancedInputX. Essentialy this enables
	//The UPROPERTY EIA`s to be used in the movement
	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &AMine_PlayerCharacter::EnhancedInputMove);
	EnhancedInputComponent->BindAction(CrawlInputAction, ETriggerEvent::Started, this, &AMine_PlayerCharacter::EnhancedInputCrawl);
	EnhancedInputComponent->BindAction(CrouchInputAction, ETriggerEvent::Started, this, &AMine_PlayerCharacter::EnhancedInputCrouch);
	EnhancedInputComponent->BindAction(SprintInputAction, ETriggerEvent::Started, this, &AMine_PlayerCharacter::EnhancedInputSprint);
	//EnhancedInputComponent->BindAction(SprintInputAction, ETriggerEvent::Canceled, this, &AMine_PlayerCharacter::EnhancedInputSprint);

	//This creates variable pointer to APlayerController, and sets it to a Cast of GetControllert to APlayerController. If Cast succeeds
	//then the variable is correct, and playerController is set.
	APlayerController* playerController = Cast<APlayerController>(GetController());


	//This creats a pointer to UEnhancedInputLocalPlayer, and then validates it through Cast THIS NEEDS TO BE ELABORATED UPON


	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
		(playerController->GetLocalPlayer());

	Subsystem->AddMappingContext(DefaultInputMapping, 0);
}



void AMine_PlayerCharacter::MoveAction()
{
}

void AMine_PlayerCharacter::CrouchAction()
{

}

void AMine_PlayerCharacter::CrawlAction()
{
}

void AMine_PlayerCharacter::SprintAction()
{
}

bool bIsObstructed = true;

void AMine_PlayerCharacter::UnCrawl(bool bWantsToCrouch)
{
	bIsObstructed = false;
	checkIfObstructed(bWantsToCrouch);
	if (bIsObstructed == false)
	{
	GetCapsuleComponent()->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	GetCapsuleComponent()->AddRelativeLocation(FVector(0, 0, 40.0f));
	PlayerCamera->AddRelativeLocation(FVector(0, 0, 64.0f));
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	bIsProne = false;
	bWantsToCrouch == true ? CurrentPlayerStance = EPlayerStance::Crouching : CurrentPlayerStance = EPlayerStance::Standing;
	}
}

void AMine_PlayerCharacter::Crawl()
{
	
	GetCapsuleComponent()->SetWorldScale3D(FVector(1.0f, 1.0f, 0.2f));
	GetCapsuleComponent()->AddRelativeLocation(FVector(0, 0, -60.0f));
	PlayerCamera->AddRelativeLocation(FVector(0.0f, 0.0f, -64.0f));
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	CurrentPlayerStance = EPlayerStance::Crawling;
	bIsProne = true;
}

void AMine_PlayerCharacter::ResetChangeTimer()
{
	FTimerManager& CooldownManager = GetWorldTimerManager();
	FTimerHandle StateCooldownHandle;
	if (!bIsProne)
	{
		bIsCrouched == true ? CurrentPlayerStance = EPlayerStance::Crawling : CurrentPlayerStance = EPlayerStance::Standing;
	}
	CooldownManager.SetTimer(StateCooldownHandle, this, &AMine_PlayerCharacter::ResetChangeState, 0.7f, false, -1);
}

void AMine_PlayerCharacter::ResetChangeState()
{
	bIsChangingState = false;
}

void AMine_PlayerCharacter::checkIfObstructed(bool bWantsToCrawl)
{
	UWorld* MineWorld = GetWorld();
	UCapsuleComponent* MyCapsule = Cast<UCapsuleComponent>(GetCapsuleComponent());
	ECollisionChannel CapsuleCollisionChannel = ECC_WorldStatic;
	

	if(bWantsToCrawl)
	{
		FCollisionShape CapsuleCollisionShape = FCollisionShape::MakeCapsule(40.0f, 5.0f);
		FCollisionQueryParams CapsuleCollisionQueryParams = FCollisionQueryParams::DefaultQueryParam;
		FCollisionResponseParams CapsuleCollsionResponseParams;//= FCollisionResponseParams::DefaultResponseParam ;
		FVector PlayerLocation = (GetActorLocation() + FVector(0.0f, 0.0f, 60.0f));
		bIsObstructed = MineWorld->OverlapBlockingTestByChannel(PlayerLocation, FQuat::Identity, MyCapsule->GetCollisionObjectType(),
			CapsuleCollisionShape, CapsuleCollisionQueryParams, CapsuleCollsionResponseParams);
	}
	
	
	else 
	{
		FCollisionShape CapsuleCollisionShape = FCollisionShape::MakeCapsule(40.0f, 20.0f);
		FCollisionQueryParams CapsuleCollisionQueryParams = FCollisionQueryParams::DefaultQueryParam;
		FCollisionResponseParams CapsuleCollsionResponseParams;//= FCollisionResponseParams::DefaultResponseParam ;
		FVector PlayerLocation = (GetActorLocation() + FVector(0.0f, 0.0f, 60.0f));
		bIsObstructed = MineWorld->OverlapBlockingTestByChannel(PlayerLocation, FQuat::Identity, MyCapsule->GetCollisionObjectType(),
			CapsuleCollisionShape, CapsuleCollisionQueryParams, CapsuleCollsionResponseParams);
	}

	
}

void AMine_PlayerCharacter::Sprint()
{
	bIsChangingState = true;
	bIsRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
	GetCharacterMovement()->MaxAcceleration = 3072.0f;
	StaminaManager();
}

void AMine_PlayerCharacter::StopSprint()
{
	bIsRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxAcceleration = 2048.0f;
	StaminaManager();
	ResetChangeTimer();
}

void AMine_PlayerCharacter::DrainStamina(FTimerHandle InStaminaHandle)
{

	if(playerStamina > 0.0f)
	{ 
		playerStamina = playerStamina - 0.05f;
	}

	else
	{
		StopSprint();
		InStaminaHandle.Invalidate();
	}

	return;

}

void AMine_PlayerCharacter::RegenStamina(FTimerHandle InStaminaHandle)
{

	if(playerStamina < 1.0f)
	{
		playerStamina += 0.01f;
	}
	else
	{
		InStaminaHandle.Invalidate();
	}
}

void AMine_PlayerCharacter::StaminaManager()
{
	FTimerManager& StaminaTimerManager = GetWorldTimerManager();

	StaminaTimerManager.SetTimer(StaminaHandle, this, &AMine_PlayerCharacter::StaminaSwapper, 0.2f, true, -1);
	
}
void AMine_PlayerCharacter::StaminaSwapper()
{
	bIsRunning == true ? DrainStamina(StaminaHandle) : RegenStamina(StaminaHandle);
}
void AMine_PlayerCharacter::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource)
	{
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();

	}



}
//CooldownManager.SetTimer(StateCooldownHandle, this, &AMine_PlayerCharacter::ResetChangeState, 0.3f, false, -1);
