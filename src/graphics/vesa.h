
#ifndef VESA_H
#define VESA_H

#include "../task.h"

#define MAX_BUTTONS   10
#define MAX_CHILDREN   100

struct window;

typedef void (*callback_type_t)(struct window);

typedef struct component
{
  int x;
  int y;
  int width;
  int height;
  callback_type_t onMouseLeftClick;
} component_t;

typedef struct window
{
  char *name;
  int id;
  int parentid;
  int x;
  int y;
  int width;
  int height;
  u32int *data;
  float z;

  char *flag;
  component_t buttons[MAX_BUTTONS];
} window_t;


#endif //VESA_H
