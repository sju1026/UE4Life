// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "Bullet.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Blueprint/UserWidget.h>

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// call SkeletalMesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		TempMesh(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);

		// 2. Mesh Component Set Rotation
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// 3-1. TPS Camera -> SpringArmComponent
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;
	springArmComp->CameraLagSpeed = 5.0f;

	// 3-2. Camera
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);

	bUseControllerRotationYaw = true;

	JumpMaxCount = 2;

	// 4. 총 스켈레탈메시 컴포넌트 등록
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMehsComp"));
	// 4-1. 부모 컴포넌트 Mesh컴포넌트로 설정
	gunMeshComp->SetupAttachment(GetMesh());
	// 4-2. 스켈레탈메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(
		TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (TempGunMesh.Succeeded()) {
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		gunMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
	}

	// 5. 스나이퍼건 컴포넌트 등록
	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	// 5-1. 부모 컴포넌트를 Mesh 컴포넌트로 등록
	sniperGunComp->SetupAttachment(GetMesh());
	// 5-2. 스태틱메시 데이터 로드
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempSniperMesh(TEXT("StaticMesh'/Game/SniperGun/sniper1.sniper1'"));
	// 5-3. 데이터로드가 성공했다면
	if (TempSniperMesh.Succeeded()) {
		// 5-4. 스태틱메시 데이터 할당
		sniperGunComp->SetStaticMesh(TempSniperMesh.Object);
		// 5-5. 위치 조정하기
		sniperGunComp->SetRelativeLocation(FVector(-22, 55, 120));
		// 5-6. 크기 조정하기
		sniperGunComp->SetRelativeScale3D(FVector(0.15f));
	}

}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// 1. 스나이퍼 UI 위젯 인스턴스 생성
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);

	ChangeToSniperGun();
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move();
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &ATPSPlayer::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ATPSPlayer::LookUp);
	PlayerInputComponent->BindAxis("Horizontal", this, &ATPSPlayer::InputHorizontal);
	PlayerInputComponent->BindAxis("Vertical", this, &ATPSPlayer::InputVertical);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATPSPlayer::InputJump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATPSPlayer::InputFire);
	PlayerInputComponent->BindAction("GrenadeGun", IE_Pressed, this, &ATPSPlayer::ChangeToGrenadeGun);
	PlayerInputComponent->BindAction("SniperGun", IE_Pressed, this, &ATPSPlayer::ChangeToSniperGun);
	PlayerInputComponent->BindAction("Sniper", IE_Pressed, this, &ATPSPlayer::SniperAim);
	PlayerInputComponent->BindAction("Sniper", IE_Released, this, &ATPSPlayer::SniperAim);
}

void ATPSPlayer::Turn(float value)
{
	AddControllerYawInput(value);
}

void ATPSPlayer::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void ATPSPlayer::InputHorizontal(float value)
{
	direction.X = value;
}

void ATPSPlayer::InputVertical(float value)
{
	direction.Y = value;
}

void ATPSPlayer::InputJump()
{
	Jump();
}

void ATPSPlayer::Move()
{
	direction = FTransform(GetControlRotation()).TransformVector(direction);
	/*FVector P0 = GetActorLocation();
	FVector vt = direction * walkSpeed * DeltaTime;
	FVector P = P0 + vt;
	SetActorLocation(P);*/
	AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void ATPSPlayer::InputFire()
{
	// 총알 발사 처리
	FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
}

void ATPSPlayer::ChangeToGrenadeGun()
{
	// 유탄총 사용 중으로 체크
	bUsingGrenadeGun = true;
	sniperGunComp->SetVisibility(false);
	gunMeshComp->SetVisibility(true);
}

void ATPSPlayer::ChangeToSniperGun()
{
	bUsingGrenadeGun = false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);
}

void ATPSPlayer::SniperAim()
{
	if(bUsingGrenadeGun){
		return;
	}
	// Pressed 입력 처리
	if (bSniperAim == false) {
		// 1. 스나이퍼 조준 모드 활성화
		bSniperAim = true;
		// 2. 스나이퍼조준 UI 등록
		_sniperUI->AddToViewport();
		// 3. 카메라의 시야각 Field Of View 설정
		tpsCamComp->SetFieldOfView(45.0f);
	}
	else {
		// 1. 스나이퍼 조준 모드 비활성화
		bSniperAim = false;
		// 2. 스나이퍼 조준 UI 화면에서 제거
		_sniperUI->RemoveFromParent();
		// 3. 카메라 시야각 원래대로 복원
		tpsCamComp->SetFieldOfView(90.0f);
	}
}

