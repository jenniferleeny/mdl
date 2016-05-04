/*========== my_main.c ==========

  This is the only file you need to modify in order
  to get a working mdl project (for now).

  my_main.c will serve as the interpreter for mdl.
  When an mdl script goes through a lexer and parser, 
  the resulting operations will be in the array op[].

  Your job is to go through each entry in op and perform
  the required action from the list below:

  push: push a new origin matrix onto the origin stack
  pop: remove the top matrix on the origin stack

  move/scale/rotate: create a transformation matrix 
                     based on the provided values, then 
		     multiply the current top of the
		     origins stack by it.

  box/sphere/torus: create a solid object based on the
                    provided values. Store that in a 
		    temporary matrix, multiply it by the
		    current top of the origins stack, then
		    call draw_polygons.

  line: create a line based on the provided values. Store 
        that in a temporary matrix, multiply it by the
	current top of the origins stack, then call draw_lines.

  save: call save_extension with the provided filename

  display: view the image live
  
  jdyrlandweaver
  =========================*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "parser.h"
#include "symtab.h"
#include "y.tab.h"

#include "misc_headers.h"
#include "matrix.h"
#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "stack.h"

void my_main( int polygons ) {

  int i;
  double step;
  double xval, yval, zval;
  struct matrix *transform;
  struct matrix *temp;
  struct stack *stax;
  screen t;

  color g;
  g.red = 225;
  g.blue = 0;
  g.green = 0;

  stax = new_stack();
  temp = new_matrix(4, 1000);
  transform = new_matrix(4, 4);
  clear_screen( t );

  for (i=0;i<lastop;i++) {  
    switch (op[i].opcode) {
    case BOX:
      add_box(temp, op[i].op.box.d0[0], op[i].op.box.d0[1], op[i].op.box.d0[2], op[i].op.box.d1[0], op[i].op.box.d1[1], op[i].op.box.d1[2]);
      matrix_mult(stax->data[stax->top], temp);
      draw_polygons(temp, t, g);
      free_matrix(temp);
      break;
    case SPHERE:
      add_sphere(temp, op[i].op.sphere.d[0], op[i].op.sphere.d[1], op[i].op.sphere.d[2], op[i].op.sphere.r, 10);
      matrix_mult(stax->data[stax->top], temp);
      draw_polygons(temp, t, g);
      free_matrix(temp);
      break;
    case TORUS:
      add_torus(temp, op[i].op.torus.d[0], op[i].op.torus.d[1], op[i].op.torus.d[2], op[i].op.torus.r0,op[i].op.torus.r1, 10);
      matrix_mult(stax->data[stax->top], temp);
      draw_polygons(temp, t, g);
      free_matrix(temp);
      break;
    case SCALE:
      transform = make_scale(op[i].op.scale.d[0], op[i].op.scale.d[1], op[i].op.scale.d[2]);
      matrix_mult(stax->data[stax->top], transform);
      copy_matrix(transform, stax->data[stax->top]);
      free_matrix(transform);
      break;
    case MOVE:
      transform = make_translate(op[i].op.move.d[0], op[i].op.move.d[1], op[i].op.move.d[2]);
      matrix_mult(stax->data[stax->top], transform);
      copy_matrix(transform, stax->data[stax->top]);
      free_matrix(transform);
      break;
    case PUSH:
      push(stax);
      break;
    case POP:
      pop(stax);
      break;
    case ROTATE:
      if (op[i].op.rotate.axis == 0)
	//case X
	transform = make_rotX(op[i].op.rotate.degrees * M_PI/180 );
      else if (op[lastop].op.rotate.axis == 1)
	//case Y
	transform = make_rotY(op[i].op.rotate.degrees * M_PI/180 );
      else if (op[lastop].op.rotate.axis == 2)
	//case Z
	transform = make_rotZ(op[i].op.rotate.degrees * M_PI/180 );
      matrix_mult(stax->data[stax->top], transform);
      copy_matrix(transform, stax->data[stax->top]);
      free_matrix(transform);
      break;
    case LINE:
      add_edge(temp, 
	       op[i].op.line.p0[0],op[i].op.line.p0[1],op[i].op.line.p0[2],
	       op[i].op.line.p1[0],op[i].op.line.p1[1],op[i].op.line.p1[2]);
      matrix_mult(stax->data[stax->top], temp);
      draw_lines(temp, t, g);
      free_matrix(temp);
      break;
    case DISPLAY:
      display(t);
      break;
    case SAVE:
      save_extension(t,op[i].op.save.p->name);
      break;
    }
  }
  free_matrix(temp);
  free_matrix(transform);
  free_stack(stax);
}
