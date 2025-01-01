// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Mine_PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerStance : uint8 { Standing, Crouching, Crawling };

UCLASS()
class EMBERFADEMINE_API AMine_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()
	

public:
	// Sets default values for this character's properties
	AMine_PlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//This function takes in FInputAction value reference, and uses it to make character move
	void EnhancedInputMove(const FInputActionValue& Value);
	void EnhancedInputCrawl();
	void EnhancedInputCrouch();
	void EnhancedInputSprint();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	void MoveAction();
	void CrouchAction();
	void CrawlAction();
	void SprintAction();



	UPROPERTY(EditAnywhere)
	int PlayerHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputMappingContext* DefaultInputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* CrawlInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* CrouchInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* SprintInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* PlayerCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsProne;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsChangingState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAcces = "true"))
	bool bMovementBlocked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category  = "Movement", meta  = (AllowPrivateAccess = "true"))
	bool bIsRunning;

	UPROPERTY()
	int currentState;

	UPROPERTY()
	float playerStamina;

	UPROPERTY()
	FTimerHandle StaminaHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAcces = "true"))
	bool BlockCrawl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAcces = "true"))
	bool BlockCrouch;


	

	UFUNCTION()
	void UnCrawl(bool bWantsToCrouch);

	UFUNCTION()
	void Crawl();

	UFUNCTION()
	void ResetChangeTimer();

	UFUNCTION()
	void ResetChangeState();

	UFUNCTION()
	void checkIfObstructed(bool bWantsToCrawl);

	UFUNCTION()
	void Sprint();

	UFUNCTION()
	void StopSprint();

	UFUNCTION()
	void DrainStamina(FTimerHandle InStaminaHandle);

	UFUNCTION()
	void RegenStamina(FTimerHandle InStaminaHandle);

	UFUNCTION()
	void StaminaManager();

	UFUNCTION()
	void StaminaSwapper();

	private:
		class UAIPerceptionStimuliSourceComponent* StimulusSource;
		void SetupStimulusSource();
public:
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess ="true")) //dziala
	EPlayerStance CurrentPlayerStance = EPlayerStance::Standing;
		
		
};
