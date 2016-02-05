/*
Copyright (c) 2016 Bastien Durix

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


/**
 *  \file  Basis.h
 *  \brief Defines a basis of a vectorial space
 *  \author Bastien Durix
 */

#ifndef _BASIS_H_
#define _BASIS_H_

#include <Eigen/Dense>
#include <stdexcept>

/**
 *  \brief Mathematical tools
 */
namespace mathtools
{
	/**
	 *  \brief Vectorial tools
	 */
	namespace vectorial
	{
		/**
		 *  \class Basis
		 *  \brief Basis of a vectorial space
		 *  \tparam Dim: dimension of the vectorial space
		 */
		template<unsigned int Dim>
		class Basis
		{
			private:
				/**
				 *  \brief Basis coordinates in canonic basis
				 */
				Eigen::Matrix<double,Dim,Dim> m_matrix;

				/**
				 *  \brief Canonic basis coordinates in basis
				 */
				Eigen::Matrix<double,Dim,Dim> m_matrix_inv;
			public:
				/**
				 *  \brief Constructor
				 *
				 *  \param matrix  full rank matrix
				 *
				 *  \throws logic_error if matrix parameter not invertible
				 */
				Basis(const Eigen::Matrix<double,Dim,Dim> &matrix = Eigen::Matrix<double,Dim,Dim>::Identity()) : m_matrix(matrix)
				{
					double det = m_matrix.determinant();
					if(det*det <= Eigen::NumTraits<double>::dummy_precision())
					{
						throw new std::logic_error("mathtools::vectorial::Basis() : basis parameter not invertible");
					}
					m_matrix_inv = m_matrix.inverse();
				}

				/**
				 *  \brief 2d basis constructor 
				 *
				 *  \param vec1 first basis vector
				 *  \param vec2 second basis vector
				 *
				 *  \throws logic_error if matrix parameter not invertible
				 */
				Basis(const Eigen::Vector2d &vec1, const Eigen::Vector2d &vec2)
				{
					m_matrix << vec1, vec2;
					double det = m_matrix.determinant();
					if(det*det <= Eigen::NumTraits<double>::dummy_precision())
					{
						throw new std::logic_error("mathtools::vectorial::Basis() : basis parameter not invertible");
					}
					m_matrix_inv = m_matrix.inverse();
				}

				/**
				 *  \brief 3d basis constructor 
				 *
				 *  \param vec1 first basis vector
				 *  \param vec2 second basis vector
				 *  \param vec3 third basis vector
				 *
				 *  \throws logic_error if matrix parameter not invertible
				 */
				Basis(const Eigen::Vector3d &vec1, const Eigen::Vector3d &vec2, const Eigen::Vector3d &vec3)
				{
					m_matrix << vec1, vec2, vec3;
					double det = m_matrix.determinant();
					if(det*det <= Eigen::NumTraits<double>::dummy_precision())
					{
						throw new std::logic_error("mathtools::vectorial::Basis() : basis parameter not invertible");
					}
					m_matrix_inv = m_matrix.inverse();
				}

				/**
				 *  \brief Copy constructor
				 *
				 *  \param basis basis to copy
				 *
				 *  \throws logic_error if basis parameter not invertible
				 */
				Basis(const Basis<Dim> &basis) : m_matrix(basis.m_matrix), m_matrix_inv(basis.m_matrix_inv)
				{}

				/**
				 *  \brief Basis matrix accessor
				 *
				 *  \return Basis matrix in canonic basis
				 */
				inline const Eigen::Matrix<double,Dim,Dim>& getMatrix() const
				{
					return m_matrix;
				}

				/**
				 *  \brief Inverse basis matrix accessor
				 *
				 *  \return Canonic basis matrix in actual basis
				 */
				inline const Eigen::Matrix<double,Dim,Dim>& getMatrixInverse() const
				{
					return m_matrix_inv;
				}
		};
	}
}

#endif //_BASIS_H_