#pragma once

#include <vector>
#include "CoreMinimal.h"
#include "DungeonGenerator.h"
#include "Rect.h"
#include "Engine/World.h"

/**
 *
 */
class PORTCULLIS_API FBSPGenerator final : public FDungeonGenerator
{
public:
	virtual void Initialize(UWorld* InWorld) override;
	virtual void Generate() override;

private:
	std::vector<FRect> Rooms;
	static void DivideSpace(std::vector<FRect>& SubSpaces, int CurrentDepth, int MaxDepth);
	void CreateRooms(const std::vector<FRect>& SubSpaces);
};
