// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMove.h"

UPlayerMove::UPlayerMove() {
	// Tick함수가 호출되도록 처리
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerMove::BeginPlay()
{
	Super::BeginPlay();

	// 초기 속도를 걷기로 지정
	moveComp->MaxWalkSpeed = walkSpeed;
}

void UPlayerMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Move();
}

void UPlayerMove::Turn(float value)
{
	me->AddControllerYawInput(value);
}

void UPlayerMove::LookUp(float value)
{
	me->AddControllerPitchInput(value);
}

void UPlayerMove::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &UPlayerMove::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &UPlayerMove::LookUp);
	PlayerInputComponent->BindAxis("Horizontal", this, &UPlayerMove::InputHorizontal);
	PlayerInputComponent->BindAxis("Vertical", this, &UPlayerMove::InputVertical);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &UPlayerMove::InputJump);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &UPlayerMove::InputRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &UPlayerMove::InputRun);
}

void UPlayerMove::InputHorizontal(float value)
{
	direction.X = value;
}

void UPlayerMove::InputVertical(float value)
{
	direction.Y = value;
}

void UPlayerMove::InputJump()
{
	me->Jump();
}

void UPlayerMove::Move()
{
	direction = FTransform(me->GetControlRotation()).TransformVector(direction);
	/*FVector P0 = GetActorLocation();
	FVector vt = direction * walkSpeed * DeltaTime;
	FVector P = P0 + vt;
	SetActorLocation(P);*/
	me->AddMovementInput(direction);
	direction = FVector::ZeroVector;
}


void UPlayerMove::InputRun()
{
	auto movement = moveComp;
	// 현재 달리기 모드라면
	if (movement->MaxWalkSpeed > walkSpeed) {
		movement->MaxWalkSpeed = walkSpeed;
	}
	else {
		movement->MaxWalkSpeed = runSpeed;
	}
}
