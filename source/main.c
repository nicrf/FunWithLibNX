#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <switch.h>


#include "lv_nx.c"

/*
#define RAYGUI_STANDALONE
#include "raygui.h"*/

/**********************
 *  STATIC PROTOTYPES
 **********************/

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
	
	/********************
	* CREATE A GROUP
	*******************/	 
	 lv_group_t *g = lv_group_create();
	// lv_group_t *groupButton = lv_group_create();
	 
	/*************************
	* Input device interface
	*************************/
    /*Add a touchpad interface*/
    /*touchpad_init();*/                            /*Initialize your touchpad*/
    lv_indev_drv_t indev_drv;                       /*Descriptor of an input device driver*/
    lv_indev_drv_init(&indev_drv);                  /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;         /*The touchpad is pointer type device*/
    indev_drv.read = ex_tp_read;                 	/*Library ready your touchpad via this function*/
    lv_indev_drv_register(&indev_drv);              /*Finally register the driver*/
	
	/*Add a gamepad interface*/
    lv_indev_drv_t indev_gamepad_drv;                       /*Descriptor of an input device driver*/
    lv_indev_drv_init(&indev_gamepad_drv);                  /*Basic initialization*/
    indev_gamepad_drv.type = LV_INDEV_TYPE_KEYPAD;         /*The touchpad is pointer type device*/
    indev_gamepad_drv.read = gamepad_read;                 	/*Library ready your touchpad via this function*/
    lv_indev_t * kp_indev = lv_indev_drv_register(&indev_gamepad_drv);              /*Finally register the driver*/
	lv_indev_set_group(kp_indev, g);
	/********************
	*  THEME
	*******************/
	/*Initialize the alien theme
     * 210: a green HUE value
     * NULL: use the default font (LV_FONT_DEFAULT)*/
    lv_theme_t *th = lv_theme_night_init(210, NULL);    
    lv_theme_set_current(th);						/*Set the current system theme*/
	
	
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
    lv_obj_t *label = lv_label_create(lv_scr_act(), NULL);  /*First parameters (scr) is the parent*/
    lv_label_set_text(label, "Simple File Explorer");  /*Set the text*/
    lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	lv_obj_set_size(label,400,50);
	/****************
     * ADD A FILE LIST
     ****************/
	/*Create the File list*/
	lv_obj_t * listFile = lv_list_create(lv_scr_act(), NULL);
	lv_obj_set_size(listFile, 400, 600);
	lv_obj_align(listFile, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 100);
	lv_list_set_sb_mode(listFile, LV_SB_MODE_AUTO);
	lv_group_add_obj(g, listFile);
	
	/*Add list elements*/
	lv_list_add(listFile, SYMBOL_FILE, "New", NULL);


	/*Create a normal button*/
	lv_obj_t * btn1 = lv_btn_create(lv_scr_act(), NULL);
	lv_cont_set_fit(btn1, true, true); /*Enable resizing horizontally and vertically*/
	lv_obj_align(btn1, NULL, LV_ALIGN_IN_TOP_LEFT, 900, 100);
	lv_obj_set_free_num(btn1, 1);   /*Set a unique number for the button*/
	lv_group_add_obj(g,btn1);
	
	/****************
     * Directory 
     ****************/
	DIR* dir;
	struct dirent* ent;
    dir = opendir(path);//Open current-working-director
	
	lv_list_add(listFile, SYMBOL_DIRECTORY, "...", NULL);
	if(dir==NULL)
    {
        //printf("Failed to open dir.\n");
		
		//lv_page_clean(listFile);
    }
    else
    {        
		//printf("Dir-listing for '':\n");
        while ((ent = readdir(dir)))
        {

			lv_list_add(listFile, SYMBOL_DIRECTORY, ent->d_name, NULL);
        }
        closedir(dir);       
    }
	
	/****************
	 * File 
	 ****************/ 
	
	
	
	/*Add a label to show the current value*/
	/*Create the third line meter and label*/
	lv_obj_t * lmeter;
	lmeter = lv_lmeter_create(lv_scr_act(), NULL);
	lv_lmeter_set_range(lmeter, 0, 60);   
	lv_lmeter_set_value(lmeter, 0);
	lv_obj_align(lmeter, NULL, LV_ALIGN_IN_BOTTOM_RIGHT, -20, -20);
	u32  cnt=0;
	

	/***********************
	* Main loop
	***********************/
	while(appletMainLoop())
	{
		
		//Scan all the inputs. This should be done once for each frame
		hidScanInput();

		// Your code goes here
		if(cnt==60)
		{
			cnt=0;
			//lv_page_clean(listFile);
		}
		else
		{
			cnt++;
		}
		lv_lmeter_set_value(lmeter, cnt);
		
		if (nx_ctrl_handler()) break; // break in order to return to hbmenu
		
		/* Periodically call this function.
         * The timing is not critical but should be between 1..10 ms */
		lv_tick_inc(5);
        lv_task_handler();
		
		//lv_label_set_text(label, get_last_key());
		/*
		if (kDown & KEY_A)
		{
			//lv_page_clean(listFile);
			//lv_label_set_text(label, "asddasadsads ");  //Set the text//
			
			for (int x = 0; x < 750; x++) {
				_put_px(x,50,RGBA8_MAXALPHA(255,128,250));
			}        
		}*/
		
		
		
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



