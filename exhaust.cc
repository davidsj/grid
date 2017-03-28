// https://redd.it/61mbrh
// run with: g++ -O3 exhaust.cc -o exhaust && ./exhaust

#include <cstdio>
#include <iostream>
#include <cmath>

using namespace std;

typedef unsigned int uint;

// Return the nth next number in a fibonacci sequence whose last two
// entries are (y, x).
uint nth_fib(uint n, uint x, uint y) {
  float sqrt5 = sqrt(5.);
  float phi = (1. + sqrt5)/2.;
  float psi = (1. - sqrt5)/2.;

  float a = (phi*x + y) * pow(phi, n);
  float b = -(y + psi*x) * pow(psi, n);

  return round((a + b)/sqrt5);
}

struct point {
  int i, j;
};

template <uint N>
class board {
public:
  board() {
    // Fill 1's on the main diagonal.
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
	vals[i][j] = (i == j) ? 1 : 0;
      }

      row_max[i] = 1;
      col_max[i] = 1;
    }

    square_used = 0;
    max[0] = max[1] = 1;
    n_free = N * (N - 1);

    last_place.i = -1;
    last_place.j = -1;
  }

  void place(int i, int j) {
    uint &new_val = vals[i][j];
    new_val = row_max[i] + col_max[j];

    // We only ever place in globally ascending order, so we know this
    // increases the max.
    row_max[i] = new_val;
    col_max[j] = new_val;

    max[1] = max[0];
    max[0] = new_val;
    n_free--;

    if (i >= square_used) square_used = i + 1;
    if (j >= square_used) square_used = j + 1;

    last_place.i = i;
    last_place.j = j;
  }

  int free_count() const {
    return n_free;
  }

  bool done() const {
    return n_free == 0;
  }

  uint score() const {
    return max[0];
  }

  uint max_possible() const {
    return nth_fib(n_free, max[0], max[1]);
  }

  bool avail(int i, int j) const {
    if (vals[i][j] != 0) return false;
    uint new_val = row_max[i] + col_max[j];

    // Only allow globally ascending placement (so that each finished
    // board has a canonical construction order, limiting redundant
    // searching).
    if (new_val < max[0]) return false;
      
    // Only allow placing the same number twice if the second point
    // comes after the first point in row, column order (canonical
    // construction order).
    if (new_val == max[0]) {
      if (i < last_place.i) return false;
      if (i == last_place.i && j <= last_place.j) return false;
    }

    // Allow any placement within or just outside the allocated square
    // (allowing otherwise is redundant up to row and column
    // permutation).
    if (i <= square_used && j <= square_used) return true;

    // Otherwise only a single free space is allowed (for new 2's).
    return (i == square_used) && (j == square_used + 1);
  }

  void print() const {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
	if (vals[i][j] != 0) printf(" %4d ", vals[i][j]);
	else if (avail(i, j)) printf("[%4d]", row_max[i] + col_max[j]);
	else printf("    _ ");
      }
      printf("\n");
    }
    printf("\n");
  }

private:
  // Filled in values (0 for empty).
  uint vals[N][N];

  // This is the side-length of the topleft part of the square that
  // we've filled in so far. We only ever expand incrementally to avoid
  // permutation-redundant searching.
  int square_used;

  // Max value so far in each row and column.
  uint row_max[N];
  uint col_max[N];

  // Largest and second-largest entry on the board.
  uint max[2];

  // Number of free cells.
  uint n_free;

  // The coordinates of the last entry.
  point last_place;
};

template <uint N>
void search(const board<N> &b, uint &best, unsigned long &count) {
  if (count % 100000000 == 0) {
    cout << "Searching ... " << count << endl;
    b.print();
  }
  count++;

  if (b.max_possible() <= best) return;
  if (b.done()) {
    cout << "Found score: " << b.score() << endl;
    best = b.score();
    b.print();
  }

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (b.avail(i, j)) {
	board<N> b2 = b;
	b2.place(i, j);
	search(b2, best, count);
      }
    }
  }
}

int main(int argc, char **argv) {
  uint best = 0;
  unsigned long count = 0;
  board<5> b;
  search(b, best, count);
}
