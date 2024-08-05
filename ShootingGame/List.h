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
		Node* newNode = new Node;
		newNode->data = data; // 데이터 push back

		// 예외 처리 -> 비어있을 때
		if (m_pTail == nullptr)
		{
			m_pHead = newNode;
			m_pTail = newNode;
		}
		else // 노드 연결
		{
			newNode->pPrev = m_pTail;
			newNode->pNext = nullptr;
			m_pTail->pNext = newNode;
			m_pTail = newNode;
		}

		//////////////////////////////////////////////////////////////////////////

		m_size++;
	}


	void erase(Node* pNode)
	{
		//////////////////////////////////////////////////////////////////////////
		// EXAM_02: 이중 연결 리스트에서 특정 노드를 삭제하는 함수를 구현하시오.

		
		// 헤드 노드를 지울 때
		if (pNode == m_pHead)
		{
			m_pHead = pNode->pNext;
			pNode->pPrev = nullptr;
		}

		// 태일 노드를 지울 때
		if (pNode == m_pTail)
		{
			m_pTail = pNode->pPrev;
			pNode->pNext = nullptr;
		}

		// 중간 노드를 지울 떄
		if (pNode != nullptr)
		{
			if (pNode->pNext != nullptr) { pNode->pNext->pPrev = pNode->pPrev; }
			if (pNode->pPrev != nullptr) { pNode->pPrev->pNext = pNode->pNext; }
		}
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

