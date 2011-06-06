#include "video.h"
#include "graphics.h"
#include "buffers.h"
#include "display.h"




int ret=0;
extern u8 *text_FONT;
/*E sequence done flag */
static bool s_seq_done;

/*E frame buffer object. */
static MyBuffs s_buffs;

/*E sync variables for display. */
static uint32_t s_out_cnt;
static sys_cond_t s_out_cond;
static sys_mutex_t s_out_mutex;

/*E sync variables for picture. */
static uint32_t s_pic_cnt;
static sys_cond_t s_pic_cond;
static sys_mutex_t s_pic_mutex;

/*E sync variables for access unit. */
static uint32_t s_au_cnt;
static sys_cond_t s_au_cond;
static sys_mutex_t s_au_mutex;

/*E libvdec library handle. */
static CellVdecHandle s_vdec_handle;

/*E function for exit trigger. */
static void exit_signal( void );

/*E create and join user threads */
static void running_user_threads( void );

/*E functions for user thread entry. */
static void disp_thread_entry( uint64_t arg );
static void post_thread_entry( uint64_t arg );
static void input_thread_entry( uint64_t arg );
static void launch_thread_entry( uint64_t arg );

/*E functions for setup sequence. */
static int32_t create_sync_primitives( void );
static int32_t open_library( CellVdecResource *resource );

/*E functions for shutdown sequence. */
static void destroy_sync_primitives( void );
static void close_library( CellVdecResource *resource );


/*E callback function for the library. */
static uint32_t lib_callback( CellVdecHandle   handle,
							  CellVdecMsgType  msg_type,
							  int32_t          err_code,
							  void            *cb_arg );

/*E helper function for get access unit */
static uint32_t get_access_unit( uint8_t   *in_addr,
								 size_t     es_size,
								 uint8_t  **au_addr,
								 uint32_t  *au_length );

/*E helper function for get frame rate. */
static uint32_t get_frame_rate( const CellVdecPicItem *pic_item );


/*E main routine. */
int32_t main_video( void )
{

	/*E number of spu for use. */
//	const uint32_t max_use_spu  = 6;
//	const uint32_t max_raw_spu  = 0;

	/*E main thread priority. */
//	const int32_t main_thr_prio = 2000;

	/*E main thread id. */
//	sys_ppu_thread_t main_thr_id;
	sys_ppu_thread_t sample_thr_id;

	printf( "\nSDK sample program for \"libvdec\"\n\n" );

	/*E set exec-flag on. */
	g_exec = true;

	/*E spu initialize. */
//	sys_spu_initialize( max_use_spu, max_raw_spu );


	/*E system call trial. */
//	sys_ppu_thread_get_id( &main_thr_id );

//	sys_ppu_thread_set_priority( main_thr_id, main_thr_prio );


	if( m2v_sample_start( &sample_thr_id ) ){

		MY_DBG( "m2v_sample_start failed.\n" );

	}else{

		while( g_exec ){
			/*E check arrival of sysutil event. */
			if( CELL_OK != cellSysutilCheckCallback() ){
				MY_DBG( "cellSysutilCheckCallback failed.\n" );
				break;
			}
			/*E sleep each frame interval. */
			sys_timer_usleep( 33367 );
		}

		/*E wait to finish the sample. */
		m2v_sample_return( sample_thr_id );

	}

	MY_DBG( "exit process.\n" );

//	sys_process_exit( 0 );
	return 0;

}



/*E global function for start sample. */
int32_t m2v_sample_start( sys_ppu_thread_t *thread_id )
{

	int32_t sys_ret;
	const size_t stack_size = 32768;
	const int32_t thread_priority = 1800;


	/*E create launcher thread. */
	sys_ret = sys_ppu_thread_create( thread_id, launch_thread_entry,
									 NULL, thread_priority, stack_size,
									 SYS_PPU_THREAD_CREATE_JOINABLE, "launch_thread_entry" );
	if( CELL_OK != sys_ret ){
		MY_DBG( "sys_ppu_thread_create:0x%x failed.\n", sys_ret );
		return -1;
	}

	MY_DBG( "create launch_thread_entry ... thread id:%d priority:%d\n",
			(int)*thread_id, thread_priority );

	return 0;

}

