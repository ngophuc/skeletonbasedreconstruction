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
 *  \file Compositor.h
 *  \brief Defines application compositor
 *  \author Bastien Durix
 */

#ifndef _COMPOSITOR_H_
#define _COMPOSITOR_H_

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
		 * \brief Recursive template composition of n function
		 */
		template<typename Fct, typename... Args>
		class Compositor : Application<typename Fct::outType, typename Fct::inType>
		{
			public:
				/**
				 *  \brief Out type of result funtion
				 */
				using outType = typename Fct::outType;
				/**
				 *  \brief In type of result funtion
				 */
				using inType = typename Fct::inType;
			protected:
				/**
				 *  \brief Current function
				 */
				Fct m_fct;
			public:
				/**
				 *  \brief Constructor
				 */
				Compositor() : m_fct() {};
				/**
				 *  \brief Constructor
				 *
				 *  \param fct reference to function to compose
				 */
				Compositor(const Fct &fct) : m_fct(fct) {};

				/**
				 *  \brief Function call
				 *
				 *  \param t input variable
				 *  \return Function evaluation at t
				 */
				outType operator()(const inType &t) const
				{
					return m_fct(t);
				};

				/**
				 *  \brief Jacobian call
				 *
				 *  \param t input variable
				 *  \return Jacobian evaluation at t
				 */
				template<typename Out = outType, typename In = inType>
				Eigen::Matrix<double,dimension<Out>::value,dimension<In>::value> jac(const inType &t) const
				{
					return m_fct.jac(t);
				}
		};

		/**
		 * \brief Recursive template composition of n function
		 */
		template<typename Fct, typename Fct_, typename... Args>
		class Compositor<Fct,Fct_,Args...> : Application<typename Fct::outType,typename Compositor<Fct_,Args...>::inType>
		{
			public:
				/**
				 *  \brief Out type of result funtion
				 */
				using outType = typename Fct::outType;
				/**
				 *  \brief In type of result funtion
				 */
				using inType = typename Compositor<Fct_,Args...>::inType;
			protected:
				/**
				 *  \brief Current function
				 */
				Fct m_fct;
				/**
				 * \brief Compositor contain all the next functions
				 */
				Compositor<Fct_,Args...> m_next;

			public:
				/**
				 *  \brief Constructor
				 */
				Compositor() :  m_fct(), m_next() {};


				/**
				 *  \brief Constructor
				 *
				 *  \param fct   reference to first function to compose
				 *  \param fct_  reference to second function to compose
				 *  \param fcts  next function arguments
				 */
				Compositor(const Fct &fct, const Fct_ &fct_, const Args&... fcts) :  m_fct(fct), m_next(fct_, fcts...) {};

				/**
				 *  \brief Function call
				 *
				 *  \param t input variable
				 *  \return Function evaluation at t
				 */
				outType operator()(const inType &t) const
				{
					return m_fct(  m_next(t) );
				};

				/**
				 *  \brief Jacobian call
				 *
				 *  \param t input variable
				 *  \return Jacobian evaluation at t
				 */
				template<typename Out = outType, typename In = inType>
				Eigen::Matrix<double,dimension<Out>::value,dimension<In>::value> jac(const inType &t) const
				{
					return m_fct.jac( m_next(t) ) * m_next.jac(t);
				}

				/**
				 *  \brief Next function getter
				 *
				 *  \return next function
				 */
				inline const Compositor<Fct_,Args...>& next() const
				{
					return m_next;
				}
		};
	}
}

#endif //_COMPOSITOR_H_
