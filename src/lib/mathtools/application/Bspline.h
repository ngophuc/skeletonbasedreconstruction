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
 *  \file Bspline.h
 *  \brief Defines bspline function
 *  \author Bastien Durix
 */

#ifndef _BSPLINE_H_
#define _BSPLINE_H_

#include "Application.h"
#include "BsplineUtils.h"
#include <Eigen/Dense>

/**
 *  \brief Mathematical tools
 */
namespace mathtools
{
	/**
	 *  \brief Application tools
	 */
	namespace application
	{
		/**
		 *  \brief Defines Bspline curve
		 *
		 *  \tparam Dim Curve dimension
		 */
		template<unsigned int Dim>
		class Bspline : public Application<Eigen::Matrix<double,Dim,1>,double>
		{
			public:
				/**
				 *  \brief Out type of result funtion
				 */
				using outType = Eigen::Matrix<double,Dim,1>;
				/**
				 *  \brief In type of result funtion
				 */
				using inType = double;

			protected:
				/**
				 *  \brief Control points
				 */
				Eigen::Matrix<double,Dim,Eigen::Dynamic> m_ctrlpt;

				/**
				 *  \brief Node vector
				 */
				Eigen::Matrix<double,1,Eigen::Dynamic> m_nodevec;

				/**
				 *  \brief Bspline degree
				 */
				unsigned int m_degree;

			public:
				/**
				 *  \brief Constructor
				 *
				 *  \param ctrlpt   Control points
				 *  \param nodevec  Node vector
				 *  \param degree   Bspline degree
				 *
				 *  \throws std::logic_error if number of control points, nodes and degree does not match
				 */
				Bspline(const Eigen::Matrix<double,Dim,Eigen::Dynamic> &ctrlpt,
						const Eigen::Matrix<double,1,Eigen::Dynamic>   &nodevec,
						const unsigned int &degree) : 
					Application<Eigen::Matrix<double,Dim,1>,double>(), m_ctrlpt(ctrlpt), 
					m_nodevec(nodevec), m_degree(degree)
				{
					if( ctrlpt.cols() + degree != nodevec.cols()+1  )
						throw std::logic_error("Bspline : not verified #CtrlPt + degree = #NodeVec + 1");
				}

				/**
				 *  \brief Copy constructor
				 *
				 *  \param bspline Bspline to copy
				 */
				Bspline(const Bspline<Dim> &bspline) : 
					Application<Eigen::Matrix<double,Dim,1>,double>(), m_ctrlpt(bspline.m_ctrlpt), 
					m_nodevec(bspline.m_nodevec), m_degree(bspline.m_degree) {};
				
				/**
				 *  \brief Bspline call
				 *
				 *  \param t Bspline parameter
				 * 
				 *  \return Bspline evaluation at t
				 */
				Eigen::Matrix<double,Dim,1> operator()(const double &t) const
				{
					Eigen::Matrix<double,Dim,1> res = Eigen::Matrix<double,Dim,1>::Zero();

					for(unsigned int ind = 0; ind<m_ctrlpt.cols(); ind++)
					{
						double basis_ind = BsplineBasis(t,m_degree,ind,m_nodevec);
						res+=m_ctrlpt.block(0,ind,Dim,1)*basis_ind;
					}

					return res;
				}
		};
	}
}


#endif //_BSPLINE_H_