/*E global function for join sample. */
int32_t m2v_sample_return( sys_ppu_thread_t thread_id )
{

	int32_t sys_ret;
	uint64_t thr_ret;

	/*E join launcher thread. */
	sys_ret = sys_ppu_thread_join( thread_id, &thr_ret );
	if( CELL_OK != sys_ret ){
		MY_DBG( "sys_ppu_thread_join:0x%x failed.\n", sys_ret );
		ret = -1;
	}

	return ret;

}

/*E sample threads launcher */
static void launch_thread_entry( uint64_t arg )
{

	(void)arg;
	int32_t i_cnt;
	size_t frame_buf_size;
	const uint32_t align_mask = 0xfffff;
	const uint32_t frame_buf_align = 0x100000;
	CellVdecResource resource = { NULL, 0, 0, 0, 0, 0 };


	MY_DBG( "display configured ... WIDTH:%d HEIGHT:%d\n",
			MY_DISPLAY_WIDTH, MY_DISPLAY_HEIGHT );

	/*E frame buffer memory size. */
	frame_buf_size = MY_FRAME_WIDTH*MY_FRAME_HEIGHT*4;

	/*E buffer must be integer multiple for libgcm. */
	frame_buf_size = ( frame_buf_size+align_mask )&( ~align_mask );

	/*E create frame buffer object. */
	if( my_buffs_create(&s_buffs, MY_FRAME_NUM, frame_buf_size, frame_buf_align) ){
		MY_DBG( "my_buffs_create failed.\n" );
		goto exitsample;
	}else
		/*E activate buffer object. */
		my_buffs_open( &s_buffs );

	/*E map main memory so RSX can access it. */
	for( i_cnt=0; i_cnt < MY_FRAME_NUM; i_cnt++ )
	{
		s_buffs.element[i_cnt].buffer=NULL;
		my_disp_mapmem( (u8*)(s_buffs.element[i_cnt].buffer), frame_buf_size );
	}


	/*E initialize sync primitives. */
	if( create_sync_primitives() ){
		MY_DBG( "create_sync_primitives failed.\n" );
		goto exitsample;
	}

	/*E library open process. */
	if( open_library(&resource) )
		MY_DBG( "open_library failed.\n" );
	else
		/*E threads create & join. */
		running_user_threads();

	/*E library close process. */
	close_library( &resource );

  exitsample:

	/*E destroy sync primitives. */
	destroy_sync_primitives();

	/*E destroy frame buffer object. */
	my_buffs_destroy( &s_buffs );


	MY_DBG( "thread exit.\n" );

	/*E exit this thread. */
	sys_ppu_thread_exit( 0 );

}

/*E function for close library. */
static int32_t open_library( CellVdecResource *resource )
{

	/*E libvdec return code. */
	int32_t lib_ret;

	/*E libvdec codec type. */
	CellVdecType dec_type;

	/*E libvdec attributes. */
	CellVdecAttr dec_attr;

	/*E libvdec threads priority. */
	const int32_t vdec_spu_prio = 100;
	const int32_t vdec_ppu_prio = 1000;

	/*E libvdec threads stack size. */
	const size_t lib_stack_size = 8192;

	/*E number of spu for libvdec. */
	const uint32_t vdec_spu_num = 1;

	/*E select MPEG2 CODEC. */
	dec_type.codecType = CELL_VDEC_CODEC_TYPE_MPEG2;

	/*E set profile and level. */
	dec_type.profileLevel = CELL_VDEC_MPEG2_MP_HL;

	/*E get library attribute information. */
	lib_ret = cellVdecQueryAttr( &dec_type, &dec_attr );

	if( CELL_OK != lib_ret ){
		MY_DBG( "cellVdecQueryAttr:0x%x failed.\n", lib_ret );
		return -1;
	}

	MY_DBG( "library version %08x:%08x\n",
			dec_attr.decoderVerUpper,
			dec_attr.decoderVerLower );

	MY_DBG( "required memory size:%d bytes\n",
			dec_attr.memSize );

	/*E allocate memory for libvdec. */
	resource->memAddr = malloc( dec_attr.memSize );
	if( NULL == resource->memAddr ){
		MY_DBG( "malloc failed.\n" );
		return -1;
	}

	/*E set other configuration parameter. */
	resource->numOfSpus = vdec_spu_num;
	resource->memSize = dec_attr.memSize;
	resource->ppuThreadPriority = vdec_ppu_prio;
	resource->spuThreadPriority = vdec_spu_prio;
	resource->ppuThreadStackSize = lib_stack_size;

	/*E set user defined callback. */
	CellVdecCb regist_cb;
	regist_cb.cbFunc = lib_callback;

	/*E open library and get library handle. */
	lib_ret = cellVdecOpen( &dec_type, resource, &regist_cb, &s_vdec_handle );
	if( CELL_OK != lib_ret ){
		MY_DBG( "cellVdecOpen:0x%x failed.\n", lib_ret );
		return -1;
	}

	MY_DBG( "use %d SPU / priority PPU:%d SPU:%d / stack size:%d bytes\n",
			resource->numOfSpus,
			resource->ppuThreadPriority,
			resource->spuThreadPriority,
			resource->ppuThreadStackSize );

	return 0;

}

