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
		// EXAM_01 : ���� ���� ����Ʈ�� ���� �����͸� �߰��ϴ� �Լ��� �����Ͻÿ�.
		Node* newNode = new Node;
		newNode->data = data; // ������ push back

		// ���� ó�� -> ������� ��
		if (m_pTail == nullptr)
		{
			m_pHead = newNode;
			m_pTail = newNode;
		}
		else // ��� ����
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
		// EXAM_02: ���� ���� ����Ʈ���� Ư�� ��带 �����ϴ� �Լ��� �����Ͻÿ�.

		
		// ��� ��带 ���� ��
		if (pNode == m_pHead)
		{
			m_pHead = pNode->pNext;
			pNode->pPrev = nullptr;
		}

		// ���� ��带 ���� ��
		if (pNode == m_pTail)
		{
			m_pTail = pNode->pPrev;
			pNode->pNext = nullptr;
		}

		// �߰� ��带 ���� ��
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

