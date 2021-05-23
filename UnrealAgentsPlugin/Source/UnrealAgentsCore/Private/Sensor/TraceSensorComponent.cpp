#include "Sensor/TraceSensorComponent.h"

#include "EngineUtils.h"

void UTraceSensorComponent::InitializeSensor()
{
	if (Sensor)
	{
		return;
	}

	Sensor = new FTraceSensor(StepBuffer, TraceLength, TraceRadius, TraceChannel, TraceDrawType, PerceptionDegree, PerceptionRadius, PerceptionTags, GetOwner());
}

void UTraceSensorComponent::Reset()
{
	Sensor->Reset();
}
