#ifndef GAUSSSEIDEL_H
#define GAUSSSEIDEL_H

#include "CentralDifference.h"
#include <algorithm>

struct GaussSeidel
{
private:

  void print(const vector<vector<double>> &matrix)
  {
    std::ofstream file;
    file.open("gaussseidel.txt");
    unsigned i=0;
    for(const auto &row: matrix)
      {
	for(const auto &columnValue: row)
	  {
	    file<<columnValue;

	    if(i<row.size()-1) file<<';';
	    i++;
	  }
	i=0;
	file<<'\n';
      }
    file.close();
  }

  void print2(const vector<double> &matrix, const vector<double> &matrix2)
  {
    std::ofstream file;
    file.open("E.txt");
    for(unsigned i=0; i<matrix.size(); i++)
      {
	file<<std::to_string(matrix[i]+matrix2[i])+'\n';
      }
    file.close();
  }

  void print3(const unsigned size)
  {
    string m="";
    std::ofstream file;
    file.open("mu.txt");
    const double delta=(double)2/size;
    double mu=-1+delta;

    while(mu<=1)
      {
	file<<std::to_string(mu)+'\n';
	mu+=delta;
      }
    file.close();
  }

  void print4(const unsigned size, const double delta)
  {
    string m="";
    std::ofstream file;
    file.open("E.txt");
    double mu=0;

    while(mu<=1)
      {
	file<<std::to_string(mu)+'\n';
	mu+=delta;
      }
    file.close();
  }

  void saveFile(double from, const double to, const double delta, const string &file)
  {
    std::ofstream file1;
    file1.open(file);

    while(from<=to)
      {
	file1<<from;
	file1<<'\n';
	from+=delta;
      }
  }

  bool CheckConvergence(const vector<vector<double>> &resultNew, const vector<vector<double>> &resultOld, const double tolerance)
  {
    vector<double> rowNew;
    vector<double> rowOld;
    double delta=0;
    // cout<<"CHECK"<<'\n';
    double largest=0;

    for(unsigned i=0; i<resultNew.size(); i++)
      {
	rowNew=resultNew[i];
	rowOld=resultOld[i];


	for(unsigned j=0; j<rowNew.size(); j++)
	  {
	    delta=fabs(rowNew[j]-rowOld[j]);


	    if(delta>largest) largest=delta;

	    // cout<<delta<<'\n';
	    /* if(rowOld[j]==0 or rowNew[j]==0) continue; */
	    /* cout<<delta<<'\n'; */

	    /* if(delta>tolerance and rowNew[j]!=0 and rowOld[j]!=0 and (rowNew[j]==0 or rowOld[j]==0)) return 0; */
	    /* if(delta>tolerance and rowNew[j]!=0 and rowOld[j]!=0) return 0; */
	  }
      }
    cout<<largest<<" tolerance "<<tolerance<<'\n';
    return largest<tolerance;
  }

  // void GS(Matrix &m, vector<double> &gridE, vector<double> &E)
  // {
  //   const unsigned size=50;
  //   vector<vector<double>> result;
  //   vector<vector<double>> resultOld;
  //   vector<double> row;
  //   vector<double> row2;
  //   result.reserve(size);
  //   row.reserve(size);
  //   double h=0.02;


  //   for(unsigned i=0; i<size; i++)
  //     {
  // 	for(unsigned j=0; j<size; j++)
  // 	  {
  // 	    row.emplace_back(0);
  // 	    /* row.emplace_back(j+1); */
  // 	  }
  // 	result.emplace_back(row);
  // 	row.clear();
  //     }


  //   vector<double> row3;
  //   vector<vector<double>> g;
  //   std::ofstream filex;
  //   filex.open("x.txt");
  //   std::ofstream filey;
  //   filey.open("y.txt");

  //   for(unsigned i=0; i<size; i++)
  //     {
  // 	filex<<i*h;
  // 	filex<<'\n';
  // 	filey<<i*h;
  // 	filey<<'\n';
  // 	for(unsigned j=0; j<size; j++)
  // 	  {
  // 	    row3.emplace_back(sin(i*h)*cos(h*j));
  // 	  }
  // 	g.emplace_back(row);
  // 	row3.clear();
  //     }
  //   filex.close();
  //   filey.close();
  //   print2(gridE, E);
  //   print3(size);

  //   cout<<"AOE "<<m.N<<'\n';


  //   bool convergence=0;

  //   while(!convergence)
  //     {
  // 	resultOld=result;
  // 	/* cout<<"ao"<<'\n'; */

  // 	for(unsigned i=1; i<size-1; i++)
  // 	  {
  // 	    row3=g[i];

  // 	    for(unsigned j=1; j<size-1; j++)
  // 	      {
  // 		result[i][j]=0.25*(result[i+1][j+1]+result[i+1][j-1]+result[i-1][j+1]
  // 				   +result[i-1][j-1]-4*h*h*row3[j]);

  // 	      }
  // 	  }
  // 	convergence=CheckConvergence(result, resultOld, 1e-6);
  // 	cout<<"convergence "<<convergence<<'\n';
  //     }




  //   print(result);
  // }

