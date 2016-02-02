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
 *  \brief Provides tests for mathtools library
 *  \author Bastien Durix
 */

#include <iostream>
#include <math.h>
#include <mathtools/application/Bspline.h>
#include <mathtools/application/Compositor.h>
#include <mathtools/application/LinearApp.h>

using namespace mathtools::application;

unsigned int bsplinetest()
{
	bool bspline_test = true;
	unsigned int fraction = 100;
	
	std::cout << "Bspline test... ";
	std::vector<unsigned int> bin_coeff(6,0);
	bin_coeff[0] = 1;
	for(unsigned int degree = 1; degree < 5 && bspline_test; degree++)
	{
		// Filling node vector
		Eigen::Matrix<double,1,Eigen::Dynamic> node(1,degree*2);
		for(unsigned int i = 0; i < degree; i++)
		{
			node(0,i)        = 0.0;
			node(0,i+degree) = 1.0;
		}
		
		for(unsigned int i = degree; i > 0; i--)
		{
			bin_coeff[i] += bin_coeff[i-1];
		}
		
		for(unsigned int indice = 0; indice <= degree && bspline_test; indice++)
		{
			for(unsigned int i = 0; i < fraction && bspline_test; i++)
			{
				double t = (double)i*(1.0/(double)fraction);
				
				double res = BsplineBasis(t,degree,indice,node);
				
				double ref = pow((1-t),degree-indice)*pow(t,indice)*(double)bin_coeff[indice];
				
				if(fabs(res - ref) > std::numeric_limits<double>::epsilon())
					bspline_test = false;
			}
		}
	}

	unsigned int return_value = 0;
	if(!bspline_test)
	{
		return_value = -1;
		std::cout << "Fail!" << std::endl;
	}
	else
		std::cout << "Success!" << std::endl;
	
	return return_value;
}

unsigned int compositortest()
{
	std::cout << "Compositor test... ";
	
	Eigen::Matrix3d mat_lin;
	mat_lin <<  0.0, 2.0, 0.0,
			   -1.0, 0.0, 0.0,
			    0.0, 0.0, 3.0;
	unsigned int degree = 3;
	unsigned int nbctrlpt = 4;
	Eigen::Matrix<double,1,Eigen::Dynamic> nodevec(1,nbctrlpt + degree - 1);
	nodevec << 0.0, 0.0, 0.0, 1.0, 1.0, 1.0;
	Eigen::Matrix<double,3,Eigen::Dynamic> ctrlpt(3,nbctrlpt);
	ctrlpt << 0.0, 1.0, 1.0, 0.0,
			  0.0, 0.0, 1.0, 1.0,
			  0.0, 0.0, 0.0, 0.0;
	Bspline<3> bsp(ctrlpt,nodevec,degree);
	Compositor<LinearApp<3,3>,Bspline<3> > comp(LinearApp<3,3>(mat_lin),bsp);
	
	bool compositor_test = true;
	for(unsigned int i = 0; i < 101; i++)
	{
		double t = (double)i*0.01;

		Eigen::Vector3d vecres = comp(t);

		Eigen::Vector3d vecref = mat_lin * bsp(t);
		
		if( (vecres-vecref).norm() > std::numeric_limits<double>::epsilon() )
		{
			compositor_test = false;
		}
	}

	unsigned int return_value = 0;
	if(!compositor_test)
	{
		return_value = -1;
		std::cout << "Fail!" << std::endl;
	}
	else
		std::cout << "Success!" << std::endl;
	
	return return_value;
}

int main()
{
	unsigned int return_value = 0;
	
	unsigned int bspline_test_value = bsplinetest();
	if(bspline_test_value != 0) return_value = -1;
	
	unsigned int compositor_test_value = compositortest();
	if(compositor_test_value != 0) return_value = -1;

	return return_value;
}
