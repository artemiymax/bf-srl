#pragma once

#include "CoreMinimal.h"
#include "Metrics/IMetricsLogger.h"

THIRD_PARTY_INCLUDES_START
#pragma push_macro("check")
#undef check
#pragma warning (push)
#pragma warning (disable : 4191)
#pragma warning (disable : 4686)
#include "pybind11/pybind11.h"
#pragma warning (pop)
#pragma pop_macro("check")
THIRD_PARTY_INCLUDES_END

class FMetricsProcessor : public FRunnable
{
public:	
	FMetricsProcessor
	(
		pybind11::object& TargetSummary, 
		pybind11::object& TargetWriter, 
		TQueue<TTuple<FString, float, uint64>, EQueueMode::Spsc>& ScalarMetricsQueue, 
		TQueue<TTuple<FString, FString, uint64>, EQueueMode::Spsc>& TextMetricsQueue
	);

	bool StartProcessor();

	void StopProcessor();

	bool IsRunning() const { return ProcessingThread != nullptr; };
	
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;

	virtual ~FMetricsProcessor() override;

private:
	pybind11::object* Summary = nullptr;
	
	pybind11::object* Writer = nullptr;
	
	bool bShouldRun = false;

	FRunnableThread* ProcessingThread = nullptr;
	
	TQueue<TTuple<FString, float, uint64>, EQueueMode::Spsc>* ScalarQueue = nullptr;

	TQueue<TTuple<FString, FString, uint64>, EQueueMode::Spsc>* TextQueue = nullptr;
};

class FTensorboardMetricsLogger: public IMetricsLogger
{
public:
	FString RunName;
	
	FTensorboardMetricsLogger();

	virtual void LogScalar(const FString& Name, const TTuple<float, uint64>& Value) override;

	virtual void LogScalar(const FString& Name, const TArray<TTuple<float, uint64>>& Values) override;

	virtual void LogString(const FString& Name, const TTuple<FString, uint64>& Value) override;

	virtual void LogString(const FString& Name, const TArray<TTuple<FString, uint64>>& Values) override;

	void Reset();

	virtual ~FTensorboardMetricsLogger() override;

private:
	pybind11::gil_scoped_release* GilReleaseGuard = nullptr;
	
	pybind11::object Writer;
	
	pybind11::object Summary;

	static pybind11::module& GetTensorflowModule();

	TQueue<TTuple<FString, float, uint64>, EQueueMode::Spsc> ScalarQueue;

	TQueue<TTuple<FString, FString, uint64>, EQueueMode::Spsc> TextQueue;
	
	FMetricsProcessor* Processor;
	
	bool CreateWriter();
};
