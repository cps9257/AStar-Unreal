// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AStarCharacter.generated.h"

UCLASS(Blueprintable)
class AAStarCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAStarCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

private:
	
	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;
};

