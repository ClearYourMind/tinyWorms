#ifndef COMMON_H
#define COMMON_H

#define F_WIDTH (WIDTH << FBITS)
#define F_HEIGHT (HEIGHT << FBITS)
#define F_WIDTH_H (F_WIDTH >> 1)
#define F_HEIGHT_H (F_HEIGHT >> 1)

#define SIGN(x) ((x) > 0) - ((x) < 0)   // evaluates X twice !!!
#define GRAVITY (1 << (FBITS-2))
#define MAX_SPEED (4 << FBITS)
#define MAX_LINES 16

#define CELL_COUNT_X 32 // WIDTH >> 2
#define CELL_COUNT_Y 16 // HEIGHT >> 2

#endif
