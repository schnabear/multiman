#include "mscommon.h"

static int audioInitCell(void);


#ifndef MS_THREADED_SAMPLE

// SPURS information
#define				SPURS_SPU_NUM	1
#define				SPU_THREAD_GROUP_PRIORITY		128
CellSpurs			spurs __attribute__((aligned (128)));
#endif

sys_ppu_thread_t     s_MultiStreamPuThread   = 0;
void *               s_pMultiStreamMemory    = NULL;


#define   CHANNEL   CELL_AUDIO_PORT_8CH
#define   BLOCK     CELL_AUDIO_BLOCK_8

/**********************************************************************************/
// MultiStream thread defines
/**********************************************************************************/
#define STACK_SIZE              (0x4000) // 16 kb
#define EXIT_CODE               (0xbee)
#define SERVER_PRIO             (50)


/**********************************************************************************

audioInitCell
	Initialises low level video and audio.
	This is not MultiStream specific.

	Returns: audio port number returned from cellAudioPortOpen(..)

**********************************************************************************/
static int audioInitCell(void)
{
	int ret = 0;
	unsigned int portNum = -1;


	//	cellMSSystemConfigureSysUtil returns the following data:
	//	Bits 0-3:	Number of available output channels
	//	Bit    4:	Dolby On status
	//	unsigned int retSysUtil = cellMSSystemConfigureSysUtil();
	//	unsigned int numChans = (retSysUtil & 0x0000000F);
	//	unsigned int dolbyOn = (retSysUtil & 0x00000010) >> 4;
	//	printf("Number Of Channels: %u\n", numChans);
	//	printf("Dolby enabled: %u\n", dolbyOn);

	ret = cellAudioInit();
	if (ret !=CELL_OK)	return -1;

	// audio port open.
	audioParam.nChannel = CHANNEL;
	audioParam.nBlock   = BLOCK;

	ret = cellAudioPortOpen(&audioParam, &portNum);
	if (ret != CELL_OK)
	{
		cellAudioQuit();
		return -1;
	}
	 
	// get port config.
	ret = cellAudioGetPortConfig(portNum, &portConfig);
	if (ret != CELL_OK)
	{
		cellAudioQuit();
		return -1;
	}

	cellMSSystemConfigureLibAudio(&audioParam, &portConfig);

	return portNum;
}


/**********************************************************************************

InitialiseAudio

	This function sets up the audio system.

	Requires:	nStreams		Maximum number of streams to be active at any time
				nmaxSubs		Maximum number of sub channels to init in MultiStream
				_nPortNumber	Reference to int - Returns port number from CELL audio init
				_audioParam		Reference to CellAudioPortParam - Returns audio params from CELL audio init
				_portConfig		Reference to CellAudioPortConfig - Returns port configuration from CELL audio init

	Returns:	0	OK
				-1	Error


**********************************************************************************/
long InitialiseAudio( const long nStreams, const long nmaxSubs, int &_nPortNumber , CellAudioPortParam &_audioParam, CellAudioPortConfig &_portConfig)
{
	CellMSSystemConfig cfg;

#ifndef MS_THREADED_SAMPLE
	uint8_t prios[8] = {1, 0, 0, 0, 0, 0, 0, 0};
#endif


	cfg.channelCount=nStreams;
	cfg.subCount=nmaxSubs;
	cfg.dspPageCount=2;//5;
	cfg.flags=CELL_MS_NOFLAGS;//CELL_MS_USE_SMOOTH_ENVELOPE;//CELL_MS_ROUTABLE_STREAMS_FLAG;//3;//CELL_MS_STREAM_AUTOCLOSE;//CELL_MS_ROUTABLE_STREAMS_FLAG;//CELL_MS_NOFLAGS;


    _nPortNumber = audioInitCell();
	if(_nPortNumber < 0)
	{
//		printf("InitialiseAudio: Failed to find valid port number!\n");
		return -1;
	}

	_audioParam = audioParam;
	_portConfig = portConfig;

	// Initialise MultiStream

	int nMemoryNeeded = cellMSSystemGetNeededMemorySize(&cfg);
	s_pMultiStreamMemory = memalign(128, nMemoryNeeded);

#ifndef MS_THREADED_SAMPLE
	InitSPURS();

	// Initialise SPURS MultiStream version
	cellMSSystemInitSPURS(s_pMultiStreamMemory, &cfg, &spurs, &prios[0]);
#else
	// Initialise Threaded MultiStream version (define MS_THREADED_SAMPLE in proprocessor flags)
	cellMSSystemInitSPUThread(s_pMultiStreamMemory, &cfg, 100);
#endif

    return 0;
}

void ShutdownMultiStream()
{
	void* pMemory = cellMSSystemClose();
	assert(pMemory == s_pMultiStreamMemory);

	free( s_pMultiStreamMemory);

	s_pMultiStreamMemory = NULL;
	pMemory = NULL;
}


long StartMultiStreamUpdateThread(void _thread (uint64_t param))
{
   
	// create the MultiStream / libaudio update thread
	int nRet = sys_ppu_thread_create(&s_MultiStreamPuThread, _thread, NULL, SERVER_PRIO, STACK_SIZE, 0, "MultiStream PU Thread"); //SYS_PPU_THREAD_CREATE_JOINABLE
	if(nRet)
	{
		return -1;
	}
    return 0;
}


/**********************************************************************************
InitSPURS
	Initialises SPURS so that SPURS version of MultiStream can then load
**********************************************************************************/
#ifndef MS_THREADED_SAMPLE

void InitSPURS(void)
{
	int ret = -1;
	sys_ppu_thread_t	thread_id;
	int					ppu_thr_prio __attribute__((aligned (128)));  // information for the reverb

	sys_ppu_thread_get_id(&thread_id);
	ret = sys_ppu_thread_get_priority(thread_id, &ppu_thr_prio);
	if(ret != CELL_OK)
	{
//		printf( " ERROR sys_ppu_thread_get_priority = 0x%x\n", ret ); while(1){};
	}
//	printf(" thread_id = %d, ppu_thr_prio = %d\n", (int)thread_id, ppu_thr_prio );
	ret = cellSpursInitialize(&spurs, SPURS_SPU_NUM, SPU_THREAD_GROUP_PRIORITY, ppu_thr_prio-1, 1);
	if(ret != CELL_OK)
	{
//		printf( "******** ERROR cellSpursInitialize = 0x%x\n", ret );
		while(1){};
	}
}

#endif