/*E function for close library. */
static void close_library( CellVdecResource *resource )
{

	int32_t lib_ret;

	/*E close library. */
	lib_ret = cellVdecClose( s_vdec_handle );

	if( CELL_OK != lib_ret )
		MY_DBG( "cellVdecClose:0x%x failed.\n", lib_ret );

	/*E free the memory area for library. */
	if( NULL != resource->memAddr )
		free( resource->memAddr );

}

/*E execute all threads. */
static void running_user_threads( void )
{

	/*E variables for threads. */
	uint64_t thr_ret;
	sys_ppu_thread_t disp_thr_id;
	sys_ppu_thread_t post_thr_id;
	sys_ppu_thread_t input_thr_id;

	/*E sample threads priority. */
	const int32_t disp_thr_prio  = 1200;
	const int32_t post_thr_prio  = 1400;
	const int32_t input_thr_prio = 1600;


//	generateTexture( text_FONT, 900, 280, (char *)"    This is a test subtitle line to show player POC (multiPLAY)");

	/*E sample threads stack size. */
	const size_t app_stack_size  = 32768;

	/*E create display thread. */
	if( CELL_OK !=
		sys_ppu_thread_create( &disp_thr_id, disp_thread_entry,
							   NULL, disp_thr_prio, app_stack_size,
							   SYS_PPU_THREAD_CREATE_JOINABLE, "disp_thread" ) ){
		MY_DBG( "create disp_thread failed.\n" );
		return;
	}else
		MY_DBG( "disp_thread ... thread id:%d priority:%d\n",
				(int)disp_thr_id, disp_thr_prio );

	/*E create post-proc thread. */
	if( CELL_OK !=
		sys_ppu_thread_create( &post_thr_id, post_thread_entry,
							   NULL, post_thr_prio, app_stack_size,
							   SYS_PPU_THREAD_CREATE_JOINABLE, "post_thread" ) ){
		MY_DBG( "create post_thread failed.\n" );
		return;
	}else
		MY_DBG( "post_thread ... thread id:%d priority:%d\n",
				(int)post_thr_id, post_thr_prio );

	/*E create input thread. */
	if( CELL_OK !=
		sys_ppu_thread_create( &input_thr_id, input_thread_entry,
							   NULL, input_thr_prio, app_stack_size,
							   SYS_PPU_THREAD_CREATE_JOINABLE, "input_thread" ) ){
		MY_DBG( "create input_thread failed.\n" );
		return;
	}else
		MY_DBG( "input_thread ... thread id:%d priority:%d\n",
				(int)input_thr_id, input_thr_prio );

	MY_DBG( "all threads have been created.\n" );

	/*E join input thread. */
	if( CELL_OK != sys_ppu_thread_join(input_thr_id, &thr_ret) )
		MY_DBG( "join input_thread failed.\n" );

	/*E join post-proc thread. */
	if( CELL_OK != sys_ppu_thread_join(post_thr_id, &thr_ret) )
		MY_DBG( "join post_thread failed.\n" );

	/*E join display thread. */
	if( CELL_OK != sys_ppu_thread_join(disp_thr_id, &thr_ret) )
		MY_DBG( "join disp_thread failed.\n" );

	MY_DBG( "all threads have finished.\n" );

	return;

}

