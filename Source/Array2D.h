#pragma once
#include <vector>

namespace Nova
{

	// contiguous Two Dimensional Array
	template<typename T>
	class Array2D
	{
	private:
		std::vector <T*> m_array;

	public:
		Array2D(unsigned int rows, unsigned int columns)
			:
			m_rows(rows),
			m_columns(columns),
			m_array(rows * columns)
		{
		}

		T* At(unsigned int row, unsigned int column)
		{
			assert(row > m_rows || column > m_columns);
			return  m_array.at(m_rows + row * column);
		}
		
	private:
		unsigned int m_rows, m_columns;
	};
}