// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AStarPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "AStarCharacter.h"
#include "Engine/World.h"

AAStarPlayerController::AAStarPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AAStarPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	if (currentPath.empty())
		return;

	if (currentIndex < currentPath.size())
	{
		auto targetPos = currentPath[currentIndex]->GetActorLocation();
		SetNewMoveDestination(targetPos);

		APawn* const MyPawn = GetPawn();
		if (MyPawn)
		{
			float const Distance = FVector::Dist(targetPos, MyPawn->GetActorLocation());

			// We need to issue move command only if far enough in order for walk animation to play correctly
			if ((Distance <= MINIMUM_DISTANCE))
			{
				currentIndex++;
			}
		}
	}
}

void AAStarPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AAStarPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AAStarPlayerController::OnSetDestinationReleased);
}

void AAStarPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > MINIMUM_DISTANCE))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void AAStarPlayerController::OnSetDestinationPressed()
{
	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		auto hitActor = Hit.GetActor();
		if (hitActor != nullptr) 
		{
			if (AGridNode *node = Cast<AGridNode>(hitActor)) 
			{
				if (currentPath.size() == 0)
					currentNode = nullptr;
				else
					currentNode = currentPath[currentIndex-1];

				targetNode = node;
			}
			else 
			{
				targetNode = nullptr;
			}
		}
	}
}

void AAStarPlayerController::OnSetDestinationReleased()
{
	if (currentNode != nullptr && targetNode != nullptr) {
		OnMouseClicked.ExecuteIfBound(currentNode->Coordinate, targetNode->Coordinate);
	}
}

void AAStarPlayerController::UpdatePath(vector<AGridNode *> path)
{
	currentPath.clear();

	for (int i = 0; i < path.size(); i++)
		currentPath.push_back(path[i]);

	currentIndex = 0;
}

