#ifndef K_H
#define K_H

struct K
{
  vector<double> t1;

  unsigned size(){return t1.size();}

  void print()
  {
    for(const auto &i: t1)
      {
	cout<<i<<'\n';
      }
  }
  void emplace_back(const double v){t1.emplace_back(v);}

  K operator+(K t)
  {
    const int size=t1.size();
    K result(size);

    for(int i=0; i<size; i++) result.t1.push_back(t1[i]+t.t1[i]);
    return result;
  }

  K operator+(double t)
  {
    const int size=t1.size();
    K result(size);

    for(int i=0; i<size; i++) result.t1.push_back(t1[i]+t);
    return result;
  }

  K operator*(K t)
  {
    const int size=t1.size();
    K result(size);

    for(int i=0; i<size; i++) result.t1.push_back(t1[i]*t.t1[i]);
    return result;
  }

  K operator*(const double t)
  {
    const int size=t1.size();
    K result(size);

    for(int i=0; i<size; i++) result.emplace_back(t1[i]*t);
    return result;
  }

  K operator/(K t)
  {
    const int size=t1.size();
    K result(size);

    for(int i=0; i<size; i++) result.t1.push_back(t1[i]/t.t1[i]);
    return result;
  }

  K operator/(double t)
  {
    const int size=t1.size();
    K result(size);

    for(int i=0; i<size; i++) result.t1.push_back(t1[i]/t);
    return result;
  }

  void init(const unsigned size1)
  {
    for(unsigned i=0; i<size1; i++)
      {
	t1.emplace_back(0);
      }
  }

  K(){}
  K(const unsigned size){t1.reserve(size);}
  ~K(){}
};

#endif
