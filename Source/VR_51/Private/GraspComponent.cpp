// Fill out your copyright notice in the Description page of Project Settings.


#include "GraspComponent.h"
#include "EnhancedInputComponent.h"


// Sets default values for this component's properties
UGraspComponent::UGraspComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGraspComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGraspComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UGraspComponent::SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(grip_right, ETriggerEvent::Triggered, this, &UGraspComponent::GripRightAction);
	PlayerInputComponent->BindAction(trigger_right, ETriggerEvent::Triggered, this, &UGraspComponent::TriggerRightAction);
	PlayerInputComponent->BindAction(thumb_right, ETriggerEvent::Triggered, this, &UGraspComponent::ThumbRightAction);
}

void UGraspComponent::GripRightAction(const FInputActionValue& value)
{

}

void UGraspComponent::TriggerRightAction(const FInputActionValue& value)
{

}

void UGraspComponent::ThumbRightAction()
{

}
