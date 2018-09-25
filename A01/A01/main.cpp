// main.cpp : Templated Queue Demonstration
// Zack Dunham, IGME309-02

#include <iostream>
#include "queue.h"
using namespace std;

int main()
{
	// test queue #1
	TQueue<int> queue;

	cout << "Default Constructor Queue: " << endl;
	cout << "IsEmpty() before adding anything: " << queue.IsEmpty() << endl;
	cout << endl;

	// populate the queue, print, and show results of size and isempty
	queue.Push(4);
	queue.Push(7);
	queue.Push(14);
	queue.Push(9);

	cout << "Print(): ";
	queue.Print();
	cout << endl;

	cout << "GetSize(): " << queue.GetSize() << endl;
	cout << "IsEmpty(): " << queue.IsEmpty() << endl;
	cout << endl;

	// call pop and show results
	cout << "Print() after calling Pop(): ";
	queue.Pop();
	queue.Print();
	cout << endl;

	cout << "GetSize(): " << queue.GetSize() << endl;
	cout << "IsEmpty(): " << queue.IsEmpty() << endl;
	cout << endl;

	// copy constructor test
	TQueue<int> cpyQueue(queue);
	cout << "Copy Constructor Queue: " << endl;
	cout << "Print(): ";
	cpyQueue.Print();
	cout << endl;

	cout << "GetSize(): " << cpyQueue.GetSize() << endl;
	cout << "IsEmpty(): " << cpyQueue.IsEmpty() << endl;
	cout << endl;

	// copy assignment operator test
	TQueue<int> cpyOpQueue = queue;
	cout << "Copy Assigment Operator Queue: " << endl;
	cout << "Print(): ";
	cpyOpQueue.Print();
	cout << endl;

	cout << "GetSize(): " << cpyOpQueue.GetSize() << endl;
	cout << "IsEmpty(): " << cpyOpQueue.IsEmpty() << endl;
	cout << endl;

	cout << "Press Enter to exit. ";
	getchar();
	return 0;
}