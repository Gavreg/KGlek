#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <cmath>

#include "..\cppheaders\vector3d.h"
#include "..\cppheaders\splines.h"





template<int _n, int _m>
struct BezierSurf
{
    std::vector<std::vector<vector3d>> P;

    double B(int n, int i, double t)
    {
        return  1.0*faktorial(n)/faktorial(i)/faktorial(n-i)*pow(t,i)*pow(1-t,n-i);
    }

    vector3d operator()(double u, double v)
    {
        vector3d r;
        for (int i=0; i<=_n; ++i)
            for (int j=0; j<=_m; ++j)
            {
                r = r+B(_n,i,u)*B(_m,j,v)*P[i][j];
            }
        return r;
    }
};

void print(std::string &filename, std::vector<std::vector<vector3d>> &points)
{
    std::ofstream fout;

    fout.open(filename,std::ios::trunc);

    char buf[1024];

    for(int i=1; i<points.size();++i)
    {
        for (int j=1; j<points[i].size(); ++j)
        {
            //int size=sprintf(buf, "\\fill (%f,%f,%f) circle(0.5pt);", points[i][j].x,points[i][j].y,points[i][j].z);
            sprintf(buf,"\\draw[very thin] (%f,%f,%f) -- (%f,%f,%f) -- (%f,%f,%f);", 
               points[i-1][j].x,points[i-1][j].y,points[i-1][j].z,
               points[i][j].x,points[i][j].y,points[i][j].z,
               points[i][j-1].x,points[i][j-1].y,points[i][j-1].z  );

            std::cout<<buf<<std::endl;
            fout<<buf<<std::endl;

            if (i==1)
            {
               sprintf(buf,"\\draw[very thin] (%f,%f,%f) -- (%f,%f,%f) ;", 
               points[i-1][j-1].x,points[i-1][j-1].y,points[i-1][j-1].z,
               points[i-1][j].x,points[i-1][j].y,points[i-1][j].z);

                std::cout<<buf<<std::endl;
                fout<<buf<<std::endl;
            }

            if (j==1)
            {
               sprintf(buf,"\\draw[very thin] (%f,%f,%f) -- (%f,%f,%f) ;", 
               points[i-1][j-1].x,points[i-1][j-1].y,points[i-1][j-1].z,
               points[i][j-1].x,points[i][j-1].y,points[i][j-1].z);

                std::cout<<buf<<std::endl;
                fout<<buf<<std::endl;
            }

        }
    }


    fout.close();

}

void print_skelet(std::string &filename, std::vector<std::vector<vector3d>> &points)
{
    std::ofstream fout;

    fout.open(filename,std::ios::trunc);

    char buf[1024];

    for(int i=1; i<points.size();++i)
    {
        for (int j=1; j<points[i].size(); ++j)
        {
            //int size=sprintf(buf, "\\fill (%f,%f,%f) circle(0.5pt);", points[i][j].x,points[i][j].y,points[i][j].z);
            sprintf(buf,"\\draw[Magenta] (%f,%f,%f) -- (%f,%f,%f) -- (%f,%f,%f);", 
               points[i-1][j].x,    points[i-1][j].y,   points[i-1][j].z,
               points[i][j].x,      points[i][j].y,     points[i][j].z,
               points[i][j-1].x,    points[i][j-1].y,   points[i][j-1].z  );

            std::cout<<buf<<std::endl;
            fout<<buf<<std::endl;

            if (i==1)
            {
               sprintf(buf,"\\draw[Magenta] (%f,%f,%f) -- (%f,%f,%f) ;", 
               points[i-1][j-1].x,  points[i-1][j-1].y, points[i-1][j-1].z,
               points[i-1][j].x,    points[i-1][j].y,   points[i-1][j].z);

                std::cout<<buf<<std::endl;
                fout<<buf<<std::endl;
            }

            if (j==1)
            {
               sprintf(buf,"\\draw[Magenta] (%f,%f,%f) -- (%f,%f,%f) ;", 
               points[i-1][j-1].x,  points[i-1][j-1].y, points[i-1][j-1].z,
               points[i][j-1].x,    points[i][j-1].y,   points[i][j-1].z);

                std::cout<<buf<<std::endl;
                fout<<buf<<std::endl;
            }

        }
    }

    fout.close();
}

