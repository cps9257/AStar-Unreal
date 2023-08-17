// Fill out your copyright notice in the Description page of Project Settings.

#include "GridNode.h"


// Sets default values
AGridNode::AGridNode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGridNode::BeginPlay()
{
	Super::BeginPlay();

	mesh = FindComponentByClass<UStaticMeshComponent>();
	if (mesh != nullptr) {
		stashMaterial = mesh->GetMaterial(0);
	}
}

// Called every frame
void AGridNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGridNode::Initialize(FIntPoint coordinate, bool isObstacle) {
	Coordinate = coordinate;
	Reset();
	_isObstacle = isObstacle;
}

void AGridNode::Reset() {
	GScore = INT_MAX / 2;
	HScore = INT_MAX / 2;
	Parent = nullptr;

	if (_isObstacle)
		return;

	ChangeMaterialColor(FLinearColor::Blue);
}

void AGridNode::Render(GridNodeState state)
{
	switch (state) {
	case None:
		ChangeMaterialColor(FLinearColor::Blue);
		break;
	case Start:
		ChangeMaterialColor(FLinearColor::Green);
		break;
	case Through:
		ChangeMaterialColor(FLinearColor::White);
		break;
	case Target:
		ChangeMaterialColor(FLinearColor::Red);
		break;
	case Obstacle:
		ChangeMaterialColor(FLinearColor::Black);
		break;
	default:
		ChangeMaterialColor(FLinearColor::Blue);
		break;
	}
}

void AGridNode::ChangeMaterialColor(FLinearColor color)
{
	if (mesh != nullptr)
	{
		UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(stashMaterial, NULL);

		material->SetVectorParameterValue(FName(TEXT("Color")), color);
		mesh->SetMaterial(0, material);
	}
}

FString AGridNode::ToString() {
	return FString::Printf(TEXT("Coordinate: {%d,%d}, GScore: %d, HScore: %d, FScore:  %d"), Coordinate.X, Coordinate.Y, GScore, HScore, FScore());
}
