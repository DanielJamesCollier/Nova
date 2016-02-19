#pragma once
#include <algorithm>
// the forward reverse map is two maps stored into one array to avoid memory fragmentation
// the reverse map can be accesed with an index + poolSize

template <class T>
class StaticPool
{
public:
	explicit StaticPool(const unsigned int poolSize)
		:
		m_poolSize(poolSize),
		m_partition(0),
		m_pool(new T[poolSize]),
		m_forwardMap(new int[poolSize]),
		m_reverseMap(new int[poolSize])
	{
		// initialiastation
		for (unsigned int i = 0; i < poolSize; i++)
		{
			m_forwardMap[i] = m_reverseMap[i] = -1;
		}
	}

	~StaticPool()
	{
		delete[] m_pool, m_forwardMap, m_reverseMap;
	}

	int AddObject(T object)
	{
		if (m_partition == m_poolSize) return -1; // check if the pool is full befor adding an object 

		m_forwardMap[m_partition] = m_partition;
		m_reverseMap[m_partition] = m_partition;
		m_pool[m_partition]		  = object;
		return m_partition++;
	}

	int AddObject(unsigned int id, T object)
	{
		if (m_partition == m_poolSize) return -1; // check if the pool is full befor adding an object 


		if (m_forwardMap[id] != -1) return -1; // check if an object is allready at id location

		std::cout << "passed " << std::endl;

		m_forwardMap[id] = m_partition;
		m_reverseMap[m_partition] = id;
		m_pool[m_partition] = object;
		return m_partition++;
	}


	T& GetObject(unsigned int index, bool& valid)
	{
		int componentPoolIndex = m_forwardMap[index];

		if (index > m_poolSize)
		{
			valid = false;
			return m_pool[m_poolSize];
		}
		else if(componentPoolIndex == -1 || componentPoolIndex > m_partition)
		{
			valid = false;
			return m_pool[m_poolSize];
		}

		valid = true;
		return m_pool[componentPoolIndex];
	}
	
	void RemoveObject(unsigned int index)
	{

		if (index > m_poolSize)
		{
			std::cout << "greater than pool size" << std::endl;
			return;
		}

		int poolIndextoDelete = m_forwardMap[index];

		// there is no object mapped so return
		if (poolIndextoDelete == -1) 
		{
			std::cout << "the object at " << index << " could not be deleted as it does not exits" << std::endl;
			return;
		}

		std::swap(m_pool[poolIndextoDelete], m_pool[--m_partition]);

		// re-map after swap
		unsigned int test = m_reverseMap[m_partition];
		m_forwardMap[test] = poolIndextoDelete;
		m_forwardMap[index]						= -1;
		m_reverseMap[m_partition]				= -1;
		
		if (poolIndextoDelete != m_partition)
		{
			m_reverseMap[poolIndextoDelete] = test;
		}
		else
		{
			m_reverseMap[poolIndextoDelete] = -1;
		}
	}

	unsigned int GetActiveObjectCount() const
	{
		return m_partition;
	}

	unsigned int GetPoolSize() const
	{
		return m_poolSize;
	}

	void PrintPool() const
	{
		std::cout << "  Rev   |  Forw  |  Pool " << std::endl;
		for (unsigned int i = 0; i < 20; i++)
		{
			
			std::cout << "| " << i << " - " << m_reverseMap[i] << " | "; // prints first row reverse map
			std::cout << i << " -  " << m_forwardMap[i] << " | "; // prints the forward mapp
			if (i < m_partition)
			{
				std::cout << i << "- A";
			}
			else
			{
				std::cout << i << "- D";
			}
			std::cout << std::endl;
		}
	}

private:
	int* m_forwardMap;
	int* m_reverseMap;
	T*	 m_pool;

	unsigned int m_poolSize;
	unsigned int m_partition; // dead alive partition
};