int main ()
{
    double h, h_u, h_v;
    BezierSurf<3,3> b;

    b.P={ 
            {{0,0,0} ,  {1,0,0} ,  {2,0,0} ,  {3,0,0}} , 
            {{0,1,0} ,  {1,1,2} ,  {2,1,0} ,  {3,1,0}} , 
            {{0,2,0} ,  {1,2,0} ,  {2,2,-1} ,  {3,2,0}} , 
            {{0,3,0} ,  {1,3,0} ,  {2,3,0} ,  {3,3,0}} , 

    };

    std::vector<std::vector<vector3d>> points;


    for (double u=0; u<=1.01; u+=0.05)
        {
            points.emplace_back();
            for (double v=0; v<=1.01; v+=0.05)
                points.back().push_back(b(u,v));
        }


    print(std::string("bsurfpoints.tex"),points);
    print_skelet(std::string("bsurfpoints_p.tex"),b.P);

    NurbsSurf<vector3d> nurbssurf;
    nurbssurf.p = {
            {{-2,-2,0}, {-1,-2,0}, {0,-2,0},  {1,-2,0},  {2,-2,0}}, 
            {{-2,-1,0}, {-1,-1,0}, {0,-1,3},  {1,-1,0},  {2,-1,0}}, 
            {{-2,0,0},  {-1,0,0},  {0,0, 0},  {1,0,0},   {2,0,0}}, 
            {{-2,1,0},  {-1,1,-2},  {0,1,0},   {1,1,0},   {2,1,-1}}, 
            {{-2,2,0},  {-1,2,0},  {0,2,0},   {1,2,0},   {2,2,-1}}, 
    };
    nurbssurf.w = {
        {1,1,1,1,1},
        {1,1,1,1,1},
        {1,1,1,1,1},
        {1,1,1,1,1},
        {1,1,1,1,1}
    };

    nurbssurf.U={0,0,0,0,0,1,1,1,1,1};
    nurbssurf.V={0,0,0,0,0,1,1,1,1,1};

    points.clear();
    h_u = (nurbssurf.U.back() - nurbssurf.U.front())/40;
    h_v = (nurbssurf.V.back() - nurbssurf.V.front())/40;

    for (int i = 0; i<= 40 ; ++i)
    {
            points.emplace_back();
            double u = i*h_u;
            u = u > nurbssurf.U.back() ? nurbssurf.U.back()  : u;
            for (int j = 0; j<=40; ++j)
            {
                double v = h_v * j;
                v = v > nurbssurf.V.back() ? nurbssurf.V.back() :  v;
                points.back().push_back(nurbssurf(u,v,5,5));
            }
                
    }

    print(std::string("nurbs_surf_order5_points.tex"),points);
    print_skelet(std::string("nurbs_surf_order5_skelet.tex"),nurbssurf.p);



    nurbssurf.U={0,0,0,0,1,2,2,2,2};
    nurbssurf.V={0,0,0,0,1,2,2,2,2};

    points.clear();
    h_u = (nurbssurf.U.back() - nurbssurf.U.front())/40;
    h_v = (nurbssurf.V.back() - nurbssurf.V.front())/40;

    for (int i = 0; i<= 40 ; ++i)
    {
            points.emplace_back();
            double u = i*h_u;
            u = u > nurbssurf.U.back() ? nurbssurf.U.back()  : u;
            for (int j = 0; j<=40; ++j)
            {
                double v = h_v * j;
                v = v > nurbssurf.V.back() ? nurbssurf.V.back() :  v;
                points.back().push_back(nurbssurf(u,v,4,4));
            }
                
    }

    print(std::string("nurbs_surf_order4_points.tex"),points);
    print_skelet(std::string("nurbs_surf_order4_skelet.tex"),nurbssurf.p);



    nurbssurf.U={0,0,0,1,2,3,3,3};
    nurbssurf.V={0,0,0,1,2,3,3,3};

    points.clear();
    h_u = (nurbssurf.U.back() - nurbssurf.U.front())/40;
    h_v = (nurbssurf.V.back() - nurbssurf.V.front())/40;

    for (int i = 0; i<= 40 ; ++i)
    {
            points.emplace_back();
            double u = i*h_u;
            u = u > nurbssurf.U.back() ? nurbssurf.U.back()  : u;
            for (int j = 0; j<=40; ++j)
            {
                double v = h_v * j;
                v = v > nurbssurf.V.back() ? nurbssurf.V.back() :  v;
                points.back().push_back(nurbssurf(u,v,3,3));
            }
                
    }

    print(std::string("nurbs_surf_order3_points.tex"),points);
    print_skelet(std::string("nurbs_surf_order3_skelet.tex"),nurbssurf.p);
    
 
    nurbssurf.U={0,0,1,2,3,4,4};
    nurbssurf.V={0,0,1,2,3,4,4};

    points.clear();
    h_u = (nurbssurf.U.back() - nurbssurf.U.front())/40;
    h_v = (nurbssurf.V.back() - nurbssurf.V.front())/40;

    for (int i = 0; i<= 40 ; ++i)
    {
            points.emplace_back();
            double u = i*h_u;
            u = u > nurbssurf.U.back() ? nurbssurf.U.back()  : u;
            for (int j = 0; j<=40; ++j)
            {
                double v = h_v * j;
                v = v > nurbssurf.V.back() ? nurbssurf.V.back() :  v;
                points.back().push_back(nurbssurf(u,v,2,2));
            }
                
    }

    print(std::string("nurbs_surf_order2_points.tex"),points);
    print_skelet(std::string("nurbs_surf_order2_skelet.tex"),nurbssurf.p);




    system("mkdir nurbs_surf_order3_anim");
    system("del /q nurbs_surf_order3_anim\\*");

    nurbssurf.U={0,0,0,1,2,3,3,3};
    nurbssurf.V={0,0,0,1,2,3,3,3};


    for (int frame=0; frame<72; ++frame)
    {    
        nurbssurf.w = {
            {1,1,1,1,1},
            {1,1,5.0*frame*frame/71/71,1,1},
            {1,1,1,1,1},
            {1,1,1,1,1},
            {1,1,1,1,1}
        };

        points.clear();
        h_u = (nurbssurf.U.back() - nurbssurf.U.front())/40;
        h_v = (nurbssurf.V.back() - nurbssurf.V.front())/40;

        for (int i = 0; i<= 40 ; ++i)
        {
                points.emplace_back();
                double u = i*h_u;
                u = u > nurbssurf.U.back() ? nurbssurf.U.back()  : u;
                for (int j = 0; j<=40; ++j)
                {
                    double v = h_v * j;
                    v = v > nurbssurf.V.back() ? nurbssurf.V.back() :  v;
                    points.back().push_back(nurbssurf(u,v,3,3));
                }
                    
        }

        print(std::string("nurbs_surf_order3_anim//nurbs_surf_order3_points_frame")+std::to_string(frame)+std::string(".tex"),points);
        print_skelet(std::string("nurbs_surf_order3_anim//nurbs_surf_order3_skelet_frame")+std::to_string(frame)+std::string(".tex"),nurbssurf.p);
    }


    nurbssurf.w = {
        {1,1,1,1,1},
        {1,1,1,1,1},
        {1,1,1,1,1},
        {1,1,1,1,1},
        {1,1,1,1,1}
    };
    nurbssurf.U={0,0,0,1,2,3,3,3};
    nurbssurf.V={0,0,1,2,3,4,4};

    points.clear();
    h_u = (nurbssurf.U.back() - nurbssurf.U.front())/40;
    h_v = (nurbssurf.V.back() - nurbssurf.V.front())/40;

    for (int i = 0; i<= 40 ; ++i)
    {
            points.emplace_back();
            double u = i*h_u;
            u = u > nurbssurf.U.back() ? nurbssurf.U.back()  : u;
            for (int j = 0; j<=40; ++j)
            {
                double v = h_v * j;
                v = v > nurbssurf.V.back() ? nurbssurf.V.back() :  v;
                points.back().push_back(nurbssurf(u,v,3,2));
            }
                
    }

    print(std::string("nurbs_surf_order2-3_points.tex"),points);
    print_skelet(std::string("nurbs_surf_order2-3_skelet.tex"),nurbssurf.p);
    
}