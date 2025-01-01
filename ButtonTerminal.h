// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GasButton.h"
#include "GasScreen.h"
#include "ButtonTerminal.generated.h"


USTRUCT()
struct FGasInfo
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<float> InGas;

	UPROPERTY()
	TArray<float> OutGas;

};

UCLASS()
class EMBERFADEMINE_API AButtonTerminal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AButtonTerminal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gas Management", meta=(AllowPrivateAccess=true))
float GasTransfer;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Gas Management", meta=(AllowPrivateAccess=true))
AGasScreen* GasSC;




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category  = "Gas Management", meta=(AllowPrivateAccess=true))
TArray<AGasButton*> ButtonArray;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category =  "Gas Management", meta=(AllowPrivateAccess=true))
TArray<USceneComponent*> ButtonSpawn;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buttons", meta=(AllowPrivateAccess=true))
UStaticMeshComponent* ButtonMesh;

UFUNCTION()
void createButton(int32 inIndex, int32 finalIndex);

UFUNCTION()
void MoveGas(FGasInfo InGasInfo, TArray<int> inIndex);

UFUNCTION()
void ButtonActivated(int buttonIndex);




};
