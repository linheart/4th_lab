#include <iostream>
#include <cmath>
#include <chrono>

double f(double);
double f_1st_der(double);
double f_2nd_der(double);
double f_max(double, double);
double f_min(double, double);
double chord(double, double, double);
double newton(double, double, double);
double dixit(double, double, double);
using namespace std;

int main() {
     double a = 1.0;
    double b = 2.0;
    double eps = 0.0001;

    cout << "Root by ditix = " << dixit(a, b, eps) << endl;
    cout << "Root by chord = " << chord(a, b, eps) << endl;
    cout << "Root by newton = " << newton(a, b, eps) << endl;
    return 0;
}

double chord(double a, double b, double eps) {
    auto start = chrono::high_resolution_clock::now();
    double x_new, x_last, pin, m, M;
    int count = 0;

    if (f(a)*f_2nd_der(a) > 0) {
        x_last = b;
        pin = a;
    }
    else if(f(b)*f_2nd_der(b)>0) {
        x_last = a;
        pin = b;
    }

    else if (f(a)*f_2nd_der(a) < 0) {
        x_last = a;
        pin = b;
    }
    else if (f(b) * f_2nd_der(b) < 0) {
        x_last = b;
        pin = a;
    }
    else {
        cout << "Error";
        return 0;
    }
    m = f_min(a, b);
    M = f_max(a, b);
    while (true) {
        x_new = x_last - (pin - x_last)*(f(x_last)) / (f(pin)-f(x_last));
        count++;
        if (fabs(x_new - x_last) < eps*m/(M-m)) break;
        x_last = x_new;
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    cout << "N = " << count << "\tExecution time: " << duration.count() << " microseconds\t";

    return x_new;
}

double newton(double a, double b, double eps) {
    auto start = chrono::high_resolution_clock::now();
    
    double x1,x2,x3,q;
    int count = 0;

    if (f(a) * f_2nd_der(a) > 0) x1 = a;
    else x1 = b;
    while (true) {
        if (count == 0) {
            x2 = x1 - f(x1) / f_1st_der(x1);
            x3 = x2 - f(x2) / f_1st_der(x2);
        }
        q = (x3 - x2) / (x2 - x1);
        if ((x3-x2)*q/(1-q)<eps) break;
        x1 = x2;
        x2 = x1 - f(x1) / f_1st_der(x1);
        x2 = x3;
        x3 = x2 - f(x2) / f_1st_der(x2);
        count++;
    }
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    cout << "N = " << count << "\tExecution time: " << duration.count() << " microseconds\t";

    return x3;
}

double dixit(double a, double b, double eps) {
    auto start = chrono::high_resolution_clock::now();

    double c;
    int count = 0;
    do{
        c = (a + b) / 2.0;
        if (f(a) * f(c) < 0) b = c;
        else if(f(b) * f(c) < 0) a = c;
        else{   
            cout << "The root is not found" << endl;
            return 0;
        }
        count++;
    } while (fabs(b-a) > eps);

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    cout << "N = " << count << "\tExecution time: " << duration.count() << " microseconds\t";

    return c;double f_2nd_der(double);

}
double f_max(double a, double b) {
    double max, dx, tmp;
    dx = 0.1;

    max = fabs(f_1st_der(a));

    for (int i = 1; a+i*dx <= b; ++i) {
        tmp = fabs(f_1st_der(a + i*dx));
        if (tmp > max) max = tmp;
    }
    return max;
}
double f_min(double a,double b) {
    double min, dx,tmp;
    dx = 0.1;

    min = fabs(f_1st_der(a));
    for (int i=1; a+i*dx <= b; ++i) {
        tmp = fabs(f_1st_der(a + i*dx));
        if (tmp < min) min = tmp;
    }

    return min;
}
double f_2nd_der(double x) {
    double dx = 1e-4;
    return (f_1st_der(x+dx)-f_1st_der(x-dx)) / (2*dx);
}
double f_1st_der(double x) {
    double dx = 1e-4;
    return (f(x + dx) - f(x-dx)) / (2*dx);
}
double f(double x) {
    return cos(x) - pow(x - 1, 2);
}

