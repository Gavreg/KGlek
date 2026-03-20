#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <array>
#include <algorithm>
#include <array>
#include <format>

#include "..\cppheaders\vector2d.h"
#include "..\cppheaders\splines.h"


int main()
{
    

    
    std::ofstream fout;

    std::vector<vector2d> curve;
    
    //NURBS n4 k 3


    Nurbs<vector2d> n;
    n.p = {{2, 1}, {1, 4}, {5, 5}, {3, 3}, {5, 1}};

     //статичные примеры NURBS с разными весами
    fout.open("points.dat", std::ios::out);
    for (auto x : n.p)
    {
        fout << x << std::endl;
    }
    fout.close();
   
    for (auto k:{2,3,4})
    for (auto h:{3.0,1.0,0.75,0.0})
    {
        fout.open(std::format("nurbs_n4_k{}_h{}.dat",k,h), std::ios::out);
        n.h = {1,1,h,1,1};
        n.x = makeKnotsOpen(k,4);
        n.getCurve(k,100,curve);
        for (auto &p: curve)
        {
            fout << p << std::endl;
        }
        curve.clear();
        fout.close();
    }


     //NURBS  animation frames

    for (int k:{2,3,4})
    {
        system(std::format("mkdir nurbs_n4_k{}_anim",k).c_str());
        system(std::format("del /q nurbs_n4_k{}_anim\\*",k).c_str());
    
        for (int fr=0; fr<100; fr++)
        {    
            fout.open(std::format("nurbs_n4_k{}_anim\\nurbs_n4_k{}_frame{}.dat",k,k,fr), std::ios::trunc);
            n.h = {1,1,5.0*fr*fr/99/99,1,1};
            n.x = makeKnotsOpen(k,4);
            n.getCurve(k,100,curve);
            for (auto &p:curve)
                fout<<p<<std::endl;
            curve.clear();
            fout.close();
            
            for (int i=0; i<=4; ++i)
            {
                fout.open(std::format("nurbs_n4_k{}_anim\\nurbs_n4_k{}_N{}_frame{}.dat",k,k,i,fr),std::ios::trunc);
                std::vector<vector2d> N_points;
                n.getN(i,k,50,N_points);
                for(auto &p:N_points)
                    fout<<p<<std::endl;
                fout.close();
            }
        }
    } 




}