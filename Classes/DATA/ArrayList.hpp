#include "cocos2d.h"
#include "../COMMON/ComMath.h"

USING_NS_CC;

template<typename T>
ArrayList<T>::ArrayList() :
	curMode	(ListMode),
	pHead	(NULL),
	pTail	(NULL),
	pGarbage(NULL),
	pArray(NULL),
	count	(0)
{}

template<typename T>
ArrayList<T>::ArrayList( ArrayList& obj ) :
	curMode  (obj.curMode),
	pHead (NULL),
	pTail (NULL),
	pGarbage(NULL),
	pArray(NULL),
	count (obj.count)
{
	if (obj.count > 0) {
		if ( obj.curMode == ArrayMode ) {

			pArray = new T[obj.count];

			//"memcpy" cant use, because object T can use Heap Memory.

			for(int i=0; i<count; i++)
				pArray[i] = obj.pArray[i];
		} else {
			ArrayListNode<T> *node = new ArrayListNode<T>();
			node->data = obj.pHead->data;
			node->pNext = NULL;
			pHead = pTail = node;

			ArrayListNode<T> *pFind = obj.pHead->pNext;
			while( pFind != NULL ) {
				ArrayListNode<T> *node = new ArrayListNode<T>();
				node->data = pFind->data;
				node->pNext = NULL;

				pTail->pNext = node;
				pTail = node;

				pFind = pFind->pNext;
			}
		}

#ifdef ARRAY_LIST_DEBUG
		g_arraylist_byte_new += sizeof(T) * obj.count;
#endif
	}
}

template<typename T>
ArrayList<T>::~ArrayList()
{
	//if ( curMode == ArrayMode ) {
		if ( pArray != NULL )
			delete[] pArray;
	//} else {
		ArrayListNode<T> *pFind, *pDel;

		pFind = pHead;
		pDel = NULL;
		while( pFind != NULL ) {
			pDel = pFind;
			pFind = pFind->pNext;
#ifdef ARRAY_LIST_DEBUG
			g_arraylist_byte_delete += sizeof(T);
#endif
			delete(pDel);
		}

		pFind = pGarbage;
		pDel = NULL;
		while( pFind != NULL ) {
			pDel = pFind;
			pFind = pFind->pNext;
#ifdef ARRAY_LIST_DEBUG
			g_arraylist_byte_delete += sizeof(T);
#endif
			delete(pDel);
		}
	//}
}

template<typename T>
T& ArrayList<T>::Add() {
	if ( curMode == ArrayMode )
		ChangeMode(ListMode);
	else
		curMode = ListMode;

	ArrayListNode<T> *node;
	if (pGarbage == NULL) {
		node = new ArrayListNode<T>();
#ifdef ARRAY_LIST_DEBUG
		g_arraylist_byte_new += sizeof(T);
#endif
	} else {
		node = pGarbage;
		pGarbage = pGarbage->pNext;
#ifdef ARRAY_LIST_DEBUG
		g_arraylist_count_from_garbage++;
#endif
	}
	
 	if ( count == 0 ) {
		node->pNext = NULL;
		pHead = pTail = node;
	} else {
		node->pNext = NULL;
		pTail->pNext = node;
		pTail = node;
	}
	++count;

	return node->data;
}

template<typename T>
void ArrayList<T>::Add( const T& data )
{
	if ( curMode == ArrayMode )
		ChangeMode(ListMode);
	else
		curMode = ListMode;

	ArrayListNode<T> *node;
	if (pGarbage == NULL) {
		node = new ArrayListNode<T>();
#ifdef ARRAY_LIST_DEBUG
		g_arraylist_byte_new += sizeof(T);
#endif
	} else {
		node = pGarbage;
		pGarbage = pGarbage->pNext;
#ifdef ARRAY_LIST_DEBUG
		g_arraylist_count_from_garbage++;
#endif
	}

	node->data = data;
	if ( count == 0 ) {
		node->pNext = NULL;
		pHead = pTail = node;
	} else {
		node->pNext = NULL;
		pTail->pNext = node;
		pTail = node;
	}
	++count;
}

