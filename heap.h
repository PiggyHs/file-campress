#pragma once

#include<iostream>
using namespace std;
#include<vector>
#include"huffman.h"

template<class T>
struct Less
{
	bool operator()(const T & l, const T& n)
	{
		return l < n;
	}
};
template<class T>
struct Greater
{
	bool operator()(const T& l, const T& n)
	{
		return l > n;
	}
};
template<class T, class Comper>
class Heap
{
public:
	Heap()
		:heap(NULL)
	{}
	Heap(T* arr, size_t n)
	{
		heap.reserve(n);
		for (size_t i = 0; i < n; i++)
		{
			heap.push_back(arr[i]);
		}
		int parent = (heap.size() - 2) / 2;
		while (parent >= 0)
		{
			_AdjustDown(parent, heap.size() - 1);
			parent--;
		}
	}
	T Top()
	{
		return heap[0];
	}
	Heap(const vector<T>& x)
	{
		heap.clear();
		heap.resize(x.size(), 0);
		for (size_t i = 0; i < x.size(); i++)
		{
			heap[i] = x[i];
		}
		int parent = (heap.size() - 2) / 2;
		while (parent >= 0)
		{
			_AdjustDown(parent, heap.size() - 1);
			parent--;
		}
	}

	void push(const T& x)
	{
		heap.push_back(x);
		_AdjistUp(heap.size() - 1);
	}

	void Pop()
	{
		swap(heap[0], heap[heap.size() - 1]);
		heap.pop_back();
		_AdjustDown(0, heap.size() - 1);
	}
	bool Empty()
	{
		return heap.empty();
	}
	size_t Size()
	{
		return heap.size();
	}
	void Print()
	{
		vector<int>::iterator it = heap.begin();
		while (it != heap.end())
		{
			cout << *it << " ";
			it++;
		}
		cout << endl;
	}

protected:
	void _AdjustDown(int root, int size)//向下调整
	{
		Comper comper;
		int parent = root;
		int child = root * 2 + 1;
		while (child <= size)
		{
			if (child < size && comper(heap[child + 1], heap[child]))
			{
				child++;
			}
			if (comper(heap[child], heap[parent]))
			{
				swap(heap[parent], heap[child]);
				parent = child;
				child = child * 2 + 1;
			}
			else
			{
				break;
			}
		}
	}

	void _AdjistUp(int pos)
	{
		Comper comper;
		int child = pos;
		int parent = (child - 1) / 2;

		while (child > 0)
		{
			if (comper(heap[child], heap[parent]))
			{
				swap(heap[child], heap[parent]);
				child = parent;
				parent = (parent - 1) / 2;
			}
			else
				break;
		}
	}
private:
	vector<T> heap;
};