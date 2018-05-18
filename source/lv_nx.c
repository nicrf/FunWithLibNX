/*********************
*      INCLUDES
 *********************/
#include "lv_nx.h"


/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void ex_disp_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p);
static void ex_disp_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p);
static void ex_disp_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2,  lv_color_t color);
#if USE_LV_GPU
static void ex_mem_blend(lv_color_t * dest, const lv_color_t * src, uint32_t length, lv_opa_t opa);
static void ex_mem_fill(lv_color_t * dest, uint32_t length, lv_color_t color);
#endif
static bool ex_tp_read(lv_indev_data_t *data);
static bool gamepad_read(lv_indev_data_t *data);
static void  put_px(u32 x,u32 y, lv_color_t  color_p);
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

/* Flush the content of the internal buffer the specific area on the display
 * You can use DMA or any hardware acceleration to do this operation in the background but
 * 'lv_flush_ready()' has to be called when finished
 * This function is required only when LV_VDB_SIZE != 0 in lv_conf.h*/
static void ex_disp_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    int32_t x;
    int32_t y;
    for(y = y1; y <= y2; y++) {
        for(x = x1; x <= x2; x++) {
            /* Put a pixel to the display. For example: */
            put_px(x, y, *color_p);
            color_p++;
        }
    }

    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    lv_flush_ready();
	// gfxFlushBuffers();
	// gfxSwapBuffers();
	// gfxWaitForVsync();
}


/* Write a pixel array (called 'map') to the a specific area on the display
 * This function is required only when LV_VDB_SIZE == 0 in lv_conf.h*/
static void ex_disp_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/
    int32_t x;
    int32_t y;
    for(y = y1; y <= y2; y++) {
        for(x = x1; x <= x2; x++) {
            /* Put a pixel to the display. For example: */
            put_px(x, y, *color_p);
            color_p++;
        }
    }
}


/* Write a pixel array (called 'map') to the a specific area on the display
 * This function is required only when LV_VDB_SIZE == 0 in lv_conf.h*/
static void ex_disp_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2,  lv_color_t color)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    int32_t x;
    int32_t y;
    for(y = y1; y <= y2; y++) {
        for(x = x1; x <= x2; x++) {
            /* Put a pixel to the display. For example: */
		  put_px(x, y, color);
        }
    }
}

#if USE_LV_GPU

/* If your MCU has hardware accelerator (GPU) then you can use it to blend to memories using opacity
 * It can be used only in buffered mode (LV_VDB_SIZE != 0 in lv_conf.h)*/
static void ex_mem_blend(lv_color_t * dest, const lv_color_t * src, uint32_t length, lv_opa_t opa)
{
    /*It's an example code which should be done by your GPU*/

    int32_t i;
    for(i = 0; i < length; i++) {
        dest[i] = lv_color_mix(dest[i], src[i], opa);
    }
}

/* If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color
 * It can be used only in buffered mode (LV_VDB_SIZE != 0 in lv_conf.h)*/
static void ex_mem_fill(lv_color_t * dest, uint32_t length, lv_color_t color)
{
    /*It's an example code which should be done by your GPU*/

    int32_t i;
    for(i = 0; i < length; i++) {
        dest[i] = color;
    }
}

#endif

/* Read the touchpad and store it in 'data'
 * REaturn false if no more data read; true for ready again */
static bool ex_tp_read(lv_indev_data_t *data)
{
	//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
	u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
	data->key = kDown;
	if (kDown & KEY_TOUCH) 
	{
		 touchPosition touch;
		 hidTouchRead(&touch, hidTouchCount()); //Get last touch
		/* Read your touchpad */
		 data->state = LV_INDEV_STATE_PR;// or LV_INDEV_STATE_PR  LV_INDEV_STATE_REL;
		 data->point.x = touch.px;
		 data->point.y = touch.py; 
	}
	else
	{
		data->state =LV_INDEV_STATE_REL;
	}

    return false;   /*false: no more data to read because we are no buffering*/
}

static bool gamepad_read(lv_indev_data_t *data) 
{
    //data->key = last_key();
        
  /*  if(key_pressed()) {
        data->state = LV_INDEV_EVENT_PR;
    } else {
        data->state = LV_INDEV_EVENT_REL;
    }*/

    return false;   /*No buffering so no more data read*/
} 


static void  put_px(u32 x,u32 y, lv_color_t  color_p)
{
	_put_px(x,y,RGBA8_MAXALPHA(color_p.red,color_p.green,color_p.blue));
}


