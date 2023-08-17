// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include "AStarPlayerController.h"
#include "AStarCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GridNode.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

using namespace std;

UCLASS()
class ASTAR_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

	UPROPERTY(EditAnywhere)
		FIntPoint mapSize;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> gridNode;

	UPROPERTY(EditAnywhere)
		FIntPoint playerStartCoordinate;

	UPROPERTY(EditAnywhere)
		FIntPoint enemyStartCoordinate;

	UPROPERTY(EditAnywhere)
		TArray<FIntPoint> obstacles;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ACharacter> playerMaster;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ACharacter> enemyMaster;

	UPROPERTY(EditAnywhere)
		AActor* camera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float const GRID_SIZE_Z = 200.0f;
	float const NODE_POSITION_Z = 266.0f;

	map<int, AGridNode*> _map;
	FIntPoint _mapSize;
	AAStarPlayerController *AStarPlayerController;

	map<int, AGridNode*> CreateMap(FIntPoint mapSize);
	void ClearMap(map<int, AGridNode*> map);
	/*void CreatePlayerMaster(FIntPoint initCoordinate);
	void CreateEnemyMaster(FIntPoint initCoordinate);*/

	void FindPath(FIntPoint startPos, FIntPoint targetPos);
	vector<AGridNode*> ReconstructPath(AGridNode* targetNode);
	vector<AGridNode*> FindPath(map<int, AGridNode*> map, FIntPoint startPos, FIntPoint targetPos);

	//Helper
	int CoordinateToIndex(FIntPoint coordinate, FIntPoint mapSize);
	FIntPoint IndexToCoordinate(int index, FIntPoint mapSize);
	int Distance(FIntPoint source, FIntPoint target);
	bool IsObstacle(FIntPoint source);
};
