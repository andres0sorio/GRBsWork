// $Id: $
#ifndef UBLASEXT_H 
#define UBLASEXT_H 1

#include <iostream>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp> 

using namespace boost::numeric;

template<class matrix_T>
double determinant(ublas::matrix_expression<matrix_T> const& mat_r)
{
  double det = 1.0;
  
  matrix_T mLu(mat_r() );
  ublas::permutation_matrix<std::size_t> pivots(mat_r().size1() );
  
  int is_singular = lu_factorize(mLu, pivots);
  
  if (!is_singular)
    {
    for (std::size_t i=0; i < pivots.size(); ++i)
      {
      if (pivots(i) != i)
        det *= -1.0;
      
      det *= mLu(i,i);
    }
  }
  else {
    std::cout << "Warning: this matrix is singular. Cannot calculate the determinant" << std::endl; 
    det = 0.0;
  }
  
  return det;

};

/* Matrix inversion routine.
   Uses lu_factorize and lu_substitute in uBLAS to invert a matrix */

template<class T>
bool invertMat (const ublas::matrix<T>& input, ublas::matrix<T>& inverse) {
  
  typedef permutation_matrix<std::size_t> pmatrix;
  
  // create a working copy of the input
  matrix<T> A(input);
  
  // create a permutation matrix for the LU-factorization
  pmatrix pm(A.size1());
  
  // perform LU-factorization
  int res = lu_factorize(A,pm);
  if( res != 0 ) return false;
  
  // create identity matrix of "inverse"
  inverse.assign(ublas::identity_matrix<T>(A.size1()));
  
  // backsubstitute to get the inverse
  lu_substitute(A, pm, inverse);
  
  return true;
  
};

#endif //