/*E input thread entry. */
static void input_thread_entry( uint64_t arg )
{

	(void)arg;
	FILE *fp = NULL;
	size_t stream_size, real_size, real_position;
	uint8_t *au_buffer;
	uint8_t *in_buffer = NULL;

	/*E open the stream file. */
	fp = fopen( MY_FILE_NAME, "rb" );
	if( NULL == fp ){
		MY_DBG( "fopen failed.\n" );
		goto exitinput;
	}

	/*E set fp to end of file. */
	if( fseek(fp, 0L, SEEK_END) ){
		MY_DBG( "fseek failed.\n" );
		goto exitinput;
	}

	/*E get total file size. */
	stream_size = ftell( fp );
	real_size = stream_size;
	MY_DBG( "file size:%d bytes\n", stream_size );

	/*E check file size if exceeded or error. */
	if( MY_READ_SIZE_MAX < stream_size ){
		stream_size = MY_READ_SIZE_MAX;
	}else if( 0 > (int)stream_size ){
		MY_DBG( "ftell failed.\n" );
		goto exitinput;
	}

	MY_DBG( "read size:%d bytes\n\n", stream_size );

	/*E allocate stream buffer.(aligned 128 bytes.) */
	in_buffer = (uint8_t*)memalign( 128, stream_size );

	if( NULL == in_buffer ){
		MY_DBG( "memalign failed.\n" );
		goto exitinput;
	}

	/*E reset fp. */
	rewind( fp );

	/*E read stream data from the file. */
	if( stream_size != (size_t)fread( in_buffer, 1, stream_size, fp ) ){
		MY_DBG( "fread failed.\n" );
		goto exitinput;
	}

	fclose(fp);

	real_position=stream_size;

	int32_t lib_ret;
	uint32_t au_cnt;
	uint32_t au_length;
	uint32_t read_length;
	uint32_t parse_length;
	CellVdecAuInfo au_info;
	CellVdecDecodeMode mode;

  startseq:

	MY_DBG( "start decode sequence ...\n" );

	/*E reset decoder for new sequence. */
	lib_ret = cellVdecStartSeq( s_vdec_handle );

	if( CELL_OK != lib_ret ){
		MY_DBG( "cellVdecStartSeq:0x%x failed.\n", lib_ret );
		goto exitinput;
	}

	au_cnt = 0;
	s_au_cnt = 0;
	read_length = 0;
	s_seq_done = false;

	/*E decoding loop. */
	while( g_exec ){


		/*E start to get access unit. */
		parse_length = get_access_unit( in_buffer+read_length,
										stream_size-read_length,
										&au_buffer, &au_length );

		if(!parse_length)
		{
//			eofs = stream_size-read_length;
//			memmove(in_buffer, in_buffer+read_length, eofs);
//			read_file=fread( in_buffer+eofs, 1, stream_size-eofs, fp );
//			if(read_file==0 || read_file==NULL) break;
//			read_length=eofs;//+parse_length;


			//fseek(fp, real_position, SEEK_SET);
			//read_file=fread( in_buffer, 1, stream_size, fp );
			//if(read_file==0 || read_file==NULL) break;
//			real_position+=read_file;
			real_position=0;
			read_length=0;

			parse_length = get_access_unit( in_buffer+read_length,
										stream_size-read_length,
										&au_buffer, &au_length );


		}


		if( !parse_length ) break;


		/*E set values for next parse. */
		read_length += parse_length;

		/*E set values for decode access unit. */
		au_info.size      = au_length;
		au_info.startAddr = au_buffer;

		/*E supplemental values for decode access unit. */
		au_info.userData          = 0; /*E free use. */
		au_info.codecSpecificData = 0; /*E now invalid. */
		au_info.pts.lower         = CELL_VDEC_PTS_INVALID;
		au_info.pts.upper         = CELL_VDEC_PTS_INVALID;
		au_info.dts.lower         = CELL_VDEC_DTS_INVALID;
		au_info.dts.upper         = CELL_VDEC_DTS_INVALID;

		/*E set the decode mode to normal. */
		mode = CELL_VDEC_DEC_MODE_NORMAL;

	  decodeau:

		/*E start decode access unit. */
		lib_ret = cellVdecDecodeAu( s_vdec_handle, mode, &au_info );

		if( CELL_OK != lib_ret ){
			if( CELL_VDEC_ERROR_BUSY == lib_ret ){
				/*E wait au-done signal, in case of busy. */
				sys_mutex_lock( s_au_mutex, 0 );
				while( (au_cnt>s_au_cnt) && g_exec )
					sys_cond_wait( s_au_cond, 0 );
				sys_mutex_unlock( s_au_mutex );
				if( !g_exec ) break;
				goto decodeau;
			}else{
				MY_DBG( "cellVdecDecodeAu:0x%x failed.\n", lib_ret );
				break;
			}
		}

		au_cnt++; /*E increment decoded counter. */

	}

	/*E flush remained picture and end sequence. */
	lib_ret = cellVdecEndSeq( s_vdec_handle );

	if( CELL_OK != lib_ret ){
		MY_DBG( "cellVdecEndSeq:0x%x failed.\n", lib_ret );
		goto exitinput;
	}

	/*E wait for sequence done. */
	sys_mutex_lock( s_out_mutex, 0 );
	while( g_exec && !s_seq_done )
		sys_cond_wait( s_out_cond, 0 );
	sys_mutex_unlock( s_out_mutex );
	if( !g_exec )
		goto exitinput;

	/*E wait for all frames displayed. */
	sys_mutex_lock( s_out_mutex, 0 );
	while( g_exec &&
		   (s_pic_cnt != s_out_cnt) )
		sys_cond_wait( s_out_cond, 0 );
	sys_mutex_unlock( s_out_mutex );
	if( !g_exec )
		goto exitinput;

	MY_DBG( "all frames have been displayed.\n\n" );

	/*E enable loop play back. */

//	read_file=fread( in_buffer, 1, stream_size, fp );
//	if(read_file==0 || read_file==NULL) goto exitinput;
//	if(read_file>0 && read_file<stream_size) stream_size=read_file;
	read_length=0;
	rewind( fp );
	real_position=0;
	goto startseq;

  exitinput:

	/*E close the input file. */
//	if( NULL != fp )
//		fclose( fp );

	/*E free the input buffer. */
	if( NULL != in_buffer )
		free( (void*)in_buffer );

	exit_signal();

	MY_DBG( "thread exit.\n" );

	sys_ppu_thread_exit( 0 );

}

