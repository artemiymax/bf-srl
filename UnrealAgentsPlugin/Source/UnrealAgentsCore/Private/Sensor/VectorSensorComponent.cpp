#include "Sensor/VectorSensorComponent.h"

void UVectorSensorComponent::InitializeSensor()
{
	Sensor = new FVectorSensor(ObservationSize);
}

void UVectorSensorComponent::Reset()
{
	Sensor->Reset();
}
