// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class TPSPROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* tpsCamComp;
	// RightLeft
	void Turn(float value);
	// UpDown
	void LookUp(float value);
	
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
		float walkSpeed = 600;

	FVector direction;

	void InputHorizontal(float value);

	void InputVertical(float value);

	void InputJump();

	void Move();

	UPROPERTY(VisibleAnywhere, Category = GunMesh)
		class USkeletalMeshComponent* gunMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
		TSubclassOf<class ABullet> bulletFactory;

	void InputFire();

	// 스나이퍼 건 스태틱메시 추가
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
		class UStaticMeshComponent* sniperGunComp; 

	// 유탄총 사용 중인지 여부
	bool bUsingGrenadeGun = true;
	// 유탄총으로 변경
	void ChangeToGrenadeGun();
	// 스나이퍼건으로 변경
	void ChangeToSniperGun();

	// 스나이퍼 조준
	void SniperAim();
	// 스나이퍼 조준 중인지 여부
	bool bSniperAim = false;

	// 스나이퍼 UI 위젯 공장
	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
		TSubclassOf<class UUserWidget> sniperUIFactory;
	// 스나이퍼 UI 위젯 인스턴스
	class UUserWidget* _sniperUI;

	/// 총알파편효과공장
	UPROPERTY(EditAnywhere, Category = BulletFactory)
		class UParticleSystem* bulletEffectFactory;

	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
		TSubclassOf<class UUserWidget> crosshairUIFactory;

	class UUserWidget* _crosshairUI;
};
