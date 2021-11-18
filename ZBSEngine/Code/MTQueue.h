#pragma once
#include <memory.h>
template <class T> 
class MTQueue
{
private:
	T* m_pData;
	size_t m_nSize;
	size_t m_nCapacity;
public:
	MTQueue();
	~MTQueue();

	T* allocate();
	T* deallocate();
	T* Resize(size_t nNewSize);
	T* operator()();

	size_t Size();
	size_t Capacity();
	void Clear();

};
template<class T>
MTQueue<T>::MTQueue()
{
	m_pData = nullptr;
	m_nSize = 0;
	m_nCapacity = 0;
}

template<class T>
MTQueue<T>::~MTQueue()
{
	if (m_pData)
	{
		delete m_pData;
	}
}

template<class T>
T* MTQueue<T>::allocate()
{
	return new T[m_nCapacity];
}

template<class T>
T* MTQueue<T>::deallocate()
{
	if (m_pData)
		delete m_pData;
	return nullptr;
}

template<class T>
T* MTQueue<T>::Resize(size_t nNewSize)
{
	m_nSize = m_nCapacity;
	m_nCapacity = nNewSize;
	auto pNewData = allocate();
	if (m_pData)
	{
		memcpy(pNewData, m_pData, m_nCapacity);
	}
	m_pData = deallocate();
	m_pData = pNewData;
	return m_pData + m_nSize;
}

template<class T>
void MTQueue<T>::Clear() //this fing also works as a shrink_to_fit, but i thinks that not good
{
	m_pData = deallocate();
	m_nSize = 0;
	m_nCapacity = 0;
}

template<class T>
size_t MTQueue<T>::Size()
{
	return m_nSize;
}

template<class T>
size_t MTQueue<T>::Capacity()
{
	return m_nCapacity;
}

template<class T>
T* MTQueue<T>::operator()()
{
	return m_pData;
}