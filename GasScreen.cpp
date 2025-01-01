// Fill out your copyright notice in the Description page of Project Settings.


#include "GasScreen.h"

// Sets default values
AGasScreen::AGasScreen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GasAmount.Init(3, 0.0f);
	GasAmount[0] = 5.0f;
	GasAmount[3] = 7.0f;
}

// Called when the game starts or when spawned
void AGasScreen::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGasScreen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AGasScreen::GetGasAmount(int inIndex)
{
	return GasAmount[inIndex];
}

void AGasScreen::SetGasAmount(float gasChange, int inIndex)
{
	GasAmount[inIndex] += gasChange;
}

