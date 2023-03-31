#include <iostream>
#include <functional>
#include <cmath>
#include <vector>

using namespace std;

double range( std::function<double( double )> f,
              double a,
              double b,
              double max_freq );

double interpolateMin( std::function<double( double )> f, double lowRange, double middleRange, double highRange, double middle, double low, double high );
double interpolateMax( std::function<double( double )> f, double lowRange, double middleRange, double highRange, double middle, double low, double high );