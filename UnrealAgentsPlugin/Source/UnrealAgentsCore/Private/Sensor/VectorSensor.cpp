#include "Sensor/VectorSensor.h"

FVectorSensor::FVectorSensor(const uint32 ObservationSize) : ISensor(FObservationSpace({ ObservationSize }, ESpaceType::Continuous))
{
	Observations.Reserve(ObservationSize);
}

void FVectorSensor::AddObservation(const float Observation)
{
	Observations.Add(Observation);
}

void FVectorSensor::AddObservation(const FVector2D Observation)
{
	Observations.Add(Observation.X);
	Observations.Add(Observation.Y);
}

void FVectorSensor::AddObservation(const FVector Observation)
{
	Observations.Add(Observation.X);
	Observations.Add(Observation.Y);
	Observations.Add(Observation.Z);
}

void FVectorSensor::AddObservation(const FVector4 Observation)
{
	Observations.Add(Observation.X);
	Observations.Add(Observation.Y);
	Observations.Add(Observation.Z);
	Observations.Add(Observation.W);
}

void FVectorSensor::AddObservation(const TArray<float> Observation)
{
	for (float Item : Observation)
	{
		Observations.Add(Item);
	}
}

void FVectorSensor::Read(TArray<float>& Reader)
{
	if (Observations.Num() > ObservationSpace.Size)
	{
		Observations.RemoveAt(0, Observations.Num() - ObservationSpace.Size);
	}
	else if (Observations.Num() < ObservationSpace.Size)
	{
		Observations.AddDefaulted(ObservationSpace.Size - Observations.Num());
	}

	for (float& Observation : Observations)
	{
		Reader.Add(Observation);
	}
}

void FVectorSensor::Reset()
{
	Observations.Reset();
}
