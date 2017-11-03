#pragma once

#include "..\Utils\Serializable.hpp"
#include "..\Utils\Utils.hpp"

namespace Magma
{
	/// <summary>
	///		Matrix class used for math operations
	/// </summary>
	template <class T, size_t columns, size_t rows>
	class Matrix : public Serializable
	{
	public:
		Matrix()
		{
			static_assert(std::is_arithmetic<T>::value);

			for (size_t x = 0; x < columns; ++x)
				for (size_t y = 0; y < rows; ++y)

		}

		/// <summary>
		///		Gets this matrix data pointer
		/// </summary>
		/// <returns>Matrix data pointer</returns>
		inline T* Data() { return &m_data[0]; }

	private:
		T m_data[columns * rows];

		// Inherited via Serializable
		virtual void Serialize(std::ostream & stream) const final
		{
			stream << columns << " " << rows; // Store matrix size
			for (size_t i = 0; i < columns * rows; ++i) // Store matrix values
				stream << m_data[i] << ' ';
		}

		virtual void Deserialize(std::istream & stream) final
		{
			size_t oColumns, oRows;
			stream >> oColumns >> oRows;
			if (columns != oColumns && rows != oRows) // Check if matrix size matches the one being read
			{
				std::string msg = "Failed to deserialize matrix, size of matrix doesn't match the one being read -> ";
				msg += " (" + std::to_string(columns) + ";" + std::to_string(rows) + ") != (" + std::to_string(oColumns) + ";" + std::to_string(oRows) + ")";
				MAGMA_ERROR(msg);
				return;
			}
			for (size_t i = 0; i < columns * rows; ++i) // Read matrix values
				stream >> m_data[i];
		}
	};

	using Matrix1   = Matrix<float, 1, 1>;
	using Matrix1x1 = Matrix<float, 1, 1>;
	using Matrix1x2 = Matrix<float, 1, 2>;
	using Matrix1x3 = Matrix<float, 1, 3>;
	using Matrix1x4 = Matrix<float, 1, 4>;

	using Matrix2   = Matrix<float, 2, 2>;
	using Matrix2x1 = Matrix<float, 2, 1>;
	using Matrix2x2 = Matrix<float, 2, 2>;
	using Matrix2x3 = Matrix<float, 2, 3>;
	using Matrix2x4 = Matrix<float, 2, 4>;

	using Matrix3   = Matrix<float, 3, 2>;
	using Matrix3x1 = Matrix<float, 3, 1>;
	using Matrix3x2 = Matrix<float, 3, 2>;
	using Matrix3x3 = Matrix<float, 3, 3>;
	using Matrix3x4 = Matrix<float, 3, 4>;

	using Matrix4   = Matrix<float, 4, 2>;
	using Matrix4x1 = Matrix<float, 4, 1>;
	using Matrix4x2 = Matrix<float, 4, 2>;
	using Matrix4x3 = Matrix<float, 4, 3>;
	using Matrix4x4 = Matrix<float, 4, 4>;
}