/*E post process thread entry. */
static void post_thread_entry( uint64_t arg )
{

	(void)arg;
	int32_t lib_ret;
	uint32_t pic_cnt;
	MyBuffsElem *buff_elem;
	CellVdecPicFormat pic_format;
	const CellVdecPicItem *pic_item;

	pic_cnt = 0;
	pic_format.alpha = 0;
	pic_format.formatType = CELL_VDEC_PICFMT_RGBA32_ILV;
	pic_format.colorMatrixType = CELL_VDEC_COLOR_MATRIX_TYPE_BT709;

	while( g_exec ){

		pic_cnt++;

		/*E wait for signal from pic-out callback. */
		sys_mutex_lock( s_pic_mutex, 0 );
		while( (pic_cnt>s_pic_cnt) && g_exec )
			sys_cond_wait( s_pic_cond, 0 );
		sys_mutex_unlock( s_pic_mutex );
		if( !g_exec ) break;

		/*E get picture attribute information. */
		lib_ret = cellVdecGetPicItem( s_vdec_handle, &pic_item );

		if( CELL_OK != lib_ret ){
			MY_DBG( "cellVdecGetPicItem:0x%x failed.\n", lib_ret );
			continue;
		}

		/*E check picture status whether error or not. */
		if( CELL_VDEC_ERROR_PIC == pic_item->status )
			MY_DBG( "error detected in picture#%d\n", pic_cnt );

		/*E check skipped picture, if you won't display it. */
		if( CELL_VDEC_PICITEM_ATTR_SKIPPED == pic_item->attr ){

			MY_DBG( "skip picture#%d ... proceed next.\n", pic_cnt );

			/*E skip current picture by NULL buffer, then proceed next. */
			cellVdecGetPicture( s_vdec_handle, &pic_format, NULL );

			/*E update the output counter. */
			sys_mutex_lock( s_out_mutex, 0 );
			s_out_cnt++;
			sys_mutex_unlock( s_out_mutex );

			continue;
		}

		/*E get empty buffer for display. */
		if( my_buffs_prepare( &s_buffs, &buff_elem ) ) break;

		/*E get frame output timing information. */
		buff_elem->interval = get_frame_rate( pic_item );
		if( !buff_elem->interval ){
			if( 1 == pic_cnt ) MY_DBG( "get_frame_rate failed.\n" );
			buff_elem->interval = MY_FPS_30;
		}

		/*E get frame size information. */
		buff_elem->width  = ((CellVdecMpeg2Info*)(pic_item->picInfo))->horizontal_size;
		buff_elem->height = ((CellVdecMpeg2Info*)(pic_item->picInfo))->vertical_size;

		/*E get current picture data, then decoder remove the picture immediately. */
		lib_ret = cellVdecGetPicture( s_vdec_handle, &pic_format, buff_elem->buffer );

		if( CELL_OK != lib_ret ){
			MY_DBG( "cellVdecGetPicture:0x%x failed.\n", lib_ret );
			continue;
		}

		if( 1 == pic_cnt ){
			MY_DBG( "initial frame info ... width:%d height:%d fps:%d\n",
					buff_elem->width, buff_elem->height, buff_elem->interval );
		}

		/*E send picture buffer for display output. */
		if( my_buffs_send( &s_buffs ) ) break;

	}

	MY_DBG( "thread exit.\n" );

	exit_signal();

	sys_ppu_thread_exit( 0 );

}

