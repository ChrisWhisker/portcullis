// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DungeonGenerator.h"

struct FRect
{
	int X, Y, Width, Height;
};

/**
 * 
 */
class PORTCULLIS_API BSPGenerator : DungeonGenerator
{
public:
	virtual void Generate() override;

private:
	static void DivideSpace(std::vector<FRect>& Spaces, int Depth, int MaxDepth);
};
