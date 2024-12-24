#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <array>
#include <algorithm>
#include <array>
#include <set>

#include "..\cppheaders\vector2d.h"
#include "..\cppheaders\splines.h"

template <class T>
void print(const char *filename, std::vector<T> points)
{
    std::ofstream fout;

    fout.open(filename, std::ios::trunc);

    for (auto &x : points)
    {
        fout << x << std::endl;
    }

    fout.close();
}

template <class T>
inline void print(const std::string &filename, std::vector<T> points)
{
    print<T>(filename.data(),points);
}

void printTikzCoords(const char * filename, std::vector<vector2d> points, const char *pointname = "P")
{
    std::ofstream fout;
    fout.open(filename, std::ios::app);

    int i = 0;
    for (auto &x : points)
    {
        char buf[256];
        sprintf(buf, "\\coordinate (%s%d) at (%f,%f);", pointname, i++, x.x, x.y);
        fout << buf << std::endl;
    }

    fout.close();
}

inline void printTikzCoords(const std::string &filename, std::vector<vector2d> points, const char *pointname = "P")
{
    printTikzCoords(filename.data(),points,pointname);
}

int main()
{

    std::ofstream fout;

    //Биспрайны
    BSpline<vector2d> b;
    b.p = {{2, 1}, {1, 4}, {5, 5}, {3, 3}, {5, 0}, {1, 1}};

    printTikzCoords("b_spline_tikz_points.dat", b.p);

    std::vector<vector2d> curve;
    for (int k = 5; k >= 1; --k)
    {
        std::stringstream ss;
        ss << "b_spline_k" << k << "_points.dat";

        b.x = makeKnotsOpen(k, 6);
        curve.clear();
        b.getCurve(k, 300, curve);
        print(ss.str().data(), curve);

        for (int i = 0; i <= 5; ++i)
        {
            curve.clear();
            b.getN(i, k, 300, curve);
            std::stringstream ss;
            ss << "N" << i << k << ".dat";
            print(ss.str().data(), curve);
        }
    }

    //анимация бисплайна второй степени
    system("mkdir build_bspline_k1_anim");
    system("del /q build_bspline_k1_anim\\*");
    {
        std::string dir = "build_bspline_k1_anim\\";
        std::stringstream ss;
        int k=1;
        b.x = makeKnotsOpen(k,6);
        curve.clear();

        b.getCurve(k,100,curve);
     

        ss<<dir<<"points_tikz.dat";
        printTikzCoords(ss.str(),b.p);

        for (int fr = 0; fr <=99; ++fr)
        {
            std::vector<vector2d> cur_curve;
            for (int i=0; i<=fr; ++i)
            {
                cur_curve.push_back(curve[i]);
            }

            ss.str("");
            ss<<dir<<"frame"<<fr<<".dat";
            print(ss.str(),cur_curve);

            std::vector<vector2d> lp {cur_curve.back()};
            ss.str("");
            ss<<dir<<"lp_tikz_frame"<<fr<<".dat";
            printTikzCoords(ss.str(),lp,"LP");

            double h = (b.x.back()-b.x.front())/99;
            double t = b.x.front() + h*fr;
            if (t>b.x.back()) t = b.x.back();

            std::set<double> t_set;
            for (auto _t : b.x)
            {
                if (_t<=t)
                    t_set.insert(_t);
            }

            ss.str("");
            ss<<dir<<"tpoints_latex_frame"<<fr<<".dat";
            fout.open(ss.str(),std::ios::out);
            fout<<"\\def\\tarr{";
            for (auto _t :t_set)
            {
               auto p = b(k,_t);
               fout<<"("<<p.x<<", "<<p.y<<"), ";
            }
            fout.seekp(-2, std::ios::end);
            fout<<"} ";
            fout.close();

            char buf[128];
            sprintf(buf,"\\edef\\tval{%f}",t);
            ss.str("");
            ss<<dir<<"t_tex_frame"<<fr<<".dat";
            fout.open(ss.str(),std::ios::app);
            fout<<buf<<std::endl;
            fout.close();
        }
    }

    //анимация бисплайна второй степени
    system("mkdir build_bspline_k2_anim");
    system("del /q build_bspline_k2_anim\\*");
    {
        std::string dir = "build_bspline_k2_anim\\";
        std::stringstream ss;
        int k=2;
        b.x = makeKnotsOpen(k,6);
        curve.clear();

        b.getCurve(k,100,curve);
     

        ss<<dir<<"points_tikz.dat";
        printTikzCoords(ss.str(),b.p);

        for (int fr = 0; fr <=99; ++fr)
        {
            std::vector<vector2d> cur_curve;
            for (int i=0; i<=fr; ++i)
            {
                cur_curve.push_back(curve[i]);
            }

            ss.str("");
            ss<<dir<<"frame"<<fr<<".dat";
            print(ss.str(),cur_curve);

            std::vector<vector2d> lp {cur_curve.back()};
            ss.str("");
            ss<<dir<<"lp_tikz_frame"<<fr<<".dat";
            printTikzCoords(ss.str(),lp,"LP");

            double h = (b.x.back()-b.x.front())/99;
            double t = b.x.front() + h*fr;
            if (t>b.x.back()) t = b.x.back();

            std::set<double> t_set;
            for (auto _t : b.x)
            {
                if (_t<=t)
                    t_set.insert(_t);
            }

            ss.str("");
            ss<<dir<<"tpoints_latex_frame"<<fr<<".dat";
            fout.open(ss.str(),std::ios::out);
            fout<<"\\def\\tarr{";
            for (auto _t :t_set)
            {
               auto p = b(k,_t);
               fout<<"("<<p.x<<", "<<p.y<<"), ";
            }
            fout.seekp(-2, std::ios::end);
            fout<<"} ";
            fout.close();

            char buf[128];
            sprintf(buf,"\\edef\\tval{%f}",t);
            ss.str("");
            ss<<dir<<"t_tex_frame"<<fr<<".dat";
            fout.open(ss.str(),std::ios::app);
            fout<<buf<<std::endl;
            fout.close();
        }
    }

    //анимация бисплайна второй степени
    system("mkdir build_bspline_k3_anim");
    system("del /q build_bspline_k3_anim\\*");
    {
        std::string dir = "build_bspline_k3_anim\\";
        std::stringstream ss;
        int k=3;
        b.x = makeKnotsOpen(k,6);
        curve.clear();

        b.getCurve(k,100,curve);
     

        ss<<dir<<"points_tikz.dat";
        printTikzCoords(ss.str(),b.p);

        for (int fr = 0; fr <=99; ++fr)
        {
            std::vector<vector2d> cur_curve;
            for (int i=0; i<=fr; ++i)
            {
                cur_curve.push_back(curve[i]);
            }

            ss.str("");
            ss<<dir<<"frame"<<fr<<".dat";
            print(ss.str(),cur_curve);

            std::vector<vector2d> lp {cur_curve.back()};
            ss.str("");
            ss<<dir<<"lp_tikz_frame"<<fr<<".dat";
            printTikzCoords(ss.str(),lp,"LP");

            double h = (b.x.back()-b.x.front())/99;
            double t = b.x.front() + h*fr;
            if (t>b.x.back()) t = b.x.back();

            std::set<double> t_set;
            for (auto _t : b.x)
            {
                if (_t<=t)
                    t_set.insert(_t);
            }

            ss.str("");
            ss<<dir<<"tpoints_latex_frame"<<fr<<".dat";
            fout.open(ss.str(),std::ios::out);
            fout<<"\\def\\tarr{";
            for (auto _t :t_set)
            {
               auto p = b(k,_t);
               fout<<"("<<p.x<<", "<<p.y<<"), ";
            }
            fout.seekp(-2, std::ios::end);
            fout<<"} ";
            fout.close();

            char buf[128];
            sprintf(buf,"\\edef\\tval{%f}",t);
            ss.str("");
            ss<<dir<<"t_tex_frame"<<fr<<".dat";
            fout.open(ss.str(),std::ios::app);
            fout<<buf<<std::endl;
            fout.close();
        }
    }

    //анимация бисплайна второй степени
    system("mkdir build_bspline_k4_anim");
    system("del /q build_bspline_k4_anim\\*");
    {
        std::string dir = "build_bspline_k4_anim\\";
        std::stringstream ss;
        int k=4;
        b.x = makeKnotsOpen(k,6);
        curve.clear();

        b.getCurve(k,100,curve);
     

        ss<<dir<<"points_tikz.dat";
        printTikzCoords(ss.str(),b.p);

        for (int fr = 0; fr <=99; ++fr)
        {
            std::vector<vector2d> cur_curve;
            for (int i=0; i<=fr; ++i)
            {
                cur_curve.push_back(curve[i]);
            }

            ss.str("");
            ss<<dir<<"frame"<<fr<<".dat";
            print(ss.str(),cur_curve);

            std::vector<vector2d> lp {cur_curve.back()};
            ss.str("");
            ss<<dir<<"lp_tikz_frame"<<fr<<".dat";
            printTikzCoords(ss.str(),lp,"LP");

            double h = (b.x.back()-b.x.front())/99;
            double t = b.x.front() + h*fr;
            if (t>b.x.back()) t = b.x.back();

            std::set<double> t_set;
            for (auto _t : b.x)
            {
                if (_t<=t)
                    t_set.insert(_t);
            }

            ss.str("");
            ss<<dir<<"tpoints_latex_frame"<<fr<<".dat";
            fout.open(ss.str(),std::ios::out);
            fout<<"\\def\\tarr{";
            for (auto _t :t_set)
            {
               auto p = b(k,_t);
               fout<<"("<<p.x<<", "<<p.y<<"), ";
            }
            fout.seekp(-2, std::ios::end);
            fout<<"} ";
            fout.close();

            char buf[128];
            sprintf(buf,"\\edef\\tval{%f}",t);
            ss.str("");
            ss<<dir<<"t_tex_frame"<<fr<<".dat";
            fout.open(ss.str(),std::ios::app);
            fout<<buf<<std::endl;
            fout.close();
        }
    }

    //анимация бисплайна второй степени
    system("mkdir build_bspline_k5_anim");
    system("del /q build_bspline_k5_anim\\*");
    {
        std::string dir = "build_bspline_k5_anim\\";
        std::stringstream ss;
        int k=5;
        b.x = makeKnotsOpen(k,6);
        curve.clear();

        b.getCurve(k,100,curve);
     

        ss<<dir<<"points_tikz.dat";
        printTikzCoords(ss.str(),b.p);

        for (int fr = 0; fr <=99; ++fr)
        {
            std::vector<vector2d> cur_curve;
            for (int i=0; i<=fr; ++i)
            {
                cur_curve.push_back(curve[i]);
            }

            ss.str("");
            ss<<dir<<"frame"<<fr<<".dat";
            print(ss.str(),cur_curve);

            std::vector<vector2d> lp {cur_curve.back()};
            ss.str("");
            ss<<dir<<"lp_tikz_frame"<<fr<<".dat";
            printTikzCoords(ss.str(),lp,"LP");

            double h = (b.x.back()-b.x.front())/99;
            double t = b.x.front() + h*fr;
            if (t>b.x.back()) t = b.x.back();

            std::set<double> t_set;
            for (auto _t : b.x)
            {
                if (_t<=t)
                    t_set.insert(_t);
            }

            ss.str("");
            ss<<dir<<"tpoints_latex_frame"<<fr<<".dat";
            fout.open(ss.str(),std::ios::out);
            fout<<"\\def\\tarr{";
            for (auto _t :t_set)
            {
               auto p = b(k,_t);
               fout<<"("<<p.x<<", "<<p.y<<"), ";
            }
            fout.seekp(-2, std::ios::end);
            fout<<"} ";
            fout.close();

            char buf[128];
            sprintf(buf,"\\edef\\tval{%f}",t);
            ss.str("");
            ss<<dir<<"t_tex_frame"<<fr<<".dat";
            fout.open(ss.str(),std::ios::app);
            fout<<buf<<std::endl;
            fout.close();
        }
    }
    



   

    //Безье
    curve.clear();
    Bezier<vector2d> bc;
    bc.p = {{0, 0}, {0.5, 2}, {3, 2.5}, {4, 0}};
    bc.getAllCurve(50, curve);
    print("bezier_example_1_curve.dat", curve);
    printTikzCoords("bezier_example_1_tikz.dat", bc.p);

    curve.clear();
    bc.p = {{0, 0}, {4, 3}, {5, -1}, {2, 0.5}};
    bc.getAllCurve(50, curve);
    print("bezier_example_2_curve.dat", curve);
    printTikzCoords("bezier_example_2_tikz.dat", bc.p);

    curve.clear();
    bc.p = {{0, 0}, {4, 1}, {-1, 2}, {3, 3}};
    bc.getAllCurve(50, curve);
    print("bezier_example_3_curve.dat", curve);
    printTikzCoords("bezier_example_3_tikz.dat", bc.p);

    // анимация построения кривой
    system("mkdir build_bezier_anim");
    system("del /q build_bezier_anim\\*");
    bc.p = {{0, 0}, {0.5, 2}, {3, 2.5}, {4, 0}};
    printTikzCoords("build_bezier_anim\\tikz.dat", bc.p);
    curve.clear();
    bc.getAllCurve(100, curve);
    {
        std::vector<vector2d> cur_curve;
        for (int fr = 0; fr <= 99; ++fr)
        {
            std::stringstream ss;
            ss << "build_bezier_anim\\frame" << fr << ".dat";
            cur_curve.push_back(curve[fr]);
            print(ss.str().data(), cur_curve);
            char buf[64];
            sprintf(buf, "\\coordinate (LP) at (%f,%f);", cur_curve.back().x, cur_curve.back().y);
            ss.str(std::string());
            ss << "build_bezier_anim\\last_point_tikz_frame" << fr << ".dat";
            fout.open(ss.str(), std::ios::trunc);
            fout << buf << std::endl;
            fout.close();
        }
    }

    system("mkdir build_bezier_anim_n2");
    system("del /q build_bezier_anim_n2\\*");
    bc.p = {{0, 0}, {4, 4}};
    printTikzCoords("build_bezier_anim_n2\\tikz.dat", bc.p);
    curve.clear();
    bc.getAllCurve(100, curve);
    {
        std::vector<vector2d> cur_curve;
        for (int fr = 0; fr <= 99; ++fr)
        {
            std::stringstream ss;
            ss << "build_bezier_anim_n2\\frame" << fr << ".dat";
            cur_curve.push_back(curve[fr]);
            print(ss.str().data(), cur_curve);
            char buf[64];
            sprintf(buf, "\\coordinate (LP) at (%f,%f);", cur_curve.back().x, cur_curve.back().y);
            ss.str(std::string());
            ss << "build_bezier_anim_n2\\last_point_tikz_frame" << fr << ".dat";
            fout.open(ss.str(), std::ios::trunc);
            fout << buf << std::endl;
            fout.close();
        }
    }

    system("mkdir build_bezier_anim_n3");
    system("del /q build_bezier_anim_n3\\*");
    bc.p = {{0, 0}, {2, 4}, {3.1}};
    printTikzCoords("build_bezier_anim_n3\\tikz.dat", bc.p);
    curve.clear();
    bc.getAllCurve(100, curve);
    {
        std::vector<vector2d> cur_curve;
        for (int fr = 0; fr <= 99; ++fr)
        {
            std::stringstream ss;
            ss << "build_bezier_anim_n3\\frame" << fr << ".dat";
            cur_curve.push_back(curve[fr]);
            print(ss.str().data(), cur_curve);
            char buf[64];
            sprintf(buf, "\\coordinate (LP) at (%f,%f);", cur_curve.back().x, cur_curve.back().y);
            ss.str(std::string());
            ss << "build_bezier_anim_n3\\last_point_tikz_frame" << fr << ".dat";
            fout.open(ss.str(), std::ios::trunc);
            fout << buf << std::endl;
            fout.close();
        }
    }

    system("mkdir build_bezier_anim_n4");
    system("del /q build_bezier_anim_n4\\*");
    bc.p = {{0, 0}, {4, 3}, {5, -1}, {2, 0.5}};
    printTikzCoords("build_bezier_anim_n4\\tikz.dat", bc.p);
    curve.clear();
    bc.getAllCurve(100, curve);
    {
        std::vector<vector2d> cur_curve;
        for (int fr = 0; fr <= 99; ++fr)
        {
            std::stringstream ss;
            ss << "build_bezier_anim_n4\\frame" << fr << ".dat";
            cur_curve.push_back(curve[fr]);
            print(ss.str().data(), cur_curve);
            char buf[64];
            sprintf(buf, "\\coordinate (LP) at (%f,%f);", cur_curve.back().x, cur_curve.back().y);
            ss.str(std::string());
            ss << "build_bezier_anim_n4\\last_point_tikz_frame" << fr << ".dat";
            fout.open(ss.str(), std::ios::trunc);
            fout << buf << std::endl;
            fout.close();
        }
    }

    system("mkdir build_bezier_anim_n6");
    system("del /q build_bezier_anim_n6\\*");
    bc.p = {{0, 0}, {5, 4}, {6, 1}, {5, 0}, {-2, 3}, {7, 5}};
    printTikzCoords("build_bezier_anim_n6\\tikz.dat", bc.p);
    curve.clear();
    bc.getAllCurve(100, curve);
    {
        std::vector<vector2d> cur_curve;
        for (int fr = 0; fr <= 99; ++fr)
        {
            std::stringstream ss;
            ss << "build_bezier_anim_n6\\frame" << fr << ".dat";
            cur_curve.push_back(curve[fr]);
            print(ss.str().data(), cur_curve);
            char buf[64];
            sprintf(buf, "\\coordinate (LP) at (%f,%f);", cur_curve.back().x, cur_curve.back().y);
            ss.str(std::string());
            ss << "build_bezier_anim_n6\\last_point_tikz_frame" << fr << ".dat";
            fout.open(ss.str(), std::ios::trunc);
            fout << buf << std::endl;
            fout.close();
        }
    }

    system("mkdir build_bezier_p0_anim_n6");
    system("del /q build_bezier_p0_anim_n6\\*");
    bc.p = {{0, 0}, {5, 4}, {6, 1}, {5, 0}, {-2, 3}, {7, 5}};
    printTikzCoords("build_bezier_p0_anim_n6\\tikz.dat", bc.p);
    curve.clear();
    bc.getAllCurve(100, curve);
    {

        for (int fr = 0; fr <= 99; ++fr)
        {
            std::vector<vector2d> cur_curve;
            std::stringstream ss;
            ss << "build_bezier_p0_anim_n6\\frame" << fr << ".dat";
            bc.p[0].y = 7.0 * fr / 99;
            bc.getAllCurve(50, cur_curve);

            print(ss.str().data(), cur_curve);
            ss.str("");
            ss << "build_bezier_p0_anim_n6\\tikz_frame" << fr << ".dat";
            printTikzCoords(ss.str().data(), bc.p);
        }
    }

    system("mkdir build_bezier_p3_anim_n6");
    system("del /q build_bezier_p3_anim_n6\\*");
    bc.p = {{0, 0}, {5, 4}, {6, 1}, {5, 0}, {-2, 3}, {7, 5}};
    printTikzCoords("build_bezier_p3_anim_n6\\tikz.dat", bc.p);
    curve.clear();
    bc.getAllCurve(100, curve);
    {

        for (int fr = 0; fr <= 99; ++fr)
        {
            std::vector<vector2d> cur_curve;
            std::stringstream ss;
            ss << "build_bezier_p3_anim_n6\\frame" << fr << ".dat";
            bc.p[3].x = 8.0 * fr / 99;
            bc.getAllCurve(50, cur_curve);

            print(ss.str().data(), cur_curve);
            ss.str("");
            ss << "build_bezier_p3_anim_n6\\tikz_frame" << fr << ".dat";
            printTikzCoords(ss.str().data(), bc.p);
        }
    }
    {
        system("mkdir recursive_bezier_anim_n3");
        system("del /q recursive_bezier_anim_n3\\*");
        bc.p = {{0, 0}, {2, 4}, {3.1}};
        curve.clear();
        bc.getAllCurve(200,curve);
        printTikzCoords("recursive_bezier_anim_n3\\tikz.dat", bc.p);

        double h=1.0/198;
        for (int frame=0; frame<=198; ++frame)
        {
            double t = h*frame;
            if(t>1) t=1;

            auto p1_1=bc.p[0]*(1-t)+bc.p[1]*t;
            auto p1_2=bc.p[1]*(1-t)+bc.p[2]*t;

            std::vector<vector2d> p;
            p.push_back(p1_1);
            p.push_back(p1_2);

            std::vector<vector2d> cur_curve;
            for(int i=0; i<=frame; ++i)
                cur_curve.push_back(curve[i]);
            std::vector<vector2d> lp{cur_curve.back()};

            std::stringstream ss;
            ss<<"recursive_bezier_anim_n3\\p_frame"<<frame<<".dat";
            printTikzCoords(ss.str().data(),p,"PP");

            ss.str("");
            ss<<"recursive_bezier_anim_n3\\lp_frame"<<frame<<".dat";
            printTikzCoords(ss.str().data(),lp,"LP");

            ss.str("");
            ss<<"recursive_bezier_anim_n3\\curve_frame"<<frame<<".dat";
            print(ss.str(),cur_curve);
        }
        



        
    }

    {
        system("mkdir recursive_bezier_anim_n4");
        system("del /q recursive_bezier_anim_n4\\*");
        bc.p = {{0, 0}, {0.5, 2}, {3, 2.5}, {4, 0}};
        curve.clear();
        bc.getAllCurve(200, curve);
        printTikzCoords("recursive_bezier_anim_n4\\tikz.dat", bc.p);

        double h = 1.0 / 198;
        for (int frame = 0; frame <= 198; ++frame)
        {
            double t = h * frame;
            if (t > 1)
                t = 1;

            auto p1_1 = bc.p[0] * (1 - t) + bc.p[1] * t;
            auto p1_2 = bc.p[1] * (1 - t) + bc.p[2] * t;
            auto p1_3 = bc.p[2] * (1 - t) + bc.p[3] * t;

            auto p2_1 = p1_1 * (1 - t) + p1_2 * t;
            auto p2_2 = p1_2 * (1 - t) + p1_3 * t;

            std::vector<vector2d> p_1{p1_1,p1_2,p1_3};
            std::vector<vector2d> p_2{p2_1,p2_2};

            std::vector<vector2d> cur_curve;
            for (int i = 0; i <= frame; ++i)
                cur_curve.push_back(curve[i]);
            std::vector<vector2d> lp{cur_curve.back()};

            std::stringstream ss;
            ss << "recursive_bezier_anim_n4\\p_frame" << frame << ".dat";
            printTikzCoords(ss.str().data(), p_1, "PP1");
            printTikzCoords(ss.str().data(), p_2, "PP2");

            ss.str("");
            ss << "recursive_bezier_anim_n4\\lp_frame" << frame << ".dat";
            printTikzCoords(ss.str().data(), lp, "LP");

            ss.str("");
            ss << "recursive_bezier_anim_n4\\curve_frame" << frame << ".dat";
            print(ss.str(), cur_curve);
        }
    }
}
