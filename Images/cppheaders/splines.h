#ifndef SPLINESH
#define SPLINESh

#include <cmath>
#include <vector>

#include "vector2d.h"
#include "vector3d.h"

template <typename t>
t faktorial(t n)
{
    if (n == 0)
        return 1;
    t r = 1;
    do
        r *= n--;
    while (n > 1);
    return r;
}

template <class T>
struct Bezier
{
    std::vector<T> p;

    double B(int i, int n, double t)
    {
        return 1.0 * faktorial(n) / faktorial(i) / faktorial(n - i) * std::pow(t, i) * std::pow(1 - t, n - i);
    }

    T operator()(double t)
    {
        T _p;
        for (int i = 0; i < p.size(); ++i)
        {
            _p = _p + B(i, p.size() - 1, t) * p[i];
        }
        return _p;
    }

    // вывод весовой функции
    void getB(int i, int n, int count, std::vector<vector2d> &points)
    {
        double h = 1.0 / (count - 1);
        for (int i = 0; i <= count; ++i)
        {
            double t = i * h;
            if (t > 1)
                t = 1;
            points.emplace_back({t, B(i, n, t)});
        }
    }

    void getAllCurve(int count, std::vector<T> &curve)
    {
        double h = 1.0 / (count - 1);

        for (int i = 0; i <= count; ++i)
        {
            double t = i * h;
            if (t > 1)
                t = 1;
            curve.emplace_back(operator()(t));
        }
    }
};

template <class T>
struct BSpline
{
    std::vector<double> x;
    std::vector<T> p;

    virtual double N(int i, int k, double t) const
    {
        if (k == 0)
        {
            if (t==x.back())  //костыль, но ничего не могу поделать.
                t-=1e-15;  //чтобы вписывалось всегда в последнйи интервал.
            if (x[i] <= t && t < x[i + 1])
                return 1;
            else
                return 0;
        }
        else
        {
            double r = 0;
            double znam1 = x[i + k] - x[i];
            if (znam1 != 0)
            {
                double n1 = BSpline::N(i, k - 1, t);
                r += n1 * (t - x[i]) / znam1;
            }

            double znam2 = x[i + k + 1] - x[i + 1];
            if (znam2 != 0)
            {
                double n2 = BSpline::N(i + 1, k - 1, t);
                r += n2 * (x[i + k + 1] - t) / znam2;
            }

            return r;
        }
    }


    virtual T operator()(int k, double t) const
    {
        T v;

        for (int i = 0; i < p.size(); ++i)
        {
            v = v + p[i] * BSpline::N(i, k, t);
        }
        return v;
    }

    void getCurve(int k, int count, std::vector<T> &curve) const
    {
        double t_min = x.front();
        double t_max = x.back();
        double h = (t_max - t_min) / (count - 1);

        for (int i = 0; i < count; ++i)
        {
            double t;
            if (i == count - 1)
                t = t_max;
            else if (i == 0)
                t = t_min;
            else
                t = t_min + h * i;

            auto p = operator()(k, t);
            curve.push_back(p);
        }
    }

    void getN(int i, int k, int count, std::vector<vector2d> &points) const
    {
        double t_min = x.front();
        double t_max = x.back();
        double h = (t_max - t_min) / (count - 1);

        for (int j = 0; j < count; ++j)
        {
            double t;
            if (j == count - 1)
                t = t_max;
            else if (j == 0)
                t = t_min;
            else
                t = t_min + h * j;
            points.emplace_back(t, N(i, k, t));
        }
    }
};

std::vector<double> makeKnotsOpen(int k, int n)
{
    std::vector<double> r(n + k + 1);
    for (int i = 0; i <= k; ++i)
    {
        r[i] = 0;
    }
    for (int i = k + 1; i <= n - 1; ++i)
    {
        r[i] = i - k;
    }
    for (int i = n; i <= n + k; ++i)
    {
        r[i] = n - k;
    }
    return r;
}

template <class T>
struct Nurbs : public BSpline<T>
{
    std::vector<double> h;

    double N(int i, int k, double t)
    {
        double znam = 0;
        for (int j = 0; j < h.size(); ++j)
        {
            znam += h[j] * BSpline::N(j, k, t);
        }
        if (!znam)
            return 0;
        return h[i] * BSpline::N(i, k, t) / znam;
    }
};

template <class T>
struct NurbsSurf
{
    std::vector<std::vector<T>> p;
    std::vector<double> U;
    std::vector<double> V;
    std::vector<std::vector<double>> w;

    double N(double i, double k, double t, std::vector<double> &knots)
    {
        double r = 0;

        if (k == 0)
        {
            if (knots[i] <= t && t <= knots[i + 1])
                r = 1;
            else
                r = 0;
        }
        else
        {
            double znam = knots[i + k] - knots[i];
            if (znam)
            {
                double n = N(i, k - 1, t, knots);
                r += (t - knots[i]) * n / znam;
            }

            znam = knots[i + k + 1] - knots[i + 1];
            if (znam)
            {
                double n = N(i + 1, k - 1, t, knots);
                r += (knots[i + k + 1] - t) * n / znam;
            }
        }
        return r;
    }
    double N_U(double i, double k, double u)
    {
        return N(i, k, u, U);
    }

    double N_V(double i, double k, double v)
    {
        return N(i, k, v, V);
    }

    double R(int i, int j, double u, double v, int n, int m)
    {
        double znam = 0;
        for (int i = 0; i < p.size(); ++i)
        {
            for (int j = 0; j < p[0].size(); ++j)
            {
                znam += N_U(j, n, u) * N_V(i, m, v) * w[i][j];
            }
        }
        if (!znam)
            return 0;

        return N_U(j, n, u) * N_V(i, m, v) * w[i][j] / znam;
    }

    T operator()(double u, double v, int n, int m)
    {
        T _p;

        for (int i = 0; i < p.size(); ++i)
        {
            for (int j = 0; j < p[0].size(); ++j)
            {
                _p = _p + p[i][j] * R(i, j, u, v, n, m);
            }
        }

        return _p;
    }
};

#endif