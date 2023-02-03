// Fill out your copyright notice in the Description page of Project Settings.


#include "GraspComponent.h"
#include "EnhancedInputComponent.h"
#include "VR_Player.h"
#include "VRHandAnimInstance.h"


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

	// 플레이어 캐릭터와 오른손 애니메이션 인스턴스를 캐싱한다.
	player = Cast<AVR_Player>(GetOwner());
	rightHandAnim = Cast<UVRHandAnimInstance>(player->rightHand->GetAnimInstance());
	
	if (rightHandAnim != nullptr)
	{
		ResetRightFingers();
	}
}


// Called every frame
void UGraspComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UGraspComponent::SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(grip_right, ETriggerEvent::Triggered, this, &UGraspComponent::GripRightAction);
	PlayerInputComponent->BindAction(grip_right, ETriggerEvent::Completed, this, &UGraspComponent::GripRightAction);
	PlayerInputComponent->BindAction(trigger_right, ETriggerEvent::Triggered, this, &UGraspComponent::TriggerRightAction);
	PlayerInputComponent->BindAction(trigger_right, ETriggerEvent::Completed, this, &UGraspComponent::TriggerRightAction);
	PlayerInputComponent->BindAction(trigger_right_touch, ETriggerEvent::Triggered, this, &UGraspComponent::TrigerRightTouch);
	PlayerInputComponent->BindAction(trigger_right_touch, ETriggerEvent::Completed, this, &UGraspComponent::TrigerRightTouchEnd);
	PlayerInputComponent->BindAction(thumb_right_touch, ETriggerEvent::Triggered, this, &UGraspComponent::ThumbRightTouch);
	PlayerInputComponent->BindAction(thumb_right_touch, ETriggerEvent::Completed, this, &UGraspComponent::ThumbRightTouchEnd);
}

void UGraspComponent::GripRightAction(const FInputActionValue& value)
{
	rightHandAnim->PoseAlphaGrasp = value.Get<float>();
	// 오른손으로 물체 잡기
	GrabObject(player->rightHand);
}

void UGraspComponent::TriggerRightAction(const FInputActionValue& value)
{
	rightHandAnim->PoseAlphaIndexCurl = value.Get<float>();
}

void UGraspComponent::TrigerRightTouch()
{
	rightHandAnim->PoseAlphaPoint = 0;
}

void UGraspComponent::TrigerRightTouchEnd()
{
	rightHandAnim->PoseAlphaPoint = 1;
}

void UGraspComponent::ThumbRightTouch()
{
	rightHandAnim->PoseAlphaThumbUp = 0;
}

void UGraspComponent::ThumbRightTouchEnd()
{
	rightHandAnim->PoseAlphaThumbUp = 1;
}

// 손가락 애니메이션 변수의 기본 값으로 되돌리기
void UGraspComponent::ResetRightFingers()
{
	rightHandAnim->PoseAlphaGrasp = 0;
	rightHandAnim->PoseAlphaIndexCurl = 0;
	rightHandAnim->PoseAlphaPoint = 1;
	rightHandAnim->PoseAlphaThumbUp = 1;
}

// 물체를 잡는 함수
void UGraspComponent::GrabObject(USkeletalMeshComponent* selectHand)
{
	// 방법 1 - LineTrace 방식
	FVector startLoc = selectHand->GetComponentLocation();
	FVector endLoc = startLoc + selectHand->GetRightVector() * grabDistance;
	FHitResult hitInfo;

	if (GetWorld()->LineTraceSingleByProfile(hitInfo, startLoc, endLoc, TEXT("PickUp")))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, hitInfo.GetActor()->GetName());
		hitInfo.GetActor()->AttachToComponent(selectHand, FAttachmentTransformRules::KeepWorldTransform);
	}
}
