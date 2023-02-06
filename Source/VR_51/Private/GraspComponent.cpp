// Fill out your copyright notice in the Description page of Project Settings.


#include "GraspComponent.h"
#include "EnhancedInputComponent.h"
#include "VR_Player.h"
#include "VRHandAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "PickUpActor.h"
#include "Components/TextRenderComponent.h"
#include "Components/SphereComponent.h"
#include "MotionControllerComponent.h"
#include "Components/BoxComponent.h"

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

	if (bIsGrab)
	{
		//DrawGrabRange();
		prevLocation = grabedObject->GetActorLocation();
	}
}

void UGraspComponent::SetupPlayerInputComponent(UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(grip_right, ETriggerEvent::Started, this, &UGraspComponent::GripRightAction);
	PlayerInputComponent->BindAction(grip_right, ETriggerEvent::Completed, this, &UGraspComponent::GripRightRelease);
	PlayerInputComponent->BindAction(trigger_right, ETriggerEvent::Started, this, &UGraspComponent::TriggerRightAction);
	PlayerInputComponent->BindAction(trigger_right, ETriggerEvent::Completed, this, &UGraspComponent::TriggerRightAction);
	PlayerInputComponent->BindAction(trigger_right_touch, ETriggerEvent::Started, this, &UGraspComponent::TrigerRightTouch);
	PlayerInputComponent->BindAction(trigger_right_touch, ETriggerEvent::Completed, this, &UGraspComponent::TrigerRightTouchEnd);
	PlayerInputComponent->BindAction(thumb_right_touch, ETriggerEvent::Started, this, &UGraspComponent::ThumbRightTouch);
	PlayerInputComponent->BindAction(thumb_right_touch, ETriggerEvent::Completed, this, &UGraspComponent::ThumbRightTouchEnd);
}

void UGraspComponent::GripRightAction(const FInputActionValue& value)
{
	rightHandAnim->PoseAlphaGrasp = value.Get<float>();
	// 오른손으로 물체 잡기
	GrabObject(player->rightHand);
}

void UGraspComponent::GripRightRelease(const struct FInputActionValue& value)
{
	bIsGrab = false;
	ReleaseObject(player->rightHand);
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
	if (myGrabType == EGrabType::ELine)
	{
		// 방법 1 - LineTrace 방식
		FVector startLoc = selectHand->GetComponentLocation();
		FVector endLoc = startLoc + selectHand->GetRightVector() * grabDistance;
		FHitResult hitInfo;

		if (GetWorld()->LineTraceSingleByProfile(hitInfo, startLoc, endLoc, TEXT("PickUp")) && grabedObject == nullptr)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, hitInfo.GetActor()->GetName());
			hitInfo.GetActor()->AttachToComponent(selectHand, FAttachmentTransformRules::KeepWorldTransform);
			grabedObject = Cast<APickUpActor>(hitInfo.GetActor());
		}
	}
	else if (myGrabType == EGrabType::ESweep)
	{
		// 방법 2 - SphereTrace 방식
		FVector center = selectHand->GetComponentLocation();
		//FVector endLoc = center + selectHand->GetRightVector() * grabDistance;
		FHitResult hitInfo;
		FCollisionQueryParams params;
		params.AddIgnoredActor(player);

		if (GetWorld()->SweepSingleByChannel(hitInfo, center, center, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(grabDistance), params) && grabedObject == nullptr)
		{
			player->rightLog->SetText(FText::FromString(hitInfo.GetActor()->GetName()));
			grabedObject = Cast<APickUpActor>(hitInfo.GetActor());
			// 만일, 검색된 물체의 physics가 simulate 중이라면...
			if (IsValid(grabedObject))
			//if (grabedObject != nullptr)
			{
				physicsState = grabedObject->sphereComp->IsSimulatingPhysics();
				if (physicsState)
				{
					// 대상의 physics를 꺼준다.
					grabedObject->sphereComp->SetSimulatePhysics(false);
				}

				hitInfo.GetActor()->AttachToComponent(selectHand, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GrabPoint"));
			}
		}
	}
	else if (myGrabType == EGrabType::EOverlap)
	{
		// 방법 3 - 오버랩 방식으로 검사하기
		FVector center = selectHand->GetComponentLocation();
		TArray<FOverlapResult> hitInfos;
		FCollisionObjectQueryParams objectQuerry;
		FCollisionQueryParams params;
		objectQuerry.AddObjectTypesToQuery(ECC_WorldDynamic);

		if (GetWorld()->OverlapMultiByObjectType(hitInfos, center, FQuat::Identity, objectQuerry, FCollisionShape::MakeSphere(grabDistance), params))
		{
			// 부딪힌 모든 대상을 순회한다.
			for (FOverlapResult& hitInfo : hitInfos)
			{
				// 검사 대상 중에 PickUpActor 클래스인 경우에만 손에 붙인다.
				APickUpActor* picks = Cast<APickUpActor>(hitInfo.GetActor());
				if (picks != nullptr)
				{
					hitInfo.GetActor()->AttachToComponent(selectHand, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GrabPoint"));
				}
			}
		}
	}

	bIsGrab = true;
}

void UGraspComponent::ReleaseObject(USkeletalMeshComponent* selectHand)
{
	// 만일, 잡고 있던 물체가 있다면...
	if (grabedObject != nullptr)
	{
		// 잡고 있던 물체를 떼어낸다.
		grabedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		// 물체의 본래 피직스 on/off 여부를 되돌려준다.
		grabedObject->sphereComp->SetSimulatePhysics(physicsState);
		//FVector dir = player->rightController->GetComponentLocation() - prevLocation;
		//dir.Normalize();
		FVector dir = player->rightController->GetPhysicsLinearVelocity();
		dir.Normalize();
		//GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, FString::Printf(TEXT("%.3f, %.3f, %.3f"), curDir.X, curDir.Y, curDir.Z));
		grabedObject->sphereComp->AddImpulse(dir * throwPower);

		FVector dTheta = player->rightController->GetPhysicsAngularVelocityInDegrees();
		//dTheta = selectHand->GetComponentTransform().TransformVector(dTheta);
		grabedObject->sphereComp->AddTorqueInDegrees(dTheta * throwPower);

		// grabedObject 포인터 변수를 nullptr로 변경한다.
		grabedObject = nullptr;
	}

}

void UGraspComponent::DrawGrabRange()
{
	if (myGrabType == EGrabType::ELine)
	{
		// 선 그리기(Line)
		FVector startLoc = player->rightHand->GetComponentLocation();
		FVector endLoc = startLoc + player->rightHand->GetRightVector() * grabDistance;
		DrawDebugLine(GetWorld(), startLoc, endLoc, FColor::Cyan, false, -1, 0, 2);
	}
	else if (myGrabType == EGrabType::ESweep || myGrabType == EGrabType::EOverlap)
	{
		FVector center = player->rightHand->GetComponentLocation();

		// 선 그리기(Sphere)
		DrawDebugSphere(GetWorld(), center, grabDistance, 30, FColor::Cyan, false, -1, 0, 1);
	}
}
