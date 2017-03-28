#include <iostream>
#include <cmath>

using namespace std;

int nth_fib(int n, int x, int y) {
  float sqrt5 = sqrt(5.);
  float phi = (1. + sqrt5)/2.;
  float psi = (1. - sqrt5)/2.;

  float a = (x - psi*y)/sqrt5 * pow(phi, n);
  float b = (phi*y - x)/sqrt5 * pow(psi, n);

  return round(a*phi + b*psi);
}

int main(int argc, char **argv) {
  for (int i = 0; i < 20; i++) {
    cout << nth_fib(i, 1, 0) << ' ';
  }
  cout << endl;
}
