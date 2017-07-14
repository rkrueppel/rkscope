#pragma once

/** A synchronized, thread-safe queue
*	was modeled after ringbuffer example from boost?! and/or a Herb Sutter column?! */
template<class T>
class SynchronizedQueue {

private:
	/** disable copy */
	SynchronizedQueue(SynchronizedQueue<T>&);

	/** disable assignment */
	SynchronizedQueue<T> operator=(SynchronizedQueue<T>&);

protected:
	/** STL dequeue */
	std::deque<T> queue;

	/** mutex for protection */
	mutable std::mutex mut;

	/** condition variable for not empty notification */
	std::condition_variable not_empty;

public:
	/** We need a default constructor here */
	SynchronizedQueue() {
	}

	/** @return current queue size */
	size_t Size() const {
		std::lock_guard<std::mutex> lock(mut);
		return queue.size();
	}

	/** Clears the queue */
	void Clear() {
		std::lock_guard<std::mutex> lock(mut);
		queue.clear();
	}

	/** Enqueues an element and notifies one waiting operation that queue is not empty */
	void Enqueue(const T& elem) {
		{
		std::lock_guard<std::mutex> lock(mut);
		queue.push_back(elem);
		}
		not_empty.notify_one();
	}

	/** Enqueues an element by moving and notifies one waiting operation that queue is not empty */
	void Enqueue(T&& elem) {
		{
		std::lock_guard<std::mutex> lock(mut);
		queue.push_back(elem);
		}
		not_empty.notify_one();
	}

	/** Dequeues front element, waits indefinitely if queue is empty */
	T Dequeue() {
		std::unique_lock<std::mutex> lock(mut);
		while ( queue.empty() )
			not_empty.wait(lock);
		T tmp(queue.front());
		queue.pop_front();
		return tmp;
	}

	/** Dequeues front element, waits timeout if queue is empty */
	T Dequeue(const std::chrono::milliseconds& timeout) {
		std::unique_lock<std::mutex> lock(mut);
		while ( queue.empty() )
			not_empty.wait_for(lock, timeout);
		T tmp(queue.front());
		queue.pop_front();
		return tmp;
	}
};


