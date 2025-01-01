// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonTerminal.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AButtonTerminal::AButtonTerminal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	createButton(0, 3);
	TArray<FGasInfo> ButtonSettings;
	
	//AGasScreen& GasSC;//= CreateDefaultSubobject<AGasScreen>(TEXT("Gas Screen"));


}

// Called when the game starts or when spawned
void AButtonTerminal::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i == 3; i++)
	{
		USceneComponent* tempScene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
		tempScene->AddRelativeLocation(FVector(0.0f, (i * 10.0f), 0.0f));
		ButtonSpawn.Insert(tempScene, i);
	}
	for (int o = 0; o == 3; o++)
	{
		AGasButton* tempButton = CreateDefaultSubobject<AGasButton>(TEXT("Gas Button"));
		tempButton->AddActorLocalOffset(ButtonSpawn[o]->GetRelativeLocation());
		ButtonArray.Insert(tempButton, o);
	}
}

// Called every frame
void AButtonTerminal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AButtonTerminal::createButton(int32 inIndex, int32 finalIndex)
{
	FString sceneName = ("Scene Comp "); 
	FString tester = FString::FromInt(inIndex);
	sceneName += tester;
	FString buttonName = ("Button Comp ");
	buttonName += tester;
	USceneComponent* tempScene = CreateDefaultSubobject<USceneComponent>(FName(sceneName));
	tempScene->AddRelativeLocation(FVector(0.0f, (inIndex * 10.0f), 0.0f));
	ButtonSpawn.Insert(tempScene, inIndex);
	AGasButton* tempButton = CreateDefaultSubobject<AGasButton>(FName(buttonName));
	tempButton->SetButtonMesh(ButtonMesh);
	tempButton->SetActorRelativeLocation(ButtonSpawn[inIndex]->GetRelativeLocation());
	tempButton->ButtonNumber = inIndex + 1;
	tempButton->OnButtonPressed.BindUFunction(this, FName("ButtonActivated"));
	ButtonArray.Insert(tempButton, inIndex);
	

	if (inIndex == finalIndex)
	{
		return;
	}
	inIndex++;
	createButton(inIndex, finalIndex);
}

void AButtonTerminal::MoveGas(FGasInfo InGasInfo, TArray<int>)
{
}



void AButtonTerminal::ButtonActivated(int buttonIndex)
{
	//this switch cases switches between settings of buttons, and applies them for MoveGas function
	switch (buttonIndex)
	{
		float tempFloat;
	case 1:
		if (GasSC->GetGasAmount(1) > 0.0f || GasSC->GetGasAmount(2) > 0.0f)
		{
			tempFloat = 0;
			GasSC->SetGasAmount(-0.02f, 1);
			tempFloat += 0.02f;
			GasSC->SetGasAmount(-0.02f, 2);
			tempFloat += 0.02f;
			GasSC->SetGasAmount(tempFloat, 0);

		}

		break;

	case 2:
		if (GasSC->GetGasAmount(2) > 0.0f)
		{
			tempFloat = 0;
			GasSC->SetGasAmount(-0.02f, 1);
			tempFloat += 0.02f;
			GasSC->SetGasAmount(tempFloat, 0);
		}

		break;
	
	case 3:
		if (GasSC->GetGasAmount(2) > 0.0f)
		{
			tempFloat = 0;
			GasSC->SetGasAmount(-0.02f, 1);
			tempFloat += 0.02f;
			GasSC->SetGasAmount(tempFloat / 2, 0);
			GasSC->SetGasAmount(tempFloat / 2, 1);
		}

		break;
	
	case 4:
		if (GasSC->GetGasAmount(0) > 0.0f)
		{
			tempFloat = 0;
			GasSC->SetGasAmount(-0.02f, 0);
			tempFloat += 0.02f;
			GasSC->SetGasAmount(tempFloat, 2);
		}


		break;

	default:
		
		break;

	}
}



