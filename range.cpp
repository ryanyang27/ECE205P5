#include "range.hpp"

double range( std::function<double( double )> f, double a, double b, double max_freq ) {
    vector<double> relativeMins;
    vector<double> relativeMaxes;
    cout.precision(16);
    double xk = f(a);
    double xkplusone = f(a + 0.25*max_freq);
    double xkplustwo;
    for (double k{2}; a + 0.25*k*max_freq < b; k++) {
        xkplustwo = f (a + 0.25*k*max_freq);
        if (xkplusone < xk && xkplusone < xkplustwo ) {
            relativeMins.push_back(interpolateMin(f, a + 0.25*(k-2)*max_freq,  a + 0.25*(k-1)*max_freq, a + 0.25*k*max_freq,  xkplusone, xk, xkplustwo ));
        } else if (xkplusone > xk && xkplusone > xkplustwo) {
            relativeMaxes.push_back(interpolateMax(f, a + 0.25*(k-2)*max_freq, a + 0.25*(k-1)*max_freq, a + 0.25*k*max_freq, xkplusone, xk, xkplustwo ));
        }
        xk = xkplusone;
        xkplusone = xkplustwo;
    }
    double minimumValue = relativeMins[0];
    double maximumValue = relativeMaxes[0];
    for (int i{1}; i < relativeMins.size(); i++) {
        std::cout << "MIN: " << relativeMins[i] << std::endl;
        if (relativeMins[i] < minimumValue) {
            minimumValue = relativeMins[i];
        }
    }
    std::cout << "STOP" << std::endl;
    for (int i{1}; i < relativeMaxes.size(); i++) {
        std::cout << "MAX: " << relativeMaxes[i] << std::endl;
        if (relativeMaxes[i] > maximumValue) {
            maximumValue = relativeMaxes[i];
        }
    }
    std::cout << "ABSMAX: " << maximumValue << std::endl;
    std::cout << "ABSMIN: " << minimumValue << std::endl;
    return maximumValue - minimumValue;
}

double interpolateMin( std::function<double( double )> f, double lowRange, double middleRange, double highRange, double middle, double low, double high ) {
    double fxk;
    double fxko;
    double fxkt;
    double xk;
    double xko;
    double xkt;
    if (middle < low && middle < high) {
        fxk = middle;
        xk = middleRange;
        if (low < high) {
            fxko = low;
            xko = lowRange;
            fxkt = high;
            xkt = highRange;
        } else {
            fxkt = low;
            xkt = lowRange;
            fxko = high;
            xko = highRange;
        }
    } else {
        fxko = middle;
        xko = middleRange;
        if ( low < high) {
            xk = lowRange;
            fxk = low;
            xkt = highRange;
            fxkt = high;
        } else {
            xkt = lowRange;
            fxkt = low;
            xk = highRange;
            fxk = high;
        }
    }
    double xkpo = (xk + xko)/2.0 + (fxk - fxko)*(xko - xkt)*(xkt-xk)/(2.0*((fxk-fxko)*(xko-xkt)+(fxkt-fxko)*(xk-xko)));
    double fxkpo = f(xkpo);
    if (abs(fxkpo-fxk) <= 1.0e-8 && abs(xkpo - xk) <= 1.0e-10) {
        if (xkpo > xk) {
            return fxk;
        } else {
            return fxkpo;
        }
    } else {
        if (xk > xkpo) {
            if (xkpo > xko) { // xk > xkpo > xko
                return interpolateMin(f, xko, xkpo, xk, fxkpo, fxko, fxk);
            } else if (xk > xko) { // xk > xko > xkpo
                return interpolateMin(f, xkpo, xko, xk, fxko, fxkpo, fxk);
            } else { // xko > xk > xkpo
                return interpolateMin(f, xkpo, xk, xko, fxk, fxkpo, fxko);
            };
        } else {
            if (xk > xko) { // xkpo > xk > xko
                return interpolateMin(f, xko, xk, xkpo, fxk, fxko, fxkpo);
            } else if (xkpo > xko) { // xkpo > xko > xk
                return interpolateMin(f, xk, xko, xkpo, fxko, fxk, fxkpo);
            } else { // xko > xkpo > xk
                return interpolateMin(f, xk, xkpo, xko, fxkpo, fxk, fxko);
            };
        };
    };
}

double interpolateMax( std::function<double( double )> f, double lowRange, double middleRange, double highRange, double middle, double low, double high ) {
    double fxk;
    double fxko;
    double fxkt;
    double xk;
    double xko;
    double xkt;
    if (middle > low && middle > high) {
        fxk = middle;
        xk = middleRange;
        if (low < high) {
            fxkt = low;
            xkt = lowRange;
            fxko = high;
            xko = highRange;
        } else {
            fxko = low;
            xko = lowRange;
            fxkt = high;
            xkt = highRange;
        }
    } else {
        fxko = middle;
        xko = middleRange;
        if ( low > high) {
            xk = lowRange;
            fxk = low;
            xkt = highRange;
            fxkt = high;
        } else {
            xkt = lowRange;
            fxkt = low;
            xk = highRange;
            fxk = high;
        }
    }
    double xkpo = (xk + xko)/2.0 + (fxk - fxko)*(xko - xkt)*(xkt-xk)/(2.0*((fxk-fxko)*(xko-xkt)+(fxkt-fxko)*(xk-xko)));
    double fxkpo = f(xkpo);
    if (abs(fxkpo-fxk) <= 1.0e-6 && abs(xkpo - xk) <= 1.0e-8) {
        if (fxkpo < fxk) {
            return fxk;
        } else {
            return fxkpo;
        }
    } else {
        if (xk > xkpo) {
            if (xkpo > xko) { // xk > xkpo > xko
                return interpolateMax(f, xko, xkpo, xk, fxkpo, fxko, fxk);
            } else if (xk > xko) { // xk > xko > xkpo
                return interpolateMax(f, xkpo, xko, xk, fxko, fxkpo, fxk);
            } else { // xko > xk > xkpo
                return interpolateMax(f, xkpo, xk, xko, fxk, fxkpo, fxko);
            };
        } else {
            if (xk > xko) { // xkpo > xk > xko
                return interpolateMax(f, xko, xk, xkpo, fxk, fxko, fxkpo);
            } else if (xkpo > xko) { // xkpo > xko > xk
                return interpolateMax(f, xk, xko, xkpo, fxko, fxk, fxkpo);
            } else { // xko > xkpo > xk
                return interpolateMax(f, xk, xkpo, xko, fxkpo, fxk, fxko);
            };
        };
    };
}


double sine(double x) {
    return sin(5*x) + sin(2*x+1);
}

int main() {
    double (*f)(double);
    f = &sine;
    std::cout << range(f, 0, 10, 2*M_PI/5) << std::endl;
}