// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncQueue.h"

AsyncQueue::AsyncQueue()
{
	Thread = FRunnableThread::Create(this, TEXT("AsyncQueueThread"), 0, TPri_Normal); //windows default = 8mb for thread, could specify more
}

AsyncQueue::~AsyncQueue()
{
	delete Thread;
	Thread = NULL;
}

TSharedPtr<AsyncQueue> AsyncQueue::CreateAsSharedPtr()
{
	return MakeShareable(new AsyncQueue());

	//TSharedPtr<FAsyncQueue> Result(new FAsyncQueue());
	//return Result;
}

bool AsyncQueue::Init()
{
	StopTaskCounter.Set(0);
	return true;
}

uint32 AsyncQueue::Run()
{
	//Do not stop even if requested if some task is waiting
	while (StopTaskCounter.GetValue() == 0 || QueueRequests.IsEmpty() == false)
	{
		while (QueueRequests.IsEmpty() == false)
		{
			FCallbackDelegate CurrentDelegate;
			{
				FScopeLock Lock(&QueueRequestsCriticalSection);

				QueueRequests.Dequeue(CurrentDelegate);
				--debugQueueCounter;
			}

			CurrentDelegate.Execute();
		}

		//There's currently no tasks so wait some time to don't waste cpu time
		FPlatformProcess::Sleep(0.0001);
	}

	//returning 0 will stop the thread
	return 0;
}

void AsyncQueue::Stop()
{
	StopTaskCounter.Set(1);
}

void AsyncQueue::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

void AsyncQueue::AddASync(const FCallbackDelegate& SyncDelegate)
{
	{
		FScopeLock Lock(&QueueRequestsCriticalSection);

		QueueRequests.Enqueue(SyncDelegate);
		++debugQueueCounter;
	}
}

void AsyncQueue::ResetAll()
{
	{
		FScopeLock Lock(&QueueRequestsCriticalSection);

		QueueRequests.Empty();
		debugQueueCounter = 0;
	}
}
