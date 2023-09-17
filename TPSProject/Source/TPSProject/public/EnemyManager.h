// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class TPSPROJECT_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 랜덤 시간 간격 최소값
	UPROPERTY(EditAnywhere, Category = SpawnSetting)
		float minTime = 1;
	// 랜덤 시간 간격 최대값
	UPROPERTY(EditAnywhere, Category = SpawnSetting)
		float maxTime = 5;
	// 스폰할 위치 정보 배열
	UPROPERTY(EditAnywhere, Category = SpawnSetting)
		TArray<class AActor*> spawnPoints;
	// AEnemy 타입의 블루 프린트 할당받을 변수
	UPROPERTY(EditAnywhere, Category = SpawnSetting)
		TSubclassOf<class AEnemy> enemyFactory;

	// 스폰을 위한 알람 타이머
	FTimerHandle spawnTimerHandle;
	// 적 생성 함수
	void CreateEnemy();

	// 스폰할 위치를 동적 찾아 할당하기
	void FindSpawnPoints();
};
