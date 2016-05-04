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
  struct matrix *tmp;
  struct stack *s;
  screen t;

  color g;
  g.red = 225;
  g.blue = 0;
  g.green = 0;

  s = new_stack();
  tmp = new_matrix(4, 1000);
  clear_screen( t );

  for (i=0;i<lastop;i++) {  
    switch (op[i].opcode) {
      case LINE;
      if ( strncmp(line, "line", strlen(line)) == 0 ) {
	fgets(line, 255, f);
	sscanf(line, "%lf %lf %lf %lf %lf %lf", &x, &y, &z, &x1, &y1, &z1);
	add_edge(tmp, x, y, z, x1, y1, z1);
	matrix_mult(stax->data[stax->top], tmp);
	draw_lines(tmp, s, g);
	free_matrix(tmp);
	// printf( "%lf %lf %lf %lf %lf %lf\n", x, y, z, x1, y1, z1);
      }
    case BOX;
      else if ( strncmp(line, "box", strlen(line)) == 0 ) {
	fgets(line, 255, f);
	sscanf(line, "%lf %lf %lf %lf %lf %lf", &x, &y, &z, &x1, &y1, &z1);
	add_box(tmp, x, y, z, x1, y1, z1);
	matrix_mult(stax->data[stax->top], tmp);
	draw_polygons(tmp, s, g);
	free_matrix(tmp);
	//add_box(pm, x, y, z, x1, y1, z1);
	// printf( "%lf %lf %lf %lf %lf %lf\n", x, y, z, x1, y1, z1);
      }
    case SPHERE;
      else if (strncmp(line, "sphere", strlen(line)) == 0 ) {
	fgets(line, 255, f);
	sscanf(line, "%lf %lf %lf", &x, &y, &z);
	add_sphere(tmp, x, y, z, 10);
	matrix_mult(stax->data[stax->top], tmp);
	draw_polygons(tmp, s, g);
	free_matrix(tmp);
	//printf( "%lf %lf %lf\n", x, y, z);
      }
    case TORUS;
      else if (strncmp(line, "torus", strlen(line)) == 0 ) {
	fgets(line, 255, f);
	sscanf(line, "%lf %lf %lf %lf", &x, &y, &z, &z1);
	add_torus(tmp, x, y, z, z1, 10);
	matrix_mult(stax->data[stax->top], tmp);
	draw_polygons(tmp, s, g);
	free_matrix(tmp);
	//printf( "%lf %lf %lf\n", x, y, z);
      }
    case SCALE;
      else if ( strncmp(line, "scale", strlen(line)) == 0 ) {
	//printf("SCALE\n");
	fgets(line, 255, f);
	//line[strlen(line)-1]='\0';      
	sscanf(line, "%lf %lf %lf", &x, &y, &z);
	tmp = make_scale(x, y, z);
	matrix_mult(tmp, stax->data[ stax->top ]);
	//print_matrix(transform);
      }
    case MOVE;
      else if ( strncmp(line, "translate", strlen(line)) == 0 ) {
	//printf("TRANSLATE\n");
	fgets(line, 255, f);
	//      line[strlen(line)-1]='\0';      
	sscanf(line, "%lf %lf %lf", &x, &y, &z);
	tmp = make_translate(x, y, z);
	matrix_mult(tmp, stax->data[ stax->top ]);
	//print_matrix(transform);
      }
    case ROTATE;

    case APPLY;
      else if ( strncmp(line, "apply", strlen(line)) == 0 ) {
	//printf("APPLY!\n");
	//print_matrix( transform );
	//      print_matrix(pm);
	matrix_mult(transform, pm);
      }
    case PUSH;
      else if (strncmp (line, "push", strlen(line)) == 0) {
	push(stax);
      }
    case POP;
      else if (strncmp (line, "pop", strlen(line)) == 0) {
      pop(stax);
      }
    case DISPLAY;
      else if ( strncmp(line, "display", strlen(line)) == 0 ) {
	//clear_screen(s);
	//draw_polygons(pm, s, g);
	display(s);
      }
    case SAVE;
      else if ( strncmp(line, "save", strlen(line)) == 0 ) {
	fgets(line, 255, f);
	// line[strlen(line)-1] = '\0';
	//clear_screen(s);
	//draw_polygons(tmp, s, g);
	save_extension(s, line);
      }
      else if ( strncmp(line, "clear", strlen(line)) == 0 ) {
	pm->lastcol = 0;
      }
      else if ( strncmp(line, "quit", strlen(line)) == 0 ) {
	return;
      }
      else if ( line[0] != '#' ) {
	printf("Invalid command\n");
      }
    }
    free_matrix(transform);
    free_matrix(tmp);
    fclose(f);
  }
}
