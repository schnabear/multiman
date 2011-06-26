/*
# Portions of code and idea for localization by:
# (c) 2006 Eugene Plotnikov <e-plotnikov@operamail.com>
# SMS Media Player for PS2 / PS2DEV Open Source Project
# Used: SMS_Locale.h, SMS_Locale.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "language.h"

static unsigned char s_pDebugMode	[]	= "Debug Mode";
static unsigned char s_pQuit0		[]	= "Quit to XMB\xE2\x84\xA2";
static unsigned char s_pQuit1		[]	= "Quit to XMB\xE2\x84\xA2 screen?";
static unsigned char s_pRestart0	[]	= "Restart multiMAN?";

static unsigned char s_pWarnFTP		[]	= "There are active FTP connections!\n\nAre you sure you want to continue and abort FTP transfers?";

static unsigned char s_pWarnSNES	[]	= "To play SNES games you must install the latest version of SNEX9x for the PS3\xE2\x84\xA2";
static unsigned char s_pWarnGEN		[]	= "To play Genesis+ GX games you must install the latest version of GENESIS Emulator for the PS3\xE2\x84\xA2";
static unsigned char s_pWarnFCEU	[]	= "To play NES/FCE Ultra games you must install the latest version of FCEU Emulator for the PS3\xE2\x84\xA2";
static unsigned char s_pWarnVBA		[]	= "To play GameBoy/Advanced games you must install the latest version of VBA Emulator for the PS3\xE2\x84\xA2";
static unsigned char s_pWarnFBA		[]	= "To play FBA games you must install the latest version of FB Alpha/Next for the PS3\xE2\x84\xA2";

static unsigned char s_pCopy0		[]	= "Copying %d files (%1.3f GB), please wait...";
static unsigned char s_pCopy1		[]	= "Copying %d files, please wait...";
static unsigned char s_pCopy2		[]	= "Creating links for %d files (%1.3f GB), please wait...";
static unsigned char s_pCopy3		[]	= "Installing Game Files to HDD cache, please wait...";
static unsigned char s_pCopy4		[]	= "Copying over %d+ files (%1.3f+ GB), please wait...";
static unsigned char s_pCopy5		[]	= "Copying, please wait!";
static unsigned char s_pCopy6		[]	= "Copying file, please wait...";

static unsigned char s_pCopy7		[]	= "Do you want to create a shadow copy of the selected folder?\n\nSource: [%s]\n\nDestination: [/dev_hdd0/G/<special_pkg_id>";
static unsigned char s_pCopy8		[]	= "Do you want to create a shadow copy of the selected folder?\n\nSource: [%s]\n\nDestination: [%s/%s]";
static unsigned char s_pCopy9		[]	= "Do you want to copy the selected folders?\n\nSource: [%s]\n\nDestination: [%s]";
static unsigned char s_pCopy10		[]	= "Do you want to enable BD-ROM GAME DISC mirror on external USB?\n\nSource: [%s]\n\nDestination: [Emulated BD-ROM on USB device]";
static unsigned char s_pCopy11		[]	= "Do you want to copy the selected file?\n\nSource: [%s]\n\nDestination: [%s/%s]";
static unsigned char s_pCopy12		[]	= "Do you want to copy selected %i files?\n\nSource: [%s]\n\nDestination: [%s]";

static unsigned char s_pNetCopy0	[]	= "Copying network folder (%i files in %i folders) from [%s], please wait!";
static unsigned char s_pNetCopy1	[]	= "Copying file to network host [%s], please wait!";
static unsigned char s_pNetCopy2	[]	= "Copying network file from [%s], please wait!";
static unsigned char s_pNetCopy3	[]	= "Copying local folder (%i files in %i folders) to network host [%s], please wait!";
static unsigned char s_pNetCopy4	[]	= "Transferred %.2f of %.2f MB. Remaining: %imin %2.2isec";

static unsigned char s_pMove0		[]	= "Do you want to move the selected folders?\n\nSource: [%s]\n\nDestination: [%s]";
static unsigned char s_pMove1		[]	= "Do you want to move the selected file?\n\nSource: [%s]\n\nDestination: [%s/%s]";
static unsigned char s_pMove2		[]	= "Do you want to move selected %i files?\n\nSource: [%s]\n\nDestination: [%s]";
static unsigned char s_pMove3		[]	= "Moving, please wait!";
static unsigned char s_pMove4		[]	= "Moving file, please wait...";

static unsigned char s_pWarnINET	[]	= "Internet connection is not available or an error has occured!";
static unsigned char s_pErrSRV0		[]	= "Error occured while contacting the server!\n\nPlease try again later.";
static unsigned char s_pErrUPD0		[]	= "Error occured while downloading the update!\n\nPlease try again later.";
static unsigned char s_pErrUPD1		[]	= "Error occured while contacting the update server!\n\nPlease try again later.";

static unsigned char s_pErrMNT		[]	= "Error occured while parsing device mount table!";
static unsigned char s_pErrMVGAME	[]	= "Error occured while moving game to new location!";

static unsigned char s_pErrMVAV		[]	= "Error (%08X) occured while setting active AVCHD folder.\n\nCannot rename [%s] to [%s]";

static unsigned char s_pDownUpdate	[]	= "Downloading update data, please wait!";
static unsigned char s_pDownCover	[]	= "Downloading cover, please wait!";
static unsigned char s_pDownFile	[]	= "Downloading file, please wait!";
static unsigned char s_pDownTheme	[]	= "Downloading theme, please wait!";

static unsigned char s_pDownMSG0	[]	= "\nDownloaded %.1f of %.3f MB. Remaining: %imin %2.2isec (/\\ to cancel)\nSave path: %s";
static unsigned char s_pDownMSG1	[]	= "Downloaded %1.2f of %1.2f KB. Remaining: %imin %2.2isec\n\nPress /\\ to cancel download";
static unsigned char s_pDownMSG2	[]	= "Downloaded %1.2f of %1.2f KB. Remaining: %imin %2.2isec";

static unsigned char s_pParamVer	[]	= "Game requires PS3 firmware version %.2f.\n\nDo you want to change PARAM.SFO version to %.2f?";

static unsigned char s_pLastPlay	[]	= "Setting data for last played game, please wait...";
static unsigned char s_pSetAccess	[]	= "Setting access permissions, please wait...";
static unsigned char s_pSetAccess1	[]	= "Setting access permissions, please wait!\n\nThis operation will be performed only once.";

static unsigned char s_pPreProcess	[]	= "Pre-processing required for this title.\n\nDo you want to install required data to internal HDD?";

static unsigned char s_pNoSpace0	[]	= "Not enough space to complete cache operation! (Available: %.2fMB)\n\nAdditional %.2fMB of free space required!";
static unsigned char s_pNoSpace1	[]	= "Not enough space on destination drive! (Available: %.2fMB)\n\nAdditional %.2fMB of free space required!";

static unsigned char s_pErrNoMemWeb	[]	= "Not enough memory to launch web browser!\n\nPlease restart multiMAN and try again.";
static unsigned char s_pErrNoMem	[]	= "Please restart multiMAN from PS3 XMB\xE2\x84\xA2";

/*
static unsigned char s_p		[]		= 
static unsigned char s_p		[]		= 
static unsigned char s_p		[]		= 
static unsigned char s_p		[]		= 
static unsigned char s_p		[]		= 
static unsigned char s_p		[]		= 
static unsigned char s_p		[]		= 
static unsigned char s_p		[]		= 
static unsigned char s_p		[]		= 
static unsigned char s_p		[]		= 
*/


