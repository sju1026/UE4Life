// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 플레이어의 이동 속도를 가져와 speed에 할당
	// 1. 소유 폰 얻어 오기
	auto ownerPawn = TryGetPawnOwner();
	// 2. 플레이어로 캐스팅
	auto player = Cast<ATPSPlayer>(ownerPawn);

	if (player) {
		// 3. 이동 속도가 필요
		FVector velocity = player->GetVelocity();
		// 4. 플레이어의 전방 벡터가 필요
		FVector forwardVector = player->GetActorForwardVector();
		// 5. speed에 값(내적) 할당하기
		speed = FVector::DotProduct(forwardVector, velocity);
		// 6. 좌우 속도 할당하기
		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);

		// 플레이어가 현재 공중에 있는지 여부를 기억
		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();
	}
}

void UPlayerAnim::PlayerAttackAnim()
{
	Montage_Play(attackAnimMontage);
}
