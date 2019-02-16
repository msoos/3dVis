#include "Node.h"
#include <iostream>

void Node3D::set_pos(float x, float y, float z)
{
  node_pos.x = x;
  node_pos.y = y;
  node_pos.z = z;
}

void Node3D::set_pos(const Vector3D& p)
{
  node_pos = p;
}

void Node3D::set_id(int i)
{
  ident = i;
}

void Node3D::set_weight(int w)
{
  c_weight = w;
}

bool Node3D::add_neighbor(Node3D* n, EdgeAttribute a)
{
  if(a == NT_3_PLUS_CLAUSE)
    // search for already existing 2-clause first
    if(neighbor_nodes.find(pair<Node3D*, EdgeAttribute>(n, NT_2_CLAUSE)) != neighbor_nodes.end())
      return false; // do not insert new edge
  pair<set<ExtNode3D>::iterator, bool> res =
    neighbor_nodes.insert(pair<Node3D*, EdgeAttribute>(n,a));

  return res.second;
}

const Vector3D& Node3D::position(void) const
{
  return node_pos;
}

int Node3D::id(void) const
{
  return ident;
}

int Node3D::weight(void) const
{
  return c_weight;
}

const set<ExtNode3D>& Node3D::neighbors(void) const
{
  return neighbor_nodes;
}
