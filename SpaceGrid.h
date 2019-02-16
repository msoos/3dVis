// -*- C++ -*-

// The space grid devides the 3D space into cubes of (side) lenght L.
// Each cube may contain a set of objects of class Node3D.

#ifndef _VIS_3D_SPACE_GRID
#define _VIS_3D_SPACE_GRID

#include <vector>
#include <map>
#include "Node.h"

using namespace std;

class GridIndex {
public:
  int a, b, c;  // coordinates of minimal corner in multiples of L

  GridIndex(int _a = 0, int _b = 0, int _c = 0) : a(_a), b(_b), c(_c) { }
  ~GridIndex() { }

  friend ostream& operator<<(ostream& os, const GridIndex& i) {
    os << "<" << i.a << ", " << i.b << ", " << i.c << ">";
    return os;
  }

  friend bool operator<(const GridIndex& i, const GridIndex& j) {
    return (i.a < j.a) || (i.a == j.a && i.b < j.b) ||
      (i.a == j.a && i.b == j.b && i.c < j.c);
  }
};

typedef vector<Node3D*> GridCube;

class SpaceGrid3D
{
private:
  map<GridIndex, GridCube*> cubes;
  float side_length;
  
public:
  SpaceGrid3D(float L) : side_length(L) { }
          // Create (empty) grid with cube (side) length L. Each cube has end points
          // (i*L, j*L, k*L) for integers i, j, k.

  ~SpaceGrid3D(void);
  
  void insert_node(Node3D* n);
          // Add node n to grid. (The position vector of node n must be set up properly.)
  
  vector<Node3D*> find_neighbors(Node3D* n);
          // Return all neighbors, i.e. nodes with distance at most 1 cube (also diagonal)
          // from node n (including node n itself).

  friend ostream& operator<<(ostream& os, const SpaceGrid3D& sg);
  
  // ----- auxiliary functions -----

  GridIndex grid_index(const Vector3D& p);
          // Compute grid index of cube into which a node with position p falls.
  
};

#endif
