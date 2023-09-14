// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlyaerAnim)
		float speed = 0;

	// 플레이어 좌우 이동 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlyaerAnim)
		float direction = 0;

	virtual void NativeUpdateAnimation(float DeltaSeconds)override;

	// 플레이어가 공중에 떴는지 확인
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlyaerAnim)
		bool isInAir = false;

	// 재생할 공격 애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
		class UAnimMontage* attackAnimMontage;

	// 공격 애니메이션 재생 함수
	void PlayerAttackAnim();
};
