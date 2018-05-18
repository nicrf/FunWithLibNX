/*********************
*      INCLUDES
 *********************/
#include <switch.h>
#include "lvgl/lvgl.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void  _put_px(u32 x,u32 y, u32  rga8);
static void  _put_line(u32 xstart,u32 ystart,u32 xend,u32 yend, u32 thickness, u32  rga8);
/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/


/**********************
 *   STATIC FUNCTIONS
 **********************/

static void  _put_px(u32 x,u32 y, u32  rga8)
{
	u32 width, height;
	u32 pos;
	u32* framebuf;
	framebuf = (u32*) gfxGetFramebuffer((u32*)&width, (u32*)&height);
	pos = y * width + x;
	framebuf[pos] = rga8;// RGBA8_MAXALPHA(color_p.red,color_p.green,color_p.blue);
}

static void  _put_line(u32 xstart,u32 ystart,u32 xend,u32 yend, u32 thickness, u32  rga8)
{
	//for (int i = -thickness/2; i < thickness/2 + 0.5; i++) {
     //   for (int x = xstart; x < end_x; x++) {
     //       _put_px(x, y+i, rga8);
     //   }
    //}
}