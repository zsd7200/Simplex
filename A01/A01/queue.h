// queue.h : Templated Queue (TQueue)
// Zack Dunham, IGME309-02

#pragma once
#include <iostream>
using namespace std;

template <class T>
class TQueue
{
public:
	TQueue();
	TQueue(const TQueue &que);
	TQueue<T>& operator= (const TQueue& other);
	~TQueue();

	void Push(T var);
	void Pop();
	void Print();
	int GetSize();
	bool IsEmpty();

private:
	int size;
	T* queue;
	void Resize();
};

template <class T>
TQueue<T>::TQueue()
{
	size = 0;
	queue = new T[1];
}

template <class T>
TQueue<T>::TQueue(const TQueue& que)
{
	// set size of new queue
	size = que.size;
	queue = new T[size];

	// copy values over
	for (size_t i = 0; i < size; i++)
		queue[i] = que.queue[i];
}

template<class T>
inline TQueue<T>& TQueue<T>::operator=(const TQueue & other)
{
	if (this != &other)
	{
		// set size of new queue
		size = other.size;
		queue = new T[size];

		// copy values over
		for (size_t i = 0; i < size; i++)
			queue[i] = other.queue[i];
	}
	return *this;
}

template <class T>
void TQueue<T>::Push(T var)
{
	// add variable to queue, increase size, and resize queue
	queue[size] = var;
	size++;
	Resize();
}

template <class T>
void TQueue<T>::Pop()
{
	// create a temporary array
	T* temp = new T[size - 1];

	// fill it with queue values and delete queue
	for (size_t i = 0; i < size; i++)
		if (i + 1 < size)
			temp[i] = queue[i + 1];
	delete[] queue;

	// reinitialize queue
	queue = new T[size - 1];

	// fill with temp values and delete temp
	for (size_t i = 0; i < size; i++)
		queue[i] = temp[i];
	delete[] temp;

	// decrement size int
	size--;
}

template <class T>
void TQueue<T>::Print()
{
	// print queue contents
	for (size_t i = 0; i < size; i++)
		cout << queue[i] << " ";
}

template <class T>
int TQueue<T>::GetSize()
{
	// returning just the size int would show the size of the queue, not the amount of entries

	int tempSize = 0;

	// checking for nulls would give the amount of entries rather than the size
	for (size_t i = 0; i < size; i++)
		if (queue[i] != NULL)
			tempSize++;

	return tempSize;
}

template <class T>
bool TQueue<T>::IsEmpty()
{
	if (size == 0)
		return true;
	else
		return false;
}

// resize helper method for pushing new items to queue
template <class T>
void TQueue<T>::Resize()
{
	// create a temporary array
	T* temp = new T[size];

	// fill it with queue values and delete queue
	for (size_t i = 0; i < size; i++)
		temp[i] = queue[i];
	delete[] queue;

	// reinitialize queue
	queue = new T[size + 1];

	// fill with temp values and delete temp
	for (size_t i = 0; i < size; i++)
		queue[i] = temp[i];
	delete[] temp;
}

template <class T>
TQueue<T>::~TQueue()
{
	delete[] queue;
}