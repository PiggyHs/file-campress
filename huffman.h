#pragma once
#include"heap.h"

template<class T>
struct HuffmanNode
{
	T _weight;
	HuffmanNode<T>* _left;
	HuffmanNode<T>* _right;
	HuffmanNode<T>(const T& w)
		: _weight(w)
		, _left(NULL)
		, _right(NULL)
	{}
};

template<class T>
class HuffmanTree
{
	typedef HuffmanNode<T> Node;
public:
	HuffmanTree();
	~HuffmanTree();
	HuffmanTree(const T* array, size_t size, const T& vailed);
	Node* GethuffmanNode();
protected:
	static Node* _HuffmanTree(const T* array, size_t size, const T& vailed);
	static void _Clear(Node* root);
protected:
	Node* _root;
};


template<class T>
HuffmanTree<T>::HuffmanTree() :_root(NULL)
{}

template<class T>
HuffmanTree<T>::HuffmanTree(const T* array, size_t size, const T& vailed) : _root(NULL)
{
	this->_root = _HuffmanTree(array, size, vailed);
}

template<class T>
HuffmanNode<T>* HuffmanTree<T>::_HuffmanTree(const T* array, size_t size, const T& vailed)
{
	struct Compare
	{
		bool operator()(const HuffmanNode<T>* l, const HuffmanNode<T>* r) const
		{
			return l->_weight < r->_weight;
		}
	};
	Heap<Node*, Compare> heap;
	for (size_t i = 0; i < size; i++)
	{
		if (array[i] != vailed)
		{
			heap.push(new Node(array[i]));
		}
	}

	Node* parent = NULL;
	while (heap.Size() > 1)
	{
		Node* left = heap.Top();
		heap.Pop();
		Node* right = heap.Top();
		heap.Pop();
		parent = new Node(left->_weight + right->_weight);
		parent->_left = left;
		parent->_right = right;
		heap.push(parent);
	}
	return parent;
}
template<class T>
void HuffmanTree<T>::_Clear(Node* root)
{
	if (root)
	{
		_Clear(root->_left);
		_Clear(root->_right);
		delete root;
	}
}
template<class T>
HuffmanTree<T>::~HuffmanTree()
{
	_Clear(_root);
}
template<class T>
HuffmanNode<T>* HuffmanTree<T>::GethuffmanNode()
{
	return _root;
}