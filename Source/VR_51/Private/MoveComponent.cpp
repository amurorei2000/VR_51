// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "VR_Player.h"


// Sets default values for this component's properties
UMoveComponent::UMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	// 주인인 플레이어 클래스를 캐싱한다.
	player = Cast<AVR_Player>(GetOwner());
	
}


// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMoveComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(leftThumbstick, ETriggerEvent::Triggered, this, &UMoveComponent::Move);
	PlayerInputComponent->BindAction(rightThumbstick, ETriggerEvent::Triggered, this, &UMoveComponent::RotateAxis);
	PlayerInputComponent->BindAction(leftTrigger, ETriggerEvent::Triggered, this, &UMoveComponent::DrawMoveLine);
	PlayerInputComponent->BindAction(leftTrigger, ETriggerEvent::Completed, this, &UMoveComponent::Teleport);
}

void UMoveComponent::Move(const FInputActionValue& value)
{
	FVector2D val = value.Get<FVector2D>();
	FVector direction = FVector(val.Y, val.X, 0);
	
	player->AddMovementInput(direction.GetSafeNormal(), 1, false);
}

void UMoveComponent::RotateAxis(const FInputActionValue& value)
{
	FVector2D axis = value.Get<FVector2D>();

	// axis 값을 이용해서 캐릭터(콘트롤러)를 회전한다.
	player->AddControllerPitchInput(axis.Y * -1.0f);
	player->AddControllerYawInput(axis.X);
}

void UMoveComponent::DrawMoveLine(const FInputActionValue& value)
{
	// 이동할 곡선을 화면에 그린다.
	// 1. 이동할 위치 구간을 구한다.(TArray<FVector>)
	// 1-1. p = p0 + vt - 0.5 * g * t^2

	// 1-2. 0.1초씩 0~50번째 구간을 계산해서 배열에 넣는다.

	// 2. 앞에서 구한 위치 마다 선으로 연결한 그림을 그린다(DrawLine).
}

void UMoveComponent::Teleport(const FInputActionValue& value)
{
	// 가리킨 지점으로 순간 이동(즉시 좌표 변경)한다.

}