static MMString s_MMStringDef[] = {
 { sizeof ( s_pDebugMode  ) - 1, s_pDebugMode	}, //0
 { sizeof ( s_pQuit0	  ) - 1, s_pQuit0		}, //1
 { sizeof ( s_pQuit1	  ) - 1, s_pQuit1		},
 { sizeof ( s_pRestart0	  ) - 1, s_pRestart0	},
 { sizeof ( s_pWarnFTP	  ) - 1, s_pWarnFTP		},
 { sizeof ( s_pWarnSNES	  ) - 1, s_pWarnSNES	},
 { sizeof ( s_pWarnGEN	  ) - 1, s_pWarnGEN		},
 { sizeof ( s_pWarnFCEU	  ) - 1, s_pWarnFCEU	},
 { sizeof ( s_pWarnVBA	  ) - 1, s_pWarnVBA		},
 { sizeof ( s_pWarnFBA	  ) - 1, s_pWarnFBA		}, //9

 { sizeof ( s_pCopy0	  ) - 1, s_pCopy0		}, //10
 { sizeof ( s_pCopy1	  ) - 1, s_pCopy1		},
 { sizeof ( s_pCopy2	  ) - 1, s_pCopy2		},
 { sizeof ( s_pCopy3	  ) - 1, s_pCopy3		},
 { sizeof ( s_pCopy4	  ) - 1, s_pCopy4		},
 { sizeof ( s_pCopy5	  ) - 1, s_pCopy5		},
 { sizeof ( s_pCopy6	  ) - 1, s_pCopy6		}, 
 { sizeof ( s_pCopy7	  ) - 1, s_pCopy7		}, 
 { sizeof ( s_pCopy8	  ) - 1, s_pCopy8		}, 
 { sizeof ( s_pCopy9	  ) - 1, s_pCopy9		}, 
 { sizeof ( s_pCopy10	  ) - 1, s_pCopy10		},
 { sizeof ( s_pCopy11	  ) - 1, s_pCopy11		}, 
 { sizeof ( s_pCopy12	  ) - 1, s_pCopy12		}, //22 

 { sizeof ( s_pNetCopy0	  ) - 1, s_pNetCopy0	}, //23
 { sizeof ( s_pNetCopy1	  ) - 1, s_pNetCopy1	},
 { sizeof ( s_pNetCopy2	  ) - 1, s_pNetCopy2	},
 { sizeof ( s_pNetCopy3	  ) - 1, s_pNetCopy3	},
 { sizeof ( s_pNetCopy4	  ) - 1, s_pNetCopy4	}, //27

 { sizeof ( s_pMove0	  ) - 1, s_pMove0		}, //28 
 { sizeof ( s_pMove1	  ) - 1, s_pMove1		}, 
 { sizeof ( s_pMove2	  ) - 1, s_pMove2		}, 
 { sizeof ( s_pMove3	  ) - 1, s_pMove3		}, 
 { sizeof ( s_pMove4	  ) - 1, s_pMove4		}, //32

 { sizeof ( s_pWarnINET	  ) - 1, s_pWarnINET	}, //33
 { sizeof ( s_pErrSRV0	  ) - 1, s_pErrSRV0		},
 { sizeof ( s_pErrUPD0	  ) - 1, s_pErrUPD0		},
 { sizeof ( s_pErrUPD1	  ) - 1, s_pErrUPD1		}, //36

 { sizeof ( s_pErrMNT	  ) - 1, s_pErrMNT		}, //37
 { sizeof ( s_pErrMVGAME  ) - 1, s_pErrMVGAME	},
 { sizeof ( s_pErrMVAV	  ) - 1, s_pErrMVAV		},

 { sizeof ( s_pDownUpdate	  ) - 1, s_pDownUpdate		}, //40
 { sizeof ( s_pDownCover	  ) - 1, s_pDownCover		},
 { sizeof ( s_pDownFile		  ) - 1, s_pDownFile		},
 { sizeof ( s_pDownTheme	  ) - 1, s_pDownTheme		},

 { sizeof ( s_pDownMSG0	  ) - 1, s_pDownMSG0		}, //44
 { sizeof ( s_pDownMSG1	  ) - 1, s_pDownMSG1		},
 { sizeof ( s_pDownMSG2	  ) - 1, s_pDownMSG2		},

 { sizeof ( s_pParamVer	  ) - 1, s_pParamVer		}, //47
 { sizeof ( s_pLastPlay	  ) - 1, s_pLastPlay		},
 { sizeof ( s_pSetAccess  ) - 1, s_pSetAccess		},
 { sizeof ( s_pSetAccess1 ) - 1, s_pSetAccess1		},

 { sizeof ( s_pPreProcess ) - 1, s_pPreProcess		}, //51

 { sizeof ( s_pNoSpace0	  ) - 1, s_pNoSpace0		}, //52
 { sizeof ( s_pNoSpace0	  ) - 1, s_pNoSpace1		},

 { sizeof ( s_pErrNoMemWeb	) - 1, s_pErrNoMemWeb	}, //54
 { sizeof ( s_pErrNoMem		) - 1, s_pErrNoMem		},


 /*,
 { sizeof ( s_p	  ) - 1, s_p	},
 { sizeof ( s_p	  ) - 1, s_p	},
 { sizeof ( s_p	  ) - 1, s_p	},
 { sizeof ( s_p	  ) - 1, s_p	},
 { sizeof ( s_p	  ) - 1, s_p	},
 { sizeof ( s_p	  ) - 1, s_p	},
 { sizeof ( s_p	  ) - 1, s_p	},
 { sizeof ( s_p	  ) - 1, s_p	},
 { sizeof ( s_p	  ) - 1, s_p	},*/

};


