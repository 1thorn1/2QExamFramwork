#pragma once
using namespace std;
template<typename T>
class List
{
public:
	struct Node
	{
		T data;
		Node* pNext = nullptr;
		Node* pPrev = nullptr;
	};
	Node* m_pHead = nullptr;
	Node* m_pTail = nullptr;
	int m_size = 0;

	void push_back(T data)
	{
		//////////////////////////////////////////////////////////////////////////
		// EXAM_01 : 이중 연결 리스트의 끝에 데이터를 추가하는 함수를 구현하시오.

		//////////////////////////////////////////////////////////////////////////

		m_size++;
	}


	void erase(Node* pNode)
	{
		//////////////////////////////////////////////////////////////////////////
		// EXAM_02: 이중 연결 리스트에서 특정 노드를 삭제하는 함수를 구현하시오.

		//////////////////////////////////////////////////////////////////////////

		
	}

	void clear()
	{
		Node* pCurr = m_pHead;
		Node* pNext = nullptr;
		while (pCurr != nullptr)
		{
			pNext = pCurr->pNext;
			erase(pCurr);
			pCurr = pNext;
		}
	}
	int size()
	{
		return m_size;
	}
};

