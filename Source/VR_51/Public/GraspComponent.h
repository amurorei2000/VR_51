// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GraspComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VR_51_API UGraspComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGraspComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent);

	UPROPERTY(EditAnywhere, Category = Inputs)
	class UInputAction* grip_right;

	UPROPERTY(EditAnywhere, Category = Inputs)
		class UInputAction* trigger_right;

	UPROPERTY(EditAnywhere, Category = Inputs)
		class UInputAction* thumb_right;

private:
	void GripRightAction(const struct FInputActionValue& value);
	void TriggerRightAction(const struct FInputActionValue& value);
	void ThumbRightAction();
};
