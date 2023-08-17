// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include <vector>
#include "GridNode.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AStarPlayerController.generated.h"

using namespace std;

DECLARE_DELEGATE_TwoParams(OnMouseClickedDelegate, FIntPoint, FIntPoint);

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTestDelegate);

UCLASS()
class AAStarPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAStarPlayerController();
	void UpdatePath(vector<AGridNode *> path);
	OnMouseClickedDelegate OnMouseClicked;

protected:

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

private:
	vector<AGridNode *> currentPath;
	int currentIndex;
	AGridNode *currentNode;
	AGridNode *targetNode;
	float const MINIMUM_DISTANCE = 120.0f;
};


