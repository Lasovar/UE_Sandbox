#pragma once

#include "CoreMinimal.h"

struct FTimer
{
	float Duration = 0.f;
	float Elapsed = 0.f;
	bool bActive = false;

	FTimer() = default;
	explicit FTimer(float InDuration) : Duration(InDuration) {}

	void Start()
	{
		bActive = true;
	}

	void Restart() { Elapsed = 0.f; bActive = true; }
	void Stop() { bActive = false; }

	bool IsActive() const { return bActive; }
	bool IsElapsed() const { return Elapsed >= Duration; }

	void Tick(float DeltaTime)
	{
		if (bActive && !IsElapsed())
		{
			Elapsed += DeltaTime;
		}
	}

	float GetProgress() const { return Duration > 0 ? Elapsed / Duration : 1.f; }
};