/*E display thread entry. */
static void disp_thread_entry( uint64_t arg )
{

	(void)arg;
	uint32_t i_cnt;
	uint32_t pull_down_toggle = 0;
	uint32_t primary_repeat_max = 0;
	uint32_t secondary_repeat_max = 0;

	MyBuffsElem *buff_elem;

	while( g_exec ){

		/*E get oldest picture buffer. */
		if( my_buffs_receive(&s_buffs, &buff_elem) ) break;

		/*E get frame output timing. */
		if( MY_FPS_60 == buff_elem->interval ){
			primary_repeat_max = 1;
			secondary_repeat_max = 0;
		}else
		if( MY_FPS_30 == buff_elem->interval ){
			primary_repeat_max = 2;
			secondary_repeat_max = 0;
		}else
		if( MY_FPS_24 == buff_elem->interval ){
			primary_repeat_max = 2;
			pull_down_toggle = ~pull_down_toggle;
			if( pull_down_toggle )
				secondary_repeat_max = 1;
			else
				secondary_repeat_max = 0;
		}else{
			MY_DBG( "unknown interval:%d\n", buff_elem->interval );
			break;
		}

		for(i_cnt = 0; i_cnt < primary_repeat_max; i_cnt++){

//			my_disp_settex( text_FONT, 900, 280 );
//			display_img(510, 900, 900, 280, 1280, 280, 0.0f, 1280, 280);

			//my_disp_settex( buff_elem->buffer, buff_elem->width, buff_elem->height );

//			char sub_line[256];
//			sprintf(sub_line, (char *)"This is a test subtitle line to show player POC (multiPLAY) (%.f frames)", (double)s_pic_cnt);
//			generateTexture( buff_elem->buffer, buff_elem->width, buff_elem->height, sub_line);
//			display_img(320, 180, 1280, 720, buff_elem->width, buff_elem->height, 0.0f, buff_elem->width, buff_elem->height);

			//display_img(0, 0, 1920, 1080, buff_elem->width, buff_elem->height, 0.0f, buff_elem->width, buff_elem->height);

//			display_img(30, 30, 480, 270, 1920, 1080, 0.0f, 1920, 1080);
//			display_img(30, 790, 480, 270, 1920, 1080, 0.0f, 1920, 1080);

//			display_img(1400, 30, 480, 270, 1920, 1080, 0.0f, 1920, 1080);
//			display_img(1400, 790, 480, 270, 1920, 1080, 0.0f, 1920, 1080);


//			cellDbgFontPrintf( 0.5f, 0.5f, 1.0f ,0xffffffff, "Bla bla text"); 
//			cellDbgFontDrawGcm();	

//			_buffer_flip();
		}

		for(i_cnt = 0; i_cnt < secondary_repeat_max; i_cnt++){
			//my_disp_settex( buff_elem->buffer, buff_elem->width, buff_elem->height );
//			display_img(0, 0, buff_elem->width, buff_elem->height, buff_elem->width, buff_elem->height, 0.0f, buff_elem->width, buff_elem->height);
			//display_img(0, 0, 1920, 1080, buff_elem->width, buff_elem->height, 0.0f, buff_elem->width, buff_elem->height);
//			my_disp_settex( text_FONT, buff_elem->width, 280 );
//			display_img(0, 800, buff_elem->width, 280, buff_elem->width, 280, 0.0f, buff_elem->width, 280);

//			display_img(800, 200, 320, 176, 320, 176, 0.0f, 320, 320);
//			my_disp_display();

//			_buffer_flip();
		
		}

		/*E release current picture buffer. */
		if( my_buffs_release(&s_buffs) ) break;

		/*E send display done signal. */
		sys_mutex_lock( s_out_mutex, 0 );
		s_out_cnt++;
		sys_cond_signal( s_out_cond );
		sys_mutex_unlock( s_out_mutex );

	}

	MY_DBG( "displayed %d pictures.\n", s_out_cnt );

	MY_DBG( "thread exit.\n" );

	exit_signal();

	sys_ppu_thread_exit( 0 );

}

