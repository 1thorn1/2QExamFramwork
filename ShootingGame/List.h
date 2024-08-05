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

		//////////////////////////////////////////////////////////////////////////

		m_size++;
	}


	void erase(Node* pNode)
	{
		//////////////////////////////////////////////////////////////////////////
		// EXAM_02: ���� ���� ����Ʈ���� Ư�� ��带 �����ϴ� �Լ��� �����Ͻÿ�.

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

