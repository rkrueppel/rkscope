#pragma once

#include "SyncQueues.h"
#include "helpers.h"

/** An active object implementation.
* I got most of the stuff from Herb Sutters column in Dr. Dobbs: http://www.drdobbs.com/parallel/prefer-using-active-objects-instead-of-n/225700095 .\n
* I did some pimping with packaged tasks and futures...
* @ingroup HELPERS */
template<class RT>
class Active {

public:
	/** Type of the worker function to be executed in Active's thread.
	* It should monitor the StopCondition and return an RT if the StopCondition is true. */
	typedef std::function<RT(StopCondition* const sc)> Command;

protected:
	/** the abort signal */
	StopCondition stop;

	/** this is set to quit the worker thread */
	bool done;

	/** synchronized queue of worker functions/packaged tasks that are executed in the Active' thread (see lambda in constructor). */
	SynchronizedQueue<const std::function<void(StopCondition* const sc)>> ptq;

	/** a thread in which Run runs */
	std::thread thread;

protected:
	/** disable copy */
	Active(const Active&);

	/** disable assignment */
	void operator=(const Active&);

protected:

public:
	/** The thread execution loop is created from a lambda */
	Active()
		: stop()
		, done(false)
		, ptq()
		, thread([=]() {while ( !done ) {
			stop.Set(false);
			ptq.Dequeue()(&stop); }
			DBOUT(L"Active::thread ended"); }) {
	}

	~Active() {
		Quit();
	}

	/** Sends a worker function/packaged task to the queue to be executed in the Active's thread
	* @param[in] _cmd The worker function/packaged task to be queued 
	* @return the future for the return value of the worker function */
	std::future<RT> Send(const Command& _cmd) {
		// make a shared_ptr to the promise (we need this since promise it non-copyable
		auto p = std::make_shared<std::promise<RT>>();

		// get the future of the promise to return it at the end
		std::future<RT> ret = std::future<RT>(std::move(p->get_future()));

		// the promise pointer is caught by the lambda, thus outlives the Send function
		ptq.Enqueue([=](StopCondition* const sc) {
			p->set_value(_cmd(sc));
		});

		return ret;
	}

	/** Aborts the currently executed worker function */
	void AbortCurrent() {
		stop.Set(true);
	}

	/** Send lambda with done=true to the worker, join the worker thread, and clear the packaged task queue */
	void Quit() {
		ptq.Enqueue( [=] (StopCondition* const sc)	{ done = true; DBOUT(L"Active::Quit enqueued\n");} );
		if ( thread.joinable() )	// This is not unfallably safe, could get unjoinable right after if?!
			thread.join();
		DBOUT(L"Active::Quitted\n");
	}
};