template<typename T>
void ArrayList<T>::Add( int index, const T& data )
{
	if ( curMode == ArrayMode )
		ChangeMode(ListMode);
	else
		curMode = ListMode;

	ArrayListNode<T> *node;
	if (pGarbage == NULL) {
		node = new ArrayListNode<T>();
#ifdef ARRAY_LIST_DEBUG
		g_arraylist_byte_new += sizeof(T);
#endif
	} else {
		node = pGarbage;
		pGarbage = pGarbage->pNext;
#ifdef ARRAY_LIST_DEBUG
		g_arraylist_count_from_garbage++;
#endif
	}

	node->data = data;
	if ( count == 0 ) {
		node->pNext = NULL;
		pHead = pTail = node;
	} else if ( count <= index ) {
		node->pNext = NULL;
		pTail->pNext = node;
		pTail = node;
	} else if ( index <= 0 ) {
		node->pNext = pHead;
		pHead = node;
	} else {
		ArrayListNode<T> *pLast = pHead;
		ArrayListNode<T> *pFind = pHead->pNext;
		for(int i=1; i<index; i++) {
			pLast = pFind;
			pFind = pFind->pNext;

#ifdef ARRAY_LIST_DEBUG
			g_arraylist_count_move_node++;
#endif
		}
		pLast->pNext = node;
		node->pNext = pFind;
	}
	++count;
}

template<typename T>
void ArrayList<T>::Remove( int index )
{
	if ( curMode == ArrayMode )
		ChangeMode(ListMode);
	else
		curMode = ListMode;

	if ( pHead == NULL || index >= count )
		return;

	ArrayListNode<T> *pLast = NULL;
	ArrayListNode<T> *pFind = pHead;
	for(int i=0; i<index; i++) {
		pLast = pFind;
		pFind = pFind->pNext;

#ifdef ARRAY_LIST_DEBUG
		g_arraylist_count_move_node++;
#endif
	}
	ArrayListNode<T> *pNext = pFind->pNext;

	pFind->pNext = pGarbage;
	pGarbage = pFind;
	//delete(pFind);
#ifdef ARRAY_LIST_DEBUG
	g_arraylist_count_to_garbage++;
#endif
	
	if (pLast != NULL) {//삭제한 이전노드가 있으면
		pLast->pNext = pNext;//삭제한 다음노드와 연결
		if (pFind == pTail)//삭제한 노드가 마지막노드였으면
			pTail = pLast;
		count--;
	} else {//삭제한 이전노드가 없으면, 
		pHead = pNext;//삭제한 다음노드가 헤더노드이고,
		if (pFind == pTail) {//삭제한 노드가 마지막노드였으면 
			pHead = pTail = NULL;//빈 리스트임
			count = 0;
		} else {
			count--;
		}
	}
}

template<typename T>
void ArrayList<T>::RemoveAll()
{
	if ( curMode == ArrayMode ) {
		if ( pArray != NULL )
			delete[] pArray;
		pArray = NULL;

#ifdef ARRAY_LIST_DEBUG
		g_arraylist_count_move_node += sizeof(T) * count;
#endif
	} else {
		pGarbage = pHead;
// 		ArrayListNode<T> *pFind = m_pHead;
// 		ArrayListNode<T> *pDel = NULL;
// 		while( pFind != NULL ) {
// 			pDel = pFind;
// 			pFind = pFind->pNext;
// 			delete(pDel);
// 		}
		pHead = pTail = NULL;

#ifdef ARRAY_LIST_DEBUG
		g_arraylist_count_to_garbage += count;
#endif
	}
	count = 0;
}

template<typename T>
void ArrayList<T>::SetLength( int size )
{
	if (size < 0)
		throw ArrayListException_OutOfRange();	

	if ( curMode == ListMode )
		ChangeMode(ArrayMode);
	else
		curMode = ArrayMode;


	if (size > 0) {
		T *newarray = new T[size];

		if (pArray != NULL) {
			int mini = _MIN(size,count);
			for(int i=0; i<mini; i++)
				newarray[i] = pArray[i];
			//memcpy(newarray,pArray, sizeof(T)*(min(size,count)));
			delete[] pArray;

#ifdef ARRAY_LIST_DEBUG
			g_arraylist_byte_new += sizeof(T) * size;
			g_arraylist_byte_delete += sizeof(T) * count;
#endif
		}

		pArray = newarray;
	} else {
		if (pArray != NULL)
			delete[] pArray;

#ifdef ARRAY_LIST_DEBUG
		g_arraylist_byte_delete += sizeof(T) * count;
#endif

		pArray = NULL;
	}

	count = size;
}

