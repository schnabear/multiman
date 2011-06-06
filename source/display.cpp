/* SCE CONFIDENTIAL
 * PLAYSTATION(R)3 Programmer Tool Runtime Library 192.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cell/gcm.h>
#include <sys/timer.h>
#include <cell/error.h>
#include <cell/sysmodule.h>

#include <sysutil/sysutil_sysparam.h> 

#include "display.h"

#define BUF_NUM 2

#define END_LABEL_ID 128

#define ROUNDUP(x, a) (((x)+((a)-1))&(~((a)-1)))


static void
_get_gpu_sync(void)
{
	static uint32_t *label;
	static int32_t first = 1;
	static uint32_t cpuCounter=0;

	if ( 1 == first ) {
		/*E initialize GPU-progress counter. */
		label = cellGcmGetLabelAddress( END_LABEL_ID );
		*label = 0;
	}

	cellGcmSetWriteBackEndLabel( gCellGcmCurrentContext, 
	                             END_LABEL_ID,
								 cpuCounter              );

	cellGcmFlush( gCellGcmCurrentContext );

	/*E not to let CPU work too fast */
	/*E this code would not work when 32bit counter overflows */
	while ( cpuCounter > *((volatile uint32_t*) label) + 1 ) {
		sys_timer_usleep(100);
	}

	cpuCounter++;

	first = 0;
}

void _buffer_flip(void)
{

	static int first = 1;

	/*E triple buffering. */
	if( 2 < BUF_NUM ){
		/*E wait real flip before starting the draw after the next. */
		if( 1 != first ){
			cellGcmSetWaitFlip( gCellGcmCurrentContext );
		}else{
			first = 0;
		}
	}

	if( cellGcmSetFlip( gCellGcmCurrentContext, 0 ) != CELL_OK ) 
		return;

	cellGcmFlush( gCellGcmCurrentContext );
	
	/*E double buffering. */
	if ( 2 == BUF_NUM ){
		/*E wait real flip before starting the next draw. */
		cellGcmSetWaitFlip( gCellGcmCurrentContext );
	}
	
	_get_gpu_sync();

	/*E new render target */
//	g_frame_idx = ( g_frame_idx+1 ) % BUF_NUM;
//	_set_render_tgt( g_frame_idx );

	first = 0;
//	vert_indx=0;
//	vert_texture_indx=0;
}



/*E map the buffer. */
int32_t my_disp_mapmem( uint8_t *buffer,
						size_t buf_size )
{

	int32_t gcm_ret;
	uint32_t buf_offset;

	gcm_ret = cellGcmMapMainMemory( buffer,
									buf_size,
									&buf_offset );
	if( CELL_OK != gcm_ret ){
		fprintf( stderr, "disp_mapmem> cellGcmMapMainMemory... fail %d\n",
				gcm_ret );
		return gcm_ret;
	}

	return 0;

}


