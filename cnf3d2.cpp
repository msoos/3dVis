#include <vector>
#include <set>
#include <map>
#include <iterator>
#include <iostream>
#include <fstream>
#include <GL/glut.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"
#include "SceneParameters.h"

using namespace std;

// ----------------------------------------------------------------------

float sphi = 0.0, stheta = 0.0;
float zoom = 1.0;
float zNear = 0.1, zFar = 20.0;
int downX, downY;
bool leftButton = false, middleButton = false, rightButton = false;

float xOffset = 0.0, yOffset = 0.0;

Graph3D* current_graph; // currently displayed graph
vector<Graph3D*> graph_stack;

float k = 1.0; // old: 5.0
float f_k = sqrt(4.0/7.0);
int curr_L;

bool next_graph = false, draw_edges = true, run_anim = false, adaptive_size = true;
bool draw_only_2clauses = false;

Vector3D min_p, max_p;

// ----------------------------------------------------------------------

void display(void)
{
  if(current_graph != NULL) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    
    //Vector3D mid_p = 0.5 * (min_p + max_p);
    glTranslatef(xOffset, yOffset, -10.0 / zoom);

    glRotatef(stheta, 1.0, 0.0, 0.0);
    glRotatef(sphi, 0.0, 1.0, 0.0);

    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);   

    current_graph->draw3D(k, draw_edges, draw_only_2clauses, adaptive_size);
    
    glPopMatrix();
  
    glutSwapBuffers();
  }
}

void setupNodes(Graph3D* g)
{
  Node3D n(1), m(2), o(3);

  g->add_node(n);
  g->add_node(m);
  g->add_node(o);
  g->insert_edge(1, 2);
  g->insert_edge(1, 3);
}

// builds (global) stack of coarsened graphs
void init(char* filename)
{
  Graph3D* g = new Graph3D();
  graph_stack.push_back(g);

  // build initial graph
  if(filename == NULL)
    setupNodes(g);
  else {
    if(strncmp("-", filename, 1) == 0)
      g->build_from_cnf(cin);
    else {
      ifstream is(filename);
      g->build_from_cnf(is);
      is.close();
    }
  }
  cout << "Built initial graph G=(V,E) with |V| = " << g->nr_nodes() << " and |E| = "
       << g->nr_edges() << "." << endl;

  // check for multiple components
  vector<int> head_nodes;
  int nr_comp = g->independent_components(&head_nodes);
  cout << "Graph consists of " << nr_comp << " independent component(s)." << endl;
  if(nr_comp > 1) {
    cout << "Multiple components not yet supported!" << endl;
    exit(10);
  }
  
  // build graph stack
  Graph3D *a = g, *b;
  int level = 1;
  while(a->nr_nodes() > 2) {
    b = a->coarsen();
    graph_stack.push_back(b);
    cout << "Coarsened graph " << level << " consists of " << b->nr_nodes()
	 << " vertices and " << b->nr_edges() << " edge(s)." << endl;
    a = b;
    level++;
  }

  // compute (random) layout of coarsest graph (with 2 nodes)
  curr_L = graph_stack.size()-1;
  graph_stack[curr_L]->init_coarsest_graph_positions(k);
  pair<Vector3D,Vector3D> ep = graph_stack[curr_L]->compute_extremal_points();
  min_p = ep.first;  max_p = ep.second;
  current_graph = graph_stack[curr_L];
  curr_L--;

  // set up OpenGL
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
  glEnable(GL_LINE_SMOOTH);
  glEnable (GL_BLEND);
  glLineWidth(1.5);
     
  // set up two light sources:
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_0);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position_0);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse_1);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position_1);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHTING);

  // hidden surface elimination
  glEnable(GL_DEPTH_TEST);
}

void motion(int x, int y)
{
  if (leftButton)
    {
      sphi += (float)(x - downX) / 4.0;
      stheta += (float)(y - downY) / 4.0;
    }
  if (middleButton)
    {
      zoom += (float)(y - downY) * 0.01;
    }
  if (rightButton)
    {
      xOffset += (float)(x - downX) / 200.0;
      yOffset += (float)(downY - y) / 200.0;
    }
  downX = x;
  downY = y;
  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    downX = x;
    downY = y;
    leftButton = ((button == GLUT_LEFT_BUTTON) &&
                  (state == GLUT_DOWN));
    middleButton = ((button == GLUT_MIDDLE_BUTTON) &&
                    (state == GLUT_DOWN));
    rightButton =  ((button == GLUT_RIGHT_BUTTON) &&
                    (state == GLUT_DOWN));
}

void reshape(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.0, (float)w / (float) h, zNear/2.0, zFar*2.0); // 1st arg.: view angle
  glMatrixMode(GL_MODELVIEW);
}

void idle(void)
{
  if(next_graph || run_anim) {
    if(curr_L >= 0) {
      // layout & display next finer graph
      cout << "L = " << curr_L << " " << flush;
      graph_stack[curr_L]->init_positions_from_graph(graph_stack[curr_L+1], k);
      //      graph_stack[curr_L]->init_positions_at_random();
      graph_stack[curr_L]->compute_layout(k);
      pair<Vector3D,Vector3D> ep = graph_stack[curr_L]->compute_extremal_points();
      min_p = ep.first;  max_p = ep.second;
      float max_extent = max(max_p.x - min_p.x, max(max_p.y - min_p.y, max_p.z - min_p.z));
      //  cout << max_extent << " " << flush;
      // rescale to -10.0 .. +10.0 on all axes
      Vector3D shift = Vector3D(-1.0,-1.0,-1.0) - 2.0 / max_extent * min_p;
      graph_stack[curr_L]->rescale(2.0 / max_extent, shift);
      current_graph = graph_stack[curr_L];
      k *= f_k;
      curr_L--;
    }
    next_graph = false;
    glutPostRedisplay();
  }
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
   case 'n':
     next_graph = true;
     break;
   case 'e':
     draw_edges = !draw_edges;
     break;
   case '2':
     draw_only_2clauses = !draw_only_2clauses;
     break;
   case 'g':
     run_anim = !run_anim;
     break;
   case 'r': // reset
     xOffset = yOffset = 0.0;
     zoom = 1.0;
     sphi = stheta = 0.0;
     break;
   case 'a':
     adaptive_size = !adaptive_size;
     break;
   }
   glutPostRedisplay();
}

int main(int argc, char* argv[])
{
  glutInitWindowSize(800, 600);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("3D CNF Visualization (C) 2006 C. Sinz, JKU Linz");
  init(argv[1]);
  glutDisplayFunc(display);
  glutMotionFunc(motion);
  glutMouseFunc(mouse);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
