// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "HAL/Runnable.h"
#include "CoreMinimal.h"

/**
 * 
 */
DECLARE_DELEGATE(FCallbackDelegate);

class MULTITHREAD_API AsyncQueue : public FRunnable
{
private:
	/** Thread to run the worker FRunnable on */
	FRunnableThread* Thread;

	//Counter thread safe used to can stop thread execution
	FThreadSafeCounter StopTaskCounter;

	FCriticalSection QueueRequestsCriticalSection;
	TQueue<FCallbackDelegate> QueueRequests;
	uint32 debugQueueCounter = 0;
	
public:
	AsyncQueue(/*customizable*/);
	~AsyncQueue();

	static TSharedPtr<AsyncQueue> CreateAsSharedPtr();

	// Begin FRunnable interface.
	virtual bool Init() override;
	virtual uint32 Run() override;

	virtual void Stop() override;

	//Stop the thread and wait until it's really stopped
	void EnsureCompletion();

	void AddASync(const FCallbackDelegate& SyncDelegate);
	void ResetAll();

};