/*E callback function for library. */
static uint32_t lib_callback( CellVdecHandle   handle,
							 CellVdecMsgType  msg_type,
							 int32_t          err_code,
							 void            *cb_arg )
{

	(void)handle;
	(void)cb_arg;

	switch( msg_type ){

	case CELL_VDEC_MSG_TYPE_AUDONE:

		/*E send au-done signal. */
		sys_mutex_lock( s_au_mutex, 0 );
		s_au_cnt++;
		sys_cond_signal( s_au_cond );
		sys_mutex_unlock( s_au_mutex );

		if( CELL_VDEC_ERROR_AU == err_code )
			MY_DBG( "AU#%d was disregarded.\n", s_au_cnt );

		break;

	case CELL_VDEC_MSG_TYPE_PICOUT:

		/*E send pic-out signal. */
		sys_mutex_lock( s_pic_mutex, 0 );
		s_pic_cnt++;
		sys_cond_signal( s_pic_cond );
		sys_mutex_unlock( s_pic_mutex );

		break;

	case CELL_VDEC_MSG_TYPE_SEQDONE:

		MY_DBG( "got sequence done callback.\n" );

		/*E send sequence done signal. */
		sys_mutex_lock( s_out_mutex, 0 );
		s_seq_done = true;
		sys_cond_signal( s_out_cond );
		sys_mutex_unlock( s_out_mutex );

		break;

	case CELL_VDEC_MSG_TYPE_ERROR:

		MY_DBG( "got fatal error callback.\n" );

		/*E force exit. */
		exit_signal();

		break;

	default:

		/*E this is impossible case. */
		MY_DBG( "unknown callback.\n" );

		/*E force exit. */
		exit_signal();

	}

	return 0;

}

/*E helper function for get access unit */
static uint32_t get_access_unit( uint8_t   *in_addr,
								 size_t     es_size,
								 uint8_t  **au_addr,
								 uint32_t  *au_length )
{

	bool detect;
	uint8_t *parse_head;
	uint32_t start_point;
	uint32_t read_length;

	detect = false;
	*au_addr = NULL;
	start_point = 0;
	read_length = 0;

	/*E parse loop. */
	while( g_exec ){

		parse_head = in_addr + read_length;

		/*E check the end of stream. */
		if( es_size < (read_length + 4) ) return 0;

		if( parse_head[0] || parse_head[1] || (1 != parse_head[2]) ){
			read_length++; continue;
		}

		if ( (0xb8 == parse_head[3]) || /*E group start code. */
			 (0xb7 == parse_head[3]) || /*E sequence end code. */
			 (0x00 == parse_head[3]) || /*E picture start code. */
			 (0xb3 == parse_head[3]) ){ /*E sequence start code. */
			
			if( detect ){

				if( 0xb7 == parse_head[3] ) /*E sequence end code. */
					read_length += 4;

				*au_length = read_length - start_point;

				return read_length;

			}else{

				if( 0x00 == parse_head[3] ) /*E picture start code. */
					detect = true;

				if( NULL == *au_addr ){
					start_point = read_length;
					*au_addr = in_addr + start_point;
				}

			}

		}
		read_length += 4;
	}

	return 0;

}