template<typename T>
void ArrayList<T>::ChangeMode( ArrayListMode mode )
{
	if ( curMode == mode )
		return;

	curMode = mode;
	if ( count == 0 )
		return;

	if ( mode == ArrayMode ) {

		if (pArray != NULL)
			delete[] pArray;

		pArray = new T[count];
#ifdef ARRAY_LIST_DEBUG
		g_arraylist_byte_new += sizeof(T) * count;
#endif

		int i=0;
		ArrayListNode<T> *pFind, *pDel;
		
		pFind = pHead;
		pDel = NULL;
		while( pFind != NULL ) {
			pDel = pFind;
			pFind = pFind->pNext;
			pArray[i++] = pDel->data;
			delete(pDel);

#ifdef ARRAY_LIST_DEBUG
			g_arraylist_byte_delete += sizeof(T);
#endif
		}

		pFind = pGarbage;
		pDel = NULL;
		while( pFind != NULL ) {
			pDel = pFind;
			pFind = pFind->pNext;
			delete(pDel);

#ifdef ARRAY_LIST_DEBUG
			g_arraylist_byte_delete += sizeof(T);
#endif
		}
		pGarbage = NULL;

		pHead = pTail = NULL;
	} else {
		ArrayListNode<T> *node = new ArrayListNode<T>();
		node->data = pArray[0];
		node->pNext = NULL;
		pHead = pTail = node;

		for(int i=1; i<count; i++) {
			node = new ArrayListNode<T>();
			node->data = pArray[i];
			node->pNext = NULL;
			pTail->pNext = node;
			pTail = node;
		}

#ifdef ARRAY_LIST_DEBUG
		g_arraylist_byte_new += sizeof(T) * count;
#endif

		delete[] pArray;
		pArray = NULL;
	}

}

template<typename T>
int ArrayList<T>::GetLength() const
{
	return count;
}

template<typename T>
bool ArrayList<T>::isHaveGarbage() const
{
	return (pGarbage != NULL);
}

template<typename T>
T* ArrayList<T>::GetHead()
{
	if (count == 0)
		return NULL;

	if (curMode == ArrayMode)
		return &pArray[0];
	else
		return &(pHead->data);
}

template<typename T>
T* ArrayList<T>::GetTail()
{
	if (count == 0)
		return NULL;

	if (curMode == ArrayMode)
		return &pArray[count-1];
	else
		return &(pTail->data);
}

template<typename T>
T& ArrayList<T>::operator[]( int index )
{
	if (index < 0)
		throw ArrayListException_OutOfRange();

	if (index >= count) {
		CCLOG("TArrayList Size AutoChanged %d => %d", count, index+1);
		SetLength(index+1);
	}

	if ( curMode == ArrayMode ) {
		return pArray[index];
	} else {
		ArrayListNode<T> *pFind = pHead;
		for(int i=0; i<index; i++)
			pFind = pFind->pNext;

#ifdef ARRAY_LIST_DEBUG
		g_arraylist_count_move_node += index;
#endif

		return pFind->data;


	}
}

template<typename T>
void ArrayList<T>::operator=( const ArrayList &obj )
{
	RemoveAll();
	curMode = obj.curMode;
	count = obj.count;

	if (obj.count > 0) {
		if ( curMode == ArrayMode ) {

			pArray = new T[count];

			//"memcpy" cant use, because object T can use Heap Memory.

			for(int i=0; i<count; i++)
				pArray[i] = obj.pArray[i];
		} else {
			ArrayListNode<T> *node = new ArrayListNode<T>();
			node->data = obj.pHead->data;
			node->pNext = NULL;
			pHead = pTail = node;

			ArrayListNode<T> *pFind = obj.pHead->pNext;
			while( pFind != NULL ) {
				node = new ArrayListNode<T>();
				node->data = pFind->data;
				node->pNext = NULL;

				pTail->pNext = node;
				pTail = node;

				pFind = pFind->pNext;

#ifdef ARRAY_LIST_DEBUG
				g_arraylist_count_move_node++;
#endif

			}
		}
	}
}

template<typename T>
int ArrayList<T>::GetSize() const
{
	return sizeof(T);
}