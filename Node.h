// -*- C++ -*-

#ifndef _VIS_3D_NODE
#define _VIS_3D_NODE

#include <set>
#include "Vector.h"

using namespace std;

class Node3D;
typedef enum { NT_3_PLUS_CLAUSE, NT_2_CLAUSE } EdgeAttribute;
typedef pair<Node3D*, EdgeAttribute> ExtNode3D;
               // extended node: node with additional edge attribute; used in neighbor set

class Node3D
{
private:
  int ident;
  int mark;    // mark bit for different purposes

  Vector3D node_pos;

  set<ExtNode3D> neighbor_nodes;

  int c_weight; // used for graph coarsening (weight: number of merged nodes)

public:
  Node3D(int id = 0) : ident(id), mark(0), c_weight(1) { }
  ~Node3D() { }
  
  // modifiers
  void set_pos(float x, float y, float z);
  void set_pos(const Vector3D& p);
  void set_id(int i);
  void set_weight(int i);
  bool add_neighbor(Node3D* n, EdgeAttribute a = NT_3_PLUS_CLAUSE);
             // returns true iff element was not already present

  // observables
  const Vector3D& position(void) const;
  int id(void) const;
  int weight(void) const;
  EdgeAttribute edge_attribute(Node3D* other_end) const;

  const set<ExtNode3D>& neighbors(void) const;

  friend class Graph3D;
};

#endif
