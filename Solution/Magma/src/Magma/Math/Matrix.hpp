#pragma once

#include "..\Utils\Serializable.hpp"

namespace Magma
{
	/// <summary>
	///		Matrix class used for math operations
	/// </summary>
	template <typename T, size_t columns, size_t rows>
	class Matrix : public Serializable
	{
	public:
		/// <summary>
		///		Gets this matrix data pointer
		/// </summary>
		/// <returns>Matrix data pointer</returns>
		inline T* Data() { return &m_data[0]; }

	private:
		T m_data[columns * rows];

		// Inherited via Serializable
		virtual void Serialize(std::ostream & ostream) const override
		{
			
		}

		virtual void Deserialize(std::istream & stream) override
		{

		}
	};

	template <typename T = float> using Matrix1   = Matrix<T, 1, 1>;
	template <typename T = float> using Matrix1x1 = Matrix<T, 1, 1>;
	template <typename T = float> using Matrix1x2 = Matrix<T, 1, 2>;
	template <typename T = float> using Matrix1x3 = Matrix<T, 1, 3>;
	template <typename T = float> using Matrix1x4 = Matrix<T, 1, 4>;

	template <typename T = float> using Matrix2   = Matrix<T, 2, 2>;
	template <typename T = float> using Matrix2x1 = Matrix<T, 2, 1>;
	template <typename T = float> using Matrix2x2 = Matrix<T, 2, 2>;
	template <typename T = float> using Matrix2x3 = Matrix<T, 2, 3>;
	template <typename T = float> using Matrix2x4 = Matrix<T, 2, 4>;

	template <typename T = float> using Matrix3   = Matrix<T, 3, 2>;
	template <typename T = float> using Matrix3x1 = Matrix<T, 3, 1>;
	template <typename T = float> using Matrix3x2 = Matrix<T, 3, 2>;
	template <typename T = float> using Matrix3x3 = Matrix<T, 3, 3>;
	template <typename T = float> using Matrix3x4 = Matrix<T, 3, 4>;

	template <typename T = float> using Matrix4   = Matrix<T, 4, 2>;
	template <typename T = float> using Matrix4x1 = Matrix<T, 4, 1>;
	template <typename T = float> using Matrix4x2 = Matrix<T, 4, 2>;
	template <typename T = float> using Matrix4x3 = Matrix<T, 4, 3>;
	template <typename T = float> using Matrix4x4 = Matrix<T, 4, 4>;
}