#ifndef MATRIX_H
#define MATRIX_H

struct Matrix
{
 public:
  unsigned N;
  vector<vector<double>> matrix;

  void init(const unsigned n){matrix.reserve(N);}
  void emplace_back(const vector<double> row){matrix.emplace_back(row);}
  Matrix(){}
  Matrix(const unsigned n)
  {
    matrix.reserve(n);
    N=n;
  }
  ~Matrix(){}
};

#endif
