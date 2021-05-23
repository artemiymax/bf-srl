#include "Metrics/MovingAverageAccumulator.h"

FMovingAverageAccumulator::FMovingAverageAccumulator(const uint32 Period)
{
	WindowPeriod = Period;
	WindowIndex = 0;
	Sum = 0.f;

	Window.Reserve(Period);
	Window.Init(0.f, Period);
}

void FMovingAverageAccumulator::Update(const float Value)
{
	Sum = Sum - Window[WindowIndex] + Value;

	Window[WindowIndex] = Value;
	WindowIndex = (WindowIndex + 1) % WindowPeriod;
}

void FMovingAverageAccumulator::Reset()
{
	Sum = 0.f;
	WindowIndex = 0;

	Window.Reset();
	Window.Init(0.f, WindowPeriod);
}

float FMovingAverageAccumulator::GetResult()
{
	return Sum / WindowPeriod;
}
