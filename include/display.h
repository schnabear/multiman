/* SCE CONFIDENTIAL
 * PLAYSTATION(R)3 Programmer Tool Runtime Library 192.001
 * Copyright (C) 2006 Sony Computer Entertainment Inc.
 * All Rights Reserved.
 */

#include <cell/dbgfont.h>

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

/*E map the buffer. */
int32_t my_disp_mapmem( uint8_t *buffer,
						size_t size );

void _buffer_flip( void );


#define CONSOLE_WIDTH		(76+16)
#define CONSOLE_HEIGHT		(31)

#endif /*E __DISPLAY_H__ */
