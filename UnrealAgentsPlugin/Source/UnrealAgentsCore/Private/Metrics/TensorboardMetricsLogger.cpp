#include "TensorboardMetricsLogger.h"

THIRD_PARTY_INCLUDES_START
#pragma push_macro("check")
#undef check
#pragma warning (push)
#pragma warning (disable : 4191)
#pragma warning (disable : 4686)
#include "pybind11/pybind11.h"
#include "pybind11/embed.h"
#pragma warning (pop)
#pragma pop_macro("check")
THIRD_PARTY_INCLUDES_END

FTensorboardMetricsLogger::FTensorboardMetricsLogger()
{
	Processor = new FMetricsProcessor(Summary, Writer, ScalarQueue, TextQueue);
	
	if (CreateWriter())
	{
		if (!Processor->StartProcessor())
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create tensorboard processor thread."));
		}
	}
}

void FTensorboardMetricsLogger::LogScalar(const FString& Name, const TTuple<float, uint64>& Value)
{
	if (!Processor->IsRunning())
	{
		return;
	}

	ScalarQueue.Enqueue(TTuple<FString, float, uint64>(Name, Value.Key, Value.Value));
}

void FTensorboardMetricsLogger::LogScalar(const FString& Name, const TArray<TTuple<float, uint64>>& Values)
{
	if (!Processor->IsRunning())
	{
		return;
	}

	for (auto& Value : Values)
	{
		ScalarQueue.Enqueue(TTuple<FString, float, uint64>(Name, Value.Key, Value.Value));
	}
}

void FTensorboardMetricsLogger::LogString(const FString& Name, const TTuple<FString, uint64>& Value)
{
	if (!Processor->IsRunning())
	{
		return;
	}

	TextQueue.Enqueue(TTuple<FString, FString, uint64>(Name, Value.Key, Value.Value));
}

void FTensorboardMetricsLogger::LogString(const FString& Name, const TArray<TTuple<FString, uint64>>& Values)
{
	if (!Processor->IsRunning())
	{
		return;
	}
	
	for (auto& Value : Values)
	{
		TextQueue.Enqueue(TTuple<FString, FString, uint64>(Name, Value.Key, Value.Value));
	}
}

pybind11::module& FTensorboardMetricsLogger::GetTensorflowModule()
{
	static pybind11::scoped_interpreter InterpreterGuard;
	static pybind11::module TensorflowModule = pybind11::module::import("tensorflow");

	return TensorflowModule;
}

void FTensorboardMetricsLogger::Reset()
{
	Processor->StopProcessor();

	ScalarQueue.Empty();
	TextQueue.Empty();

	CreateWriter();

	Processor->StartProcessor();
}

bool FTensorboardMetricsLogger::CreateWriter()
{
	RunName = FString("Run_") + FString::FromInt(FDateTime::Now().ToUnixTimestamp());

	FString MetricsDirectory = FPaths::Combine(FPaths::ProjectDir(), TEXT("UnrealAgents"), TEXT("Metrics"), TEXT("Tensorboard"), RunName);
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (PlatformFile.CreateDirectoryTree(*MetricsDirectory))
	{
		if (GilReleaseGuard)
		{
			delete GilReleaseGuard;
		}

		Summary = GetTensorflowModule().attr("summary");
		Writer = Summary.attr("create_file_writer")(*MetricsDirectory, 10);

		GilReleaseGuard = new pybind11::gil_scoped_release(true);
		return true;
	}

	UE_LOG(LogPath, Error, TEXT("Failed to create directory for Tensorboard metrics writer."));
	return false;
}

FTensorboardMetricsLogger::~FTensorboardMetricsLogger()
{
	Processor->StopProcessor();
	delete Processor;

	if (GilReleaseGuard)
	{
		delete GilReleaseGuard;
	}

	Writer.attr("close")();
	Summary.release();
	Writer.release();
}

FMetricsProcessor::FMetricsProcessor
(
	pybind11::object& TargetSummary, 
	pybind11::object& TargetWriter, 
	TQueue<TTuple<FString, float, uint64>, EQueueMode::Spsc>& ScalarMetricsQueue, 
	TQueue<TTuple<FString, FString, uint64>, EQueueMode::Spsc>& TextMetricsQueue
)
{
	Summary = &TargetSummary;
	Writer = &TargetWriter;
	ScalarQueue = &ScalarMetricsQueue;
	TextQueue = &TextMetricsQueue;
}

bool FMetricsProcessor::StartProcessor()
{
	if (IsRunning())
	{
		return true;
	}

	ProcessingThread = FRunnableThread::Create(this, TEXT("Tensorboard processor thread"));
	return IsRunning();
}

void FMetricsProcessor::StopProcessor()
{
	if (IsRunning())
	{
		ProcessingThread->Kill(true);
	}

	delete ProcessingThread;
	ProcessingThread = nullptr;
}

bool FMetricsProcessor::Init()
{
	bShouldRun = true;
	return bShouldRun;
}

uint32 FMetricsProcessor::Run()
{
	pybind11::gil_scoped_acquire Acquire;

	pybind11::object WriterContext = Writer->attr("as_default")();
	WriterContext.attr("__enter__")();

	TTuple<FString, float, uint64> ScalarMetric;
	TTuple<FString, FString, uint64> TextMetric;
	while (bShouldRun)
	{	
		if (ScalarQueue && ScalarQueue->Dequeue(ScalarMetric))
		{
			Summary->attr("scalar")(TCHAR_TO_UTF8(*ScalarMetric.Get<0>()), ScalarMetric.Get<1>(), ScalarMetric.Get<2>());
		}

		if (TextQueue && TextQueue->Dequeue(TextMetric))
		{
			Summary->attr("text")(TCHAR_TO_UTF8(*TextMetric.Get<0>()), TCHAR_TO_UTF8(*TextMetric.Get<1>()), TextMetric.Get<2>());
		}

		if (ScalarQueue->IsEmpty() && TextQueue->IsEmpty())
		{
			FPlatformProcess::Sleep(0.5f);
		}
	}

	WriterContext.attr("__exit__")(nullptr, nullptr, nullptr);
	return 0;
}

void FMetricsProcessor::Stop()
{
	bShouldRun = false;
}

FMetricsProcessor::~FMetricsProcessor()
{
}
