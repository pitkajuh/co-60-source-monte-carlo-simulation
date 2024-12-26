#ifndef K_H
#define K_H

struct K
{
  vector<double> t1;

  K operator+(K t)
  {
    const int size=t1.size();
    K result(size);

    for(int i=0; i<=size; i++) result.t1.push_back(t1[i]+t.t1[i]);
    return result;
  }

  K operator+(double t)
  {
    const int size=t1.size();
    K result(size);

    for(int i=0; i<=size; i++) result.t1.push_back(t1[i]+t);
    return result;
  }

  K operator*(K t)
  {
    const int size=t1.size();
    K result(size);

    for(int i=0; i<=size; i++) result.t1.push_back(t1[i]*t.t1[i]);
    return result;
  }

  K operator*(double t)
  {
    const int size=t1.size();
    K result(size);

    for(int i=0; i<=size; i++) result.t1.push_back(t1[i]*t);
    return result;
  }

  K operator/(K t)
  {
    const int size=t1.size();
    K result(size);

    for(int i=0; i<=size; i++) result.t1.push_back(t1[i]/t.t1[i]);
    return result;
  }

  K operator/(double t)
  {
    const int size=t1.size();
    K result(size);

    for(int i=0; i<=size; i++) result.t1.push_back(t1[i]/t);
    return result;
  }

  K(){}
  K(const unsigned size){t1.reserve(size);}
  ~K(){}
};

#endif
