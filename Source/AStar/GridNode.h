// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridNode.generated.h"

using namespace std;


enum GridNodeState {
	None, Start, Through, Target, Obstacle
};

UCLASS()
class ASTAR_API AGridNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridNode();

	UPROPERTY(EditAnywhere)
		FVector2D GridSize;

	FIntPoint Coordinate;
	int GScore; //Cost from source node to node
	int HScore; //Estimated cost from node to target node
	int FScore() { return GScore + HScore; }; //Total cost from source to target node
	AGridNode *Parent;

	void Initialize(FIntPoint coordinate, bool isObstacle);
	void Reset();
	void Render(GridNodeState state);

	//Debug
	FString ToString();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UStaticMeshComponent* mesh;
	UMaterialInterface *stashMaterial;
	bool _isObstacle;
	void ChangeMaterialColor(FLinearColor color);
};
