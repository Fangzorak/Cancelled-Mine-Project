// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GasScreen.generated.h"

UCLASS()
class EMBERFADEMINE_API AGasScreen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGasScreen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gas Management", meta=(AllowPrivateAccess=true))
	TArray<float> GasAmount;


public:
	UFUNCTION(BlueprintCallable)
	float GetGasAmount(int inIndex);

	UFUNCTION(BlueprintCallable)
	void SetGasAmount(float gasChange, int inIndex);

};
