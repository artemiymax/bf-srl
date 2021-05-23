#pragma once

class IRecorder
{
public:
	virtual void Reset() = 0;
	
	virtual void Capture() = 0;
};
