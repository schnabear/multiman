#ifndef	__M2V_SAMPLE_H__
#define	__M2V_SAMPLE_H__

#include <types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/paths.h>
#include <sys/timer.h>
#include <sys/process.h>
#include <sys/sys_time.h>
#include <sys/ppu_thread.h>
#include <sys/spu_initialize.h>
#include <sys/synchronization.h>
#include <sysutil/sysutil_common.h>

#include <cell/font.h>
#include <cell/fontFT.h>
#include <cell/dbgfont.h>

#include <cell/codec.h>
#include <cell/sysmodule.h>

#include <cell/pad.h>
#include <cell/mouse.h>
#include <cell/keyboard.h>




/*E frame rate. */
#define MY_FPS_24 24
#define MY_FPS_30 30
#define MY_FPS_60 60
#define MY_FRAME_NUM 2
#define MY_FRAME_WIDTH 720
#define MY_FRAME_HEIGHT 480

#define MY_DISPLAY_WIDTH 1920//MY_FRAME_WIDTH
#define MY_DISPLAY_HEIGHT 1080 // MY_FRAME_HEIGHT

#define MY_READ_SIZE_MAX (512*1024)*4//(1289585) //18874368//
#define MY_FILE_NAME "/dev_hdd0/game/BLES80608/USRDIR/video.m2v"


/*E number for sysutil callback slot. */
#define MY_SYSUTIL_CB_SLOT 0

#define MY_DBG( STRING, ... ) \
	printf( "%s> " STRING, __func__, ##__VA_ARGS__ )

/*E execution flag. */
extern bool g_exec;

void
m2v_sample_exit( void );

int32_t
m2v_sample_start( sys_ppu_thread_t *thread_id );

//int32_t main_video( void );

int32_t
m2v_sample_return( sys_ppu_thread_t thread_id );

//int32_t main_video( void );

#endif /*E __M2V_SAMPLE_H__ */