static MMString s_MMStringUDF[ sizeof ( s_MMStringDef ) / sizeof ( s_MMStringDef[ 0 ] ) ]; //UDF - user defined file with texts
       MMString g_MMString   [ sizeof ( s_MMStringDef ) / sizeof ( s_MMStringDef[ 0 ] ) ]; //holds default GUI texts


long MM_LocaleInit ( char *lang_file ) 
{
	long lSize=0;
	FILE *lFD = fopen(lang_file, "rb");

	if ( lFD != NULL ) 
	{
		fseek ( lFD, 0, SEEK_END );
		lSize = ftell(lFD);

		if ( lSize > 0 ) 
		{
			unsigned int   lIdx;
			unsigned char* lpEnd;
			unsigned char* lpPtr;
			unsigned char* lpBuff = lpPtr = ( unsigned char* )malloc ( lSize + 1 );

			lpEnd = lpBuff + lSize;
			lIdx  = 0;

			fseek( lFD, 3, SEEK_SET);
			fread( (unsigned char*) lpBuff, lSize, 1, lFD);

			while ( 1 ) 
			{

				while ( lpPtr != lpEnd && *lpPtr != '\r' && *lpPtr != '\n' ) ++lpPtr;

				*lpPtr = '\x00';

				s_MMStringUDF[ lIdx ].m_pStr = lpBuff;
				s_MMStringUDF[ lIdx ].m_Len  = lpPtr - lpBuff;

				if (  !s_MMStringUDF[ lIdx++ ].m_Len ||
				lpPtr++ == lpEnd               ||
				lIdx    == sizeof ( s_MMStringUDF ) / sizeof ( s_MMStringUDF[ 0 ] )
				) break;

				if ( *lpPtr  == '\n'  ) ++lpPtr;

				lpBuff = lpPtr;
			}
		}

		fclose ( lFD );
	} 
	return lSize;
}


void MM_LocaleSet ( bool mm_language ) {

 MMString* lpStr;

	if (  mm_language )
		lpStr = s_MMStringUDF;	//user defined translation file
	else
		lpStr = s_MMStringDef;	//default English

	memcpy (  g_MMString, lpStr, sizeof ( g_MMString )  );
}
