#pragma once

#include "CoreMinimal.h"

struct PORTCULLIS_API FRect
{
public:
	int32 X, Y, Width, Height;

	FRect(const int32 InX, const int32 InY, const int32 InWidth, const int32 InHeight)
		: X(InX), Y(InY), Width(InWidth), Height(InHeight)
	{
	}
};