  void GS(Matrix &m, vector<double> &gridE, vector<double> &E)
  {
    const unsigned size=m.N-1;
    const double deltaMu=(double) 2/size;
    const double deltaE=(double) (E.back()-E[0])/size;
    vector<vector<double>> result;
    vector<vector<double>> resultOld;
    vector<double> row;
    vector<double> row2;
    result.reserve(size);
    row.reserve(size);

    // print2(gridE, E);
    // // print4(size, deltaE);
    // print3(size);

    saveFile(deltaE, E.back(), deltaE, "E.txt");
    saveFile(-1+deltaMu, 1, deltaMu, "mu.txt");

    for(unsigned i=0; i<size; i++)
      {
	for(unsigned j=0; j<size; j++)
	  {
	    row.emplace_back(0);
	  }
	result.emplace_back(row);
	row.clear();
      }

    cout<<"size "<<m.N<<" dE"<<deltaE<<" dmu"<<deltaMu<<'\n';


    bool convergence=0;

    while(!convergence)
      {
     	resultOld=result;
	// cout<<"ao"<<'\n';

     	for(unsigned i=1; i<size-1; i++)
     	  {
     	    row2=m.GetRow(i);

     	    for(unsigned j=1; j<size-1; j++)
     	      {
		// cout<<gridE[j]<<'\n';
		result[i][j]=0.25*(result[i+1][j+1]+result[i+1][j-1]+result[i-1][j+1]
				   // +result[i-1][j-1]-4*gridE[j]*deltaMu*row2[j]);
				   +result[i-1][j-1]-4*deltaE*deltaMu*row2[j]);

		/////////////////////////////////////////////////////////////////////////////


		// result[i][j]=0.25*(result[i+1][j+1]-result[i+1][j-1]-result[i-1][j+1]
		// 		   -result[i-1][j-1]-4*deltaE*deltaMu*row2[j]);

		// result[i][j]=result[i+1][j+1]-result[i+1][j-1]-result[i-1][j+1]
		//   -result[i-1][j-1]-4*deltaE*deltaMu*row2[j];

     	      }
     	  }
     	convergence=CheckConvergence(result, resultOld, 5e-22);
     	cout<<"convergence "<<convergence<<'\n';
      }


     // for(unsigned i=1; i<size-1; i++) *\/
     //   { *\/
     // 	row2=m.GetRow(i); *\/

     // 	for(unsigned j=1; j<size-1; j++) *\/
     // 	  { *\/
     // 	     result[i][j]=result[i+1][j+1]+result[i+1][j-1]+result[i-1][j+1] *\\/ *\/
     // 	       +result[i-1][j-1]-4*deltaE*deltaMu*row2[j]; *\\/ *\/

     // 	     result[i][j]=result[i+1][j+1]+result[i+1][j-1]+result[i-1][j+1] *\\/ *\/
     // 	       +result[i-1][j-1]+4*deltaE*deltaMu*row2[j]; *\\/ *\/


     // 	     result[i][j]=result[i+1][j+1]-result[i+1][j-1]-result[i-1][j+1] *\\/ *\/
     // 	       +result[i-1][j-1]-4*deltaE*deltaMu*row2[j]; *\\/ // wrong *\/



     // 	     result[i][j]=-result[i+1][j+1]+result[i+1][j-1]+result[i-1][j+1] *\\/ *\/
     // 	       +result[i-1][j-1]-4*deltaE*deltaMu*row2[j]; *\\/ *\/


     // 	     result[i][j]=-result[i+1][j+1]+result[i+1][j-1]+result[i-1][j+1] *\\/ *\/
     // 	       +result[i-1][j-1]+4*deltaE*deltaMu*row2[j]; *\\/ *\/



     // 	     result[i][j]=result[i+1][j+1]+result[i+1][j-1]+result[i-1][j+1] *\\/ *\/
     // 	       +result[i-1][j-1]-4*deltaE*deltaMu*row2[j]; *\\/ *\/
     // 	     /\\\* result[i][j]=result[i][j]/4; *\\\/ *\\/ *\/


     // 	     result[i][j]=result[i+1][j-1]+result[i-1][j+1] *\\/ *\/
     // 	       +result[i-1][j-1]+4*deltaE*deltaMu*row2[j]; *\\/ *\/
     // 	     result[i][j]=result[i][j]/4; *\\/ *\/


     // 	     result[i][j]=-result[i+1][j-1]-result[i-1][j+1] *\\/ *\/
     // 	       -result[i-1][j-1]-4*deltaE*deltaMu*row2[j]; *\\/ *\/
     // 	     result[i][j]=result[i][j]/4; *\\/ *\/



     // 	    result[i][j]=0.25*(result[i+1][j+1]+result[i+1][j-1]+result[i-1][j+1] *\/
     // 			       +result[i-1][j-1]-4*deltaE*deltaMu*row2[j]); *\/



     // 	     result[i][j]=result[i+1][j+1]-result[i+1][j-1]-result[i-1][j+1] *\\/ *\/
     // 	       -result[i-1][j-1]-4*deltaE*deltaMu*row2[j]; *\\/ *\/
     // 	  } *\/
     //   } *\/
    print(result);
  }
public:
  GaussSeidel(){}
  GaussSeidel(Matrix &m, vector<double> &gridE, vector<double> &E)
  {
    GS(m, gridE, E);
  }
  ~GaussSeidel(){}
};

#endif