/*E helper function for get frame rate. */
static uint32_t get_frame_rate( const CellVdecPicItem *pic_item )
{

	uint8_t frame_rate;

	switch( pic_item->codecType ){
	case CELL_VDEC_CODEC_TYPE_MPEG2:
		frame_rate = ((CellVdecMpeg2Info*)(pic_item->picInfo))->frame_rate_code;
		if( CELL_VDEC_MPEG2_FRC_24000DIV1001 == frame_rate ||
			CELL_VDEC_MPEG2_FRC_24 == frame_rate ){
			return MY_FPS_24;
		}
		if( CELL_VDEC_MPEG2_FRC_25 == frame_rate ||
			CELL_VDEC_MPEG2_FRC_30000DIV1001 == frame_rate ||
			CELL_VDEC_MPEG2_FRC_30 == frame_rate ){
			return MY_FPS_30;
		}
		if( CELL_VDEC_MPEG2_FRC_50 == frame_rate ||
			CELL_VDEC_MPEG2_FRC_60000DIV1001 == frame_rate ||
			CELL_VDEC_MPEG2_FRC_60 == frame_rate ){
			return MY_FPS_60;
		}
		break;
	default:
		break;
	}

	return 0;

}

/*E create sync primitives. */
static int32_t create_sync_primitives( void )
{

	/*E nitialize synchronous primitives. */
	sys_cond_attribute_t cond_attr;
	sys_mutex_attribute_t mutex_attr;

	sys_cond_attribute_initialize( cond_attr );
	sys_mutex_attribute_initialize( mutex_attr );

	if( CELL_OK != sys_mutex_create(&s_au_mutex, &mutex_attr) ){
		MY_DBG( "create s_au_mutex failed.\n" );
		return -1;
	}

	if( CELL_OK != sys_mutex_create(&s_pic_mutex, &mutex_attr) ){
		MY_DBG( "create s_pic_mutex failed.\n" );
		return -1;
	}

	if( CELL_OK != sys_mutex_create(&s_out_mutex, &mutex_attr) ){
		MY_DBG( "create s_out_mutex failed.\n" );
		return -1;
	}

	if( CELL_OK != sys_cond_create(&s_au_cond, s_au_mutex, &cond_attr) ){
		MY_DBG( "create s_au_cond failed.\n" );
		return -1;
	}

	if( CELL_OK != sys_cond_create(&s_pic_cond, s_pic_mutex, &cond_attr) ){
		MY_DBG( "create s_pic_cond failed.\n" );
		return -1;
	}

	if( CELL_OK != sys_cond_create(&s_out_cond, s_out_mutex, &cond_attr) ){
		MY_DBG( "create s_out_cond failed.\n" );
		return -1;
	}

	return 0;

}

/*E destroy sync primitives. */
static void destroy_sync_primitives( void )
{

	if( CELL_OK != sys_cond_destroy(s_au_cond) )
		MY_DBG( "destroy s_au_cond failed.\n" );

	if( CELL_OK != sys_cond_destroy(s_pic_cond) )
		MY_DBG( "destroy s_pic_cond failed.\n" );

	if( CELL_OK != sys_cond_destroy(s_out_cond) )
		MY_DBG( "destroy s_out_cond failed.\n" );

	if( CELL_OK != sys_mutex_destroy(s_au_mutex) )
		MY_DBG( "destroy s_au_mutex failed.\n" );

	if( CELL_OK != sys_mutex_destroy(s_pic_mutex) )
		MY_DBG( "destroy s_pic_mutex failed.\n" );

	if( CELL_OK != sys_mutex_destroy(s_out_mutex) )
		MY_DBG( "destroy s_out_mutex failed.\n" );

}

/*E helper function for exit sample. */
void exit_signal( void )
{
	g_exec = false;
	my_buffs_close( &s_buffs );
	sys_cond_signal( s_au_cond );
	sys_cond_signal( s_pic_cond );
	sys_cond_signal( s_out_cond );
}

/*E global function for exit sample. */
void m2v_sample_exit( void )
{
	exit_signal();
}


