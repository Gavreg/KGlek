#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <array>
#include <algorithm>
#include <array>

#include "..\cppheaders\vector2d.h"
#include "..\cppheaders\splines.h"


int main()
{
    std::ofstream fout;
    
    //NURBS n4 k 3

    Nurbs<vector2d> n;
    n.p = {{2, 1}, {1, 4}, {5, 5}, {3, 3}, {5, 1}};


    fout.open("points.dat", std::ios::trunc);
    for (auto x : n.p)
    {
        fout << x << std::endl;
    }

    fout.close();

    n.x = {0,0,0,1,2,3,3,3};


    fout.open("nurbs_n4_k3_h3.dat", std::ios::trunc);
    n.h = {1,1,3,1,1};
    n.x = {0, 0, 0, 1, 2, 3, 3, 3};
    for (double t = *n.x.begin(); t <= *(n.x.end() - 1); t += 0.01)
    {
        fout << n(3, t) << std::endl;
    }
    fout.close();



    fout.open("nurbs_n4_k3_h1.dat", std::ios::trunc);
    n.h = {1,1,1,1,1};
    n.x = {0, 0, 0, 1, 2, 3, 3, 3};
    for (double t = *n.x.begin(); t <= *(n.x.end() - 1); t += 0.01)
    {
        fout << n(3, t) << std::endl;
    }
    fout.close();

    fout.open("nurbs_n4_k3_h0.75.dat", std::ios::trunc);
    n.h = {1,1,0.75,1,1};
    n.x = {0, 0, 0, 1, 2, 3, 3, 3};
    for (double t = *n.x.begin(); t <= *(n.x.end() - 1); t += 0.01)
    {
        fout << n(3, t) << std::endl;
    }
    fout.close();

    fout.open("nurbs_n4_k3_h0.dat", std::ios::trunc);
    n.h = {1,1,0,1,1};
    n.x = {0, 0, 0, 1, 2, 3, 3, 3};
    for (double t = *n.x.begin(); t <= *(n.x.end() - 1); t += 0.01)
    {
        fout << n(3, t) << std::endl;
    }
    fout.close();

    //NURBS n4 k 4



    fout.open("nurbs_n4_k4_h3.dat", std::ios::trunc);
    n.h = {1,1,3,1,1};
    n.x = {0,0,0,0,1,2,2,2,2};
    for (double t = *n.x.begin(); t <= *(n.x.end() - 1); t += 0.01)
    {
        fout << n(4, t) << std::endl;
    }
    fout.close();



    fout.open("nurbs_n4_k4_h1.dat", std::ios::trunc);
    n.h = {1,1,1,1,1};
    
    for (double t = *n.x.begin(); t <= *(n.x.end() - 1); t += 0.01)
    {
        fout << n(3, t) << std::endl;
    }
    fout.close();

    fout.open("nurbs_n4_k4_h0.75.dat", std::ios::trunc);
    n.h = {1,1,0.75,1,1};
    for (double t = *n.x.begin(); t <= *(n.x.end() - 1); t += 0.01)
    {
        fout << n(4, t) << std::endl;
    }
    fout.close();

    fout.open("nurbs_n4_k4_h0.dat", std::ios::trunc);
    n.h = {1,1,0,1,1};
    for (double t = *n.x.begin(); t <= *(n.x.end() - 1); t += 0.01)
    {
        fout << n(4, t) << std::endl;
    }
    fout.close();


     //NURBS n4 k 4 animation frames

    system("mkdir nurbs_n4_k4_anim");
    system("del /q nurbs_n4_k4_anim\\*");

    for (int fr=0; fr<100; fr++)
    {
        std::stringstream ss;

        ss<<"nurbs_n4_k4_anim\\nurbs_n4_k4_frame"<<fr<<".dat";

        fout.open(ss.str(), std::ios::trunc);
        n.h = {1,1,5.0*fr*fr/99/99,1,1};
        for (double t = *n.x.begin(); t <= *(n.x.end() - 1); t += 0.01)
        {
            fout << n(4, t) << std::endl;
        }
        fout.close();
        
        for (int i=0; i<=4; ++i)
        {
            std::stringstream ss;
            ss<<"nurbs_n4_k4_anim\\nurbs_n4_k4_N"<<i<<"_"<<"frame"<<fr<<".dat";
            fout.open(ss.str(),std::ios::trunc);
            double h = (n.x.back() - n.x.front())/49;
            for (int j=0; j<=49; ++j)
            {
                double t = j*h;
                if (t>n.x.back()) t = n.x.back();
                fout<<t<<" "<<n.N(i,4,t)<<std::endl;
            }
            fout.close();
        }
    }

     //NURBS n4 k 3 animation frames  

    system("mkdir nurbs_n4_k3_anim");
    system("del /q nurbs_n4_k3_anim\\*");
    
    n.x = {0, 0, 0, 1, 2, 3, 3, 3};
    for (int fr=0; fr<100; fr++)
    {
        std::stringstream ss;

        ss<<"nurbs_n4_k3_anim\\nurbs_n4_k3_frame"<<fr<<".dat";

        fout.open(ss.str(), std::ios::trunc);
        n.h = {1,1,5.0*fr/99*fr/99,1,1};
        for (double t = *n.x.begin(); t <= *(n.x.end() - 1); t += 0.01)
        {
            fout << n(3, t) << std::endl;
        }
        fout.close();

        for (int i=0; i<=4; ++i)
        {
            std::stringstream ss;
            ss<<"nurbs_n4_k3_anim\\nurbs_n4_k3_N"<<i<<"_"<<"frame"<<fr<<".dat";
            fout.open(ss.str(),std::ios::trunc);
            double h = (n.x.back() - n.x.front())/49;
            for (int j=0; j<=49; ++j)
            {
                double t = j*h;
                if (t>n.x.back()) t = n.x.back();
                fout<<t<<" "<<n.N(i,3,t)<<std::endl;
            }
            fout.close();
        }


    }



    //NURBS n4 k 5


    n.x = {0,0,0,0,0,1,1,1,1,1};


    fout.open("nurbs_n4_k5_h3.dat", std::ios::trunc);
    
    n.h = {1,1,3,1,1};
    for (double t = *n.x.begin(); t <= *(n.x.end() - 1); t += 0.01)
    {
        fout << n(5, t) << std::endl;
    }
    fout.close();


    fout.open("nurbs_n4_k5_h1.dat", std::ios::trunc);
    n.h = {1,1,1,1,1};
    for (double t = *n.x.begin(); t <= *(n.x.end() - 1); t += 0.01)
    {
        fout << n(5, t) << std::endl;
    }
    fout.close();

    fout.open("nurbs_n4_k5_h0.75.dat", std::ios::trunc);
    n.h = {1,1,0.75,1,1};
    for (double t = *n.x.begin(); t <= *(n.x.end() - 1); t += 0.01)
    {
        fout << n(5, t) << std::endl;
    }
    fout.close();

    fout.open("nurbs_n4_k5_h0.dat", std::ios::trunc);
    n.h = {1,1,0,1,1};
    for (double t = *n.x.begin(); t <= *(n.x.end() - 1); t += 0.01)
    {
        fout << n(5, t) << std::endl;
    }
    fout.close();

}