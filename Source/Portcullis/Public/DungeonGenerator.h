// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FRect
{
	int X, Y, Width, Height;
};

/**
 * 
 */
class PORTCULLIS_API DungeonGenerator
{
public:
	virtual ~DungeonGenerator();

	virtual void Generate() = 0;

protected:
	UWorld* World = nullptr;

	void DrawDebugRect(const FRect& Rect) const;

	void DrawLine(const FVector& Start, const FVector& End, const FColor Color) const;
};
