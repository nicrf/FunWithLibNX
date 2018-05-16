#include <string.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <switch.h>

#include "lvgl/lvgl.h"
/*
#define RAYGUI_STANDALONE
#include "raygui.h"*/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void ex_disp_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p);
static void ex_disp_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p);
static void ex_disp_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2,  lv_color_t color);
static bool ex_tp_read(lv_indev_data_t *data);
static void  put_px(u32 x,u32 y, lv_color_t color_p);


int main(int argc, char **argv)
{
	char path[255]="";
	/***********************
	* Initialize GFX
	***********************/
	gfxInitDefault();
	//consoleInit(NULL);	
	//gfxConfigureResolution(LV_HOR_RES,LV_VER_RES);
	
	/***********************
     * Initialize LittlevGL
     ***********************/
    lv_init();
	
	/***********************
	* Display interface
	***********************/
	lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
	lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/
	
	
	/*Set up the functions to access to your display*/
    disp_drv.disp_flush = ex_disp_flush;            /*Used in buffered mode (LV_VDB_SIZE != 0  in lv_conf.h)*/

  //  disp_drv.disp_fill = ex_disp_fill;              /*Used in unbuffered mode (LV_VDB_SIZE == 0  in lv_conf.h)*/
  //  disp_drv.disp_map = ex_disp_map;                /*Used in unbuffered mode (LV_VDB_SIZE == 0  in lv_conf.h)*/
   
	/*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
	
	
	/*************************
	* Input device interface
	*************************/
    /*Add a touchpad in the example*/
    /*touchpad_init();*/                            /*Initialize your touchpad*/
    lv_indev_drv_t indev_drv;                       /*Descriptor of an input device driver*/
    lv_indev_drv_init(&indev_drv);                  /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;         /*The touchpad is pointer type device*/
    indev_drv.read = ex_tp_read;                 	/*Library ready your touchpad via this function*/
    lv_indev_drv_register(&indev_drv);              /*Finally register the driver*/
	
	
     /********************
     *  THEME
     *******************/
	/*Initialize the alien theme
     * 210: a green HUE value
     * NULL: use the default font (LV_FONT_DEFAULT)*/
    lv_theme_t *th = lv_theme_night_init(210, NULL);

    /*Set the surent system theme*/
    lv_theme_set_current(th);
     /********************
     * CREATE A SCREEN
     *******************/
    /* Create a new screen and load it
     * Screen can be created from any type object type
     * Now a Page is used which is an objects with scrollable content*/
    lv_obj_t *scr = lv_page_create(NULL, NULL);
    lv_scr_load(scr);

    /****************
     * ADD A TITLE
     ****************/
    lv_obj_t *label = lv_label_create(scr, NULL);  /*First parameters (scr) is the parent*/
    lv_label_set_text(label, "Simple File Explorer ");  /*Set the text*/
    lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

	/****************
     * ADD A FILE LIST
     ****************/
	/*Create the File list*/
	lv_obj_t * listFile = lv_list_create(lv_scr_act(), NULL);
	lv_obj_set_size(listFile, 400, 600);
	lv_obj_align(listFile, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 100);
	lv_list_set_sb_mode(listFile, LV_SB_MODE_AUTO);
	
	/*Add list elements*/
	lv_list_add(listFile, SYMBOL_FILE, "New", NULL);
	
	
	
	/****************
     * Directory 
     ****************/
	DIR* dir;
	struct dirent* ent;
    dir = opendir(path);//Open current-working-director
	y.
	lv_list_add(listFile, SYMBOL_DIRECTORY, "...", NULL);
	if(dir==NULL)
    {
        //printf("Failed to open dir.\n");
    }
    else
    {        //printf("Dir-listing for '':\n");
        while ((ent = readdir(dir)))
        {

			lv_list_add(listFile, SYMBOL_DIRECTORY, ent->d_name, NULL);
        }
        closedir(dir);       
    }
	
	/****************
     * File 
     ****************/
	 

	/***********************
	* Main loop
	***********************/
	while(appletMainLoop())
	{
		
		//Scan all the inputs. This should be done once for each frame
		hidScanInput();

		// Your code goes here

		
		/* Periodically call this function.
         * The timing is not critical but should be between 1..10 ms */
		lv_tick_inc(5);
        lv_task_handler();
        svcSleepThread(500000);//Delay 5ms
	
		//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
		u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

		if (kDown & KEY_PLUS) break; // break in order to return to hbmenu

		 gfxFlushBuffers();
		 gfxSwapBuffers();
		 gfxWaitForVsync();
	}

	gfxExit();
	return 0;
}

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

static void  put_px(u32 x,u32 y, lv_color_t  color_p)
{
	u32 width, height;
	u32 pos;
	u32* framebuf;
	framebuf = (u32*) gfxGetFramebuffer((u32*)&width, (u32*)&height);
	pos = y * width + x;
	framebuf[pos] = RGBA8_MAXALPHA(color_p.red,color_p.green,color_p.blue);
}


/* Read the touchpad and store it in 'data'
 * REaturn false if no more data read; true for ready again */
static bool ex_tp_read(lv_indev_data_t *data)
{
	//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
	u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
	if (kDown & KEY_TOUCH) 
	{
		 touchPosition touch;
		 hidTouchRead(&touch, hidTouchCount()); //Get last touch
		/* Read your touchpad */
		 data->state = LV_INDEV_STATE_REL;// or LV_INDEV_STATE_PR  LV_INDEV_STATE_REL;
		 data->point.x = touch.px;
		 data->point.y = touch.py; 
	}

    return false;   /*false: no more data to read because we are no buffering*/
}
