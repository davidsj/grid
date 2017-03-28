#include <cstdio>
#include <iostream>
#include <cmath>

using namespace std;

typedef unsigned int uint;

uint nth_fib(uint n, uint x, uint y) {
  float sqrt5 = sqrt(5.);
  float phi = (1. + sqrt5)/2.;
  float psi = (1. - sqrt5)/2.;

  float a = (x - psi*y)/sqrt5 * pow(phi, n);
  float b = (phi*y - x)/sqrt5 * pow(psi, n);

  return round(a*phi + b*psi);
}

template <uint N>
class board {
public:
  board() {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
	vals[i][j] = (i == j);
      }
    }
  }

  void print() {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
	if (vals[i][j] == 0) printf("   _");
	else printf("%4d", vals[i][j]);
      }
      printf("\n");
    }
    printf("\n");
  }

  uint vals[N][N];
};

int main(int argc, char **argv) {
  board<5> b;
  b.print();
}
