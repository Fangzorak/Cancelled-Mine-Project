// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "ButtonTerminal.h"
#include "GasButton.generated.h"


UCLASS()
class EMBERFADEMINE_API AGasButton : public AActor
{
	GENERATED_BODY()

	class UTimelineComponent* AnimButton;

public:
	// Sets default values for this actor's properties
	AGasButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	UFUNCTION()
	FVector GetButtonPosition();

	UFUNCTION(BlueprintCallable, meta=(DisplayName="SetButtonPos"))
	void SetButtonPosition(FVector InPosition);

	//This function should modify gas values according to the table
	UFUNCTION(BlueprintCallable)
	int ModifyGasValues();

	//This function was responsible for moving the button, but is now deprecated
	UFUNCTION()
	void MoveButtonDown();
	
	//Also deprecated like above
	UFUNCTION()
	void MoveButtonUp();

	//Function responsible for interacting wit the player, takes in actor that interacts with it.
	UFUNCTION(BlueprintCallable)
	void InteractWithPlayer();
	
	//Function that updates the position of the button over each tick
	UFUNCTION()
	void ButtonTimelineUpdate(float InTime);
	//Function that is called once the button is fully pressed or unpressed
	UFUNCTION()
	void ButtonTimelineFinished();

	UFUNCTION(BlueprintCallable)
	void PlayerButtonMove();

	UFUNCTION(BlueprintCallable)
	void SetButtonMesh(UStaticMeshComponent* InStaticMesh);
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = true))
	FVector Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = true))
	FLinearColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	AButtonTerminal* OwnerTerminal;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Mesh", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* ButtonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gas Operations", meta = (AllowPrivateAccess = true))
	int ButtonNumber;

	DECLARE_DELEGATE_TwoParams(ButtonPressed, bool, int);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category "Gas Operations")
	ButtonPressed OnButtonPressed;
	
	
	//Delegate responsible for handlind functions in AnimButtonTimeline. Need TimelineComponent.h in order to work
	FOnTimelineEvent TimelineFinished{};

	FOnTimelineFloat InterpFunction{};

	//Declare delegate function to bind with ButtonTimelineUpdated
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = true))
	UCurveFloat* animCurve;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gas Operations", meta = (AllowPrivateAccess = true))
	bool isPressed;
};
