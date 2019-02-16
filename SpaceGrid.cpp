#include "SpaceGrid.h"

SpaceGrid3D::~SpaceGrid3D()
{
  // free cubes
  for(map<GridIndex, GridCube*>::iterator i = cubes.begin(); i != cubes.end(); i++)
    delete i->second;
}


// Add node n to grid. (The position vector of node n must be set up properly.)

void SpaceGrid3D::insert_node(Node3D* n)
{
  Vector3D pos = n->position();
  GridIndex gi = grid_index(pos);
  if(cubes.find(gi) == cubes.end())
    cubes[gi] = new GridCube();
  cubes[gi]->push_back(n);
}


// Return all neighbors, i.e. nodes with distance at most 1 cube (also diagonal)
// from node n (including node n itself).

vector<Node3D*> SpaceGrid3D::find_neighbors(Node3D* n)
{
  vector<Node3D*> res;
  int i, j, k;
  GridIndex gi, gj;

  Vector3D pos = n->position();
  gi = grid_index(pos);

  // add nodes from own and neighboring cube
  for(i = gi.a - 1; i <= gi.a + 1; i++)
    for(j = gi.b - 1; j <= gi.b + 1; j++)
      for(k = gi.c - 1; k <= gi.c + 1; k++) {
	gj = GridIndex(i, j, k);
	if(cubes.find(gj) != cubes.end())
	  copy(cubes[gj]->begin(), cubes[gj]->end(), inserter(res, res.begin()));
      }
  
  return res;
} 


ostream& operator<<(ostream& os, const SpaceGrid3D& sg)
{
  map<GridIndex, GridCube*>::const_iterator i;
  vector<Node3D*>::iterator j;

  for(i = sg.cubes.begin(); i != sg.cubes.end(); i++) {
    os << "cube " << i->first << ":" << endl;
    for(j = i->second->begin(); j != i->second->end(); j++)
      os << "\t[" << (*j)->id() << ", " << (*j)->position() << "]" << endl;
  }
   
  return os;
}


// -------------------- methods for internal use  --------------------

GridIndex SpaceGrid3D::grid_index(const Vector3D& p)
{
  GridIndex gi;
  
  gi.a = (int)floorf(p.x / side_length);
  gi.b = (int)floorf(p.y / side_length);
  gi.c = (int)floorf(p.z / side_length);

  return gi;
}
