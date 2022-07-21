#ifndef __R2K_DATA__ARRAY_LIST__
#define __R2K_DATA__ARRAY_LIST__
#include "../COMMON/DebugingCode.h"

#include <iostream>

#define ARRAY_LIST_MIN_CAPACITY 4
#define ARRAY_LIST_DEBUG

enum ArrayListMode{
	ArrayMode,
	ListMode
};

class ArrayListException_NotSupportInThisMode{};
class ArrayListException_OutOfRange{};

template<typename T>
struct ArrayListNode{
	T data;
	ArrayListNode<T> *pNext;
};

template<typename T>
class ArrayList{
private:
	ArrayListMode curMode;	
	ArrayListNode<T> *pHead;
	ArrayListNode<T> *pTail;
	ArrayListNode<T> *pGarbage;

	T *pArray;
	int count;
	
public:

	ArrayList();
	ArrayList(ArrayList& obj);
	~ArrayList();

	//ListMode Only
	T& Add();
	void Add(const T& data);
	void Add(int index, const T& data);
	void Remove(int index);

	//ArrayMode Only
	void SetLength(int size);

	//All Mode Support
	void RemoveAll();
	void ChangeMode(ArrayListMode mode);
	int GetLength() const;

	bool isHaveGarbage() const;

	T* GetHead();
	T* GetTail();

	T& operator[](int index);

	void operator=(const ArrayList &tstr);

	int GetSize() const;
};

extern int g_arraylist_byte_new;
extern int g_arraylist_byte_delete;
extern int g_arraylist_count_to_garbage;
extern int g_arraylist_count_from_garbage;
extern int g_arraylist_count_move_node;

#include "ArrayList.hpp"

#endif