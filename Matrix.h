#ifndef MATRIX_H
#define MATRIX_H

struct Iterator
{
private:
  int p;
public:
  explicit Iterator(int *d):p(d){}
  int &operator*()(return *p;)
  /* vector<double>*() const {return *p;} */
  /* vector<double> operator->(){return p;} */
  Iterator &operator++()
  {
    p++;
    return *this;
  }
  /* Iterator operator++(int) */
  /* { */
  /*   Iterator t=*this; */
  /*   ++(*this); */
  /*   return t; */
  /* } */
  bool operator==(const Iterator &x, const Iterator &y){return x.p==y.p;}
  bool operator!=(const Iterator &x, const Iterator &y){return x.p!=y.p;}
    /*  private: */
    /*     int* ptr;  // Pointer to the current element */

    /* public: */
        // Constructor
        /* explicit Iterator(int* p) : ptr(p) {} */

        // Dereference operator to access the element
        /* int& operator*() { return *ptr; } */

        // Increment operator to move to the next element
        /* Iterator& operator++() { */
        /*     ++ptr; */
        /*     return *this; */
        /* } */

    /*     // Equality comparison operator to compare iterators */
    /*     bool operator!=(const Iterator& other) const { */
    /*         return ptr != other.ptr; */
    /*     } */
    /* }; */

    /* // Begin iterator (returns an iterator pointing to the first element) */
    /* Iterator begin() { */
    /*     return Iterator(&data[0]); */
    /* } */

    /* // End iterator (returns an iterator pointing past the last element) */
    /* Iterator end() { */
    /*     return Iterator(&data[data.size()]); */
    /* } */

    /* // Method to print the container (for testing purposes) */
    /* void print() const { */
    /*     for (const auto& value : data) { */
    /*         std::cout << value << " "; */
    /*     } */
    /*     std::cout << std::endl; */
    /* } */
};

struct Matrix
{
 public:
  unsigned N;
  vector<vector<double>> matrix;

  vector<double> begin(){return matrix[0];}
  void init(const unsigned n){matrix.reserve(N);}
  void init2(const unsigned n)
  {
    matrix.reserve(n);
    vector<double> row;
    row.reserve(n);

    for(unsigned i=0; i<n; i++)
      {
	for(unsigned i=0; i<n; i++)
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
