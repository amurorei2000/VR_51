// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VR_51_API UMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent);

	UPROPERTY(EditAnywhere, Category=Inputs)
	class UInputAction* leftThumbstick;

	UPROPERTY(EditAnywhere, Category = Inputs)
	class UInputAction* rightThumbstick;

	UPROPERTY(EditAnywhere, Category = Inputs)
	class UInputAction* leftTrigger;


private:
	class AVR_Player* player;
	TArray<FVector> lineLoc;
	
	
	void Move(const struct FInputActionValue& value);
	void RotateAxis(const struct FInputActionValue& value);
	void DrawMoveLine(const struct FInputActionValue& value);
	void Teleport(const struct FInputActionValue& value);
};
