#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"

/**
 * Interface for dungeon generation algorithms
 */
class PORTCULLIS_API FDungeonGenerator
{
public:
	virtual ~FDungeonGenerator() = default;
	virtual void Initialize(UWorld* InWorld);
	virtual void Generate() = 0;

protected:
	UWorld* World = nullptr;
};
