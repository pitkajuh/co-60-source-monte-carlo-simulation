#ifndef MATRIX_H
#define MATRIX_H

struct Matrix
{
 public:
  unsigned N;
  vector<vector<double>> matrix;

  void Set(const unsigned i, const unsigned j, const double v)
  {
    matrix[i][j]=v;
  }
  double &Get(const unsigned i, const unsigned j)
  {
    return matrix[i][j];
  }
  void init(const unsigned n)
  {
    matrix.reserve(n);
    N=n;
  }
  void init2(const unsigned n)
  {
    matrix.reserve(n);
    vector<double> row;
    row.reserve(n);
    N=n;

    for(unsigned i=0; i<n; i++)
      {
	for(unsigned j=0; j<n; j++)
	  {
	    row.emplace_back(0);
	  }
	matrix.emplace_back(row);
	row.clear();
      }
  }
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
