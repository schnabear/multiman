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

//multiMAN Update
static unsigned char s_pPleaseWait	[]	= "Please wait...";
static unsigned char s_pWhatsNew	[]	= "What's new in multiMAN %s:\n\n%s";
static unsigned char s_pNewVer		[]	= "New version found: %s\n\nYour current version: %s\n\nDo you want to download the update?";
static unsigned char s_pNewVerDL	[]	= "Download completed successfully!\n\nInstall the update from [* Install Package Files] XMB\xE2\x84\xA2 tab.\n\nUpdate file saved as: %s\n\n%s?";
static unsigned char s_pNewVerNN	[]	= "You already have the latest version: %s\n\n            There is no need to update.";
static unsigned char s_pNewVerUSB	[]	= "Please attach USB storage device to save update data and try again!";

//Game Update
static unsigned char s_pGameUpdate1	[]	= "%s\n\nVersion: %s (%i update)\nUpdate size: %.2fMB\n\nDownload update now?";
static unsigned char s_pGameUpdate2	[]	= "%s\n\nVersions: %s - %s (%i updates)\nTotal update size: %.2fMB\n\nDownload updates now?";
static unsigned char s_pGameUpdate3	[]	= "%s\n\nYou already have version %.2f installed.\n\nDo you want to download newer updates only?";
static unsigned char s_pGameUpdate5	[]	= "Download completed successfully!\n\nUpdate files saved in: %s\n\n%s?";
static unsigned char s_pGameUpdate6	[]	= "You already have the latest version of the game.";
static unsigned char s_pGameUpdate7	[]	= "Cannot find update information for this title!";

//Selection menus captions
static unsigned char s_pSelTheme	[]	= "Select Theme";
static unsigned char s_pSelLang		[]	= "Select Language";
static unsigned char s_pDelGameC	[]	= "Delete Game Cache";

//File Manager top
static unsigned char s_pFMGames		[]	= "Games";
static unsigned char s_pFMUpdate	[]	= "Update";
static unsigned char s_pFMAbout		[]	= "About";
static unsigned char s_pFMHelp		[]	= "Help";
static unsigned char s_pFMThemes	[]	= "Themes";

//File Manager Command menu
static unsigned char s_pCMMulDir	[]	= "Multiple folders";
static unsigned char s_pCMMulFile	[]	= "Multiple files";

static unsigned char s_pCMCopy		[]	= "Copy";
static unsigned char s_pCMMove		[]	= "Move";
static unsigned char s_pCMRename	[]	= "Rename";
static unsigned char s_pCMDelete	[]	= "Delete";
static unsigned char s_pCMShortcut	[]	= "Create Shortcut";
static unsigned char s_pCMShadow	[]	= "Shadow for PKG game";
static unsigned char s_pCMBDMirror	[]	= "Activate BD-Mirror";
static unsigned char s_pCMNetHost	[]	= "Refresh Net Host";
static unsigned char s_pCMHexView	[]	= "Open in HEX Viewer";
static unsigned char s_pCMProps		[]	= "Properties";
static unsigned char s_pCMNewDir	[]	= "Create New Folder";

static unsigned char s_pApplyTheme	[]	= "Applying \x22%s\x22 theme, please wait...";

// system menu
static unsigned char s_pMMUpdate	[]	= "Update";
static unsigned char s_pMMUpdateL1	[]	= "Check for available program updates.";
static unsigned char s_pMMUpdateL2	[]	= "multiMAN is being updated constantly, so make sure";
static unsigned char s_pMMUpdateL3	[]	= "to check for new versions regularly.";
static unsigned char s_pMMUpdateL4	[]	= "Internet connection required.";

static unsigned char s_pMMRefresh	[]	= "Refresh List";
static unsigned char s_pMMRefreshL1	[]	= "Re-scan internal and external hard disk drives.";
static unsigned char s_pMMRefreshL2	[]	= "Force multiMAN to refresh and re-detect content";
static unsigned char s_pMMRefreshL3	[]	= "(games, video, other). Use this option if you";
static unsigned char s_pMMRefreshL4	[]	= "transfer content via FTP. Shortcut is SELECT+L3.";

static unsigned char s_pMMFileMan	[]	= "File Manager";
static unsigned char s_pMMFileManL1	[]	= "Switch to File Manager mode. Shortcut is SELECT+START.";
static unsigned char s_pMMFileManL2	[]	= "Use file manager to manage your files and folders, to copy,";
static unsigned char s_pMMFileManL3	[]	= "move or rename them, to view images and play music and video.";
static unsigned char s_pMMFileManL4	[]	= "SHOWTIME may be required to play some content.";

static unsigned char s_pMMShowtimeST	[]	= "Launch Showtime";
static unsigned char s_pMMShowtimeSTL1	[]	= "Quit multiMAN and launch Showtime Player.";
static unsigned char s_pMMShowtimeSTL2	[]	= "Showtime Media Player is a feature rich";
static unsigned char s_pMMShowtimeSTL3	[]	= "application, which allows you to play";
static unsigned char s_pMMShowtimeSTL4	[]	= "various video, audio and graphic formats.";

static unsigned char s_pMMNTFS		[]	= "PFS / NTFS driver";
static unsigned char s_pMMNTFSL1	[]	= "Switch between FAT32 and NTFS drivers.";
static unsigned char s_pMMNTFSL2	[]	= "To use this option you must connect external USB HDD,";
static unsigned char s_pMMNTFSL3	[]	= "formatted with FAT32 or NTFS file system.";
static unsigned char s_pMMNTFSL4	[]	= "Proper USB.CFG required. Shortcut is START+TRIANGLE.";

static unsigned char s_pMMShowtimeLK	[]	= "Link VIDEO to Showtime";
static unsigned char s_pMMShowtimeLKL1	[]	= "Create links of your XMB\xE2\x84\xA2 video files to";
static unsigned char s_pMMShowtimeLKL2	[]	= "a special folder for Showtime Media Player";
static unsigned char s_pMMShowtimeLKL3	[]	= "to find them. When linking completes, multiMAN";
static unsigned char s_pMMShowtimeLKL4	[]	= "will launch Showtime.";

static unsigned char s_pMMScrShot	[]	= "Screenshot";
static unsigned char s_pMMScrShotL1	[]	= "Take a screenshot of your game list.";
static unsigned char s_pMMScrShotL2	[]	= "Current screen will be saved as RGB raw file to /dev_hdd0";
static unsigned char s_pMMScrShotL3	[]	= "or in the root folder of connected USB device.";
static unsigned char s_pMMScrShotL4	[]	= "Shortcut is START+R2.";

static unsigned char s_pMMScrSave	[]	= "Screensaver";
static unsigned char s_pMMScrSaveL1	[]	= "Turn on multiMAN's screensaver.";
static unsigned char s_pMMScrSaveL2	[]	= "A screen with falling 'stars' will appear on";
static unsigned char s_pMMScrSaveL3	[]	= "your display. Press a button to quit";
static unsigned char s_pMMScrSaveL4	[]	= "the screensaver mode.";

static unsigned char s_pMMRestart	[]	= "Restart";
static unsigned char s_pMMRestartL1	[]	= "Restart multiMAN. Shortcut is START+SELECT.";
static unsigned char s_pMMRestartL2	[]	= "You can remotely restart multiMAN by connecting to your";
static unsigned char s_pMMRestartL3	[]	= "Playstation\xC2\xAE\x33 system via telnet to port 8080";
static unsigned char s_pMMRestartL4	[]	= "and type 'restart'.";

static unsigned char s_pMMSetup		[]	= "multiMAN Setup";
static unsigned char s_pMMSetupL1	[]	= "Switch to XMMB Settings column.";
static unsigned char s_pMMSetupL2	[]	= "You can edit all multiMAN options in XMMB mode.";
static unsigned char s_pMMSetupL3	[]	= "If you select this option, the current display";
static unsigned char s_pMMSetupL4	[]	= "mode will be changed.";

static unsigned char s_pMMQuit		[]	= "Quit";
static unsigned char s_pMMQuitL1	[]	= "Quit multiMAN and return to XMB\xE2\x84\xA2 home screen.";
static unsigned char s_pMMQuitL2	[]	= "You can remotely quit multiMAN by connecting to your";
static unsigned char s_pMMQuitL3	[]	= "Playstation\xC2\xAE\x33 system via telnet to port 8080";
static unsigned char s_pMMQuitL4	[]	= "and type 'quit'.";

static unsigned char s_pMMHelp		[]	= "Help";
static unsigned char s_pMMHelpL1	[]	= "Start the HELP application.";
static unsigned char s_pMMHelpL2	[]	= "This option will execute external";
static unsigned char s_pMMHelpL3	[]	= "help.MME application and quit multiMAN.";
static unsigned char s_pMMHelpL4	[]	= "Avoid using it during FTP transfers.";

static unsigned char s_pButNavigate	[]	= "Navigate";
static unsigned char s_pButSelect	[]	= "Select";
static unsigned char s_pButBack		[]	= "Back";
static unsigned char s_pButCancel	[]	= "Cancel";
static unsigned char s_pButApply	[]	= "Apply";
static unsigned char s_pButConfirm	[]	= "Confirm";
static unsigned char s_pButGenre	[]	= "Genre";
static unsigned char s_pButDownload	[]	= "Download";
static unsigned char s_pButLoad		[]	= "Load";
static unsigned char s_pButPrev		[]	= "Prev Title";
static unsigned char s_pButNext		[]	= "Next Title";
static unsigned char s_pButLast		[]	= "Last Title";
static unsigned char s_pButFirst	[]	= "First Title";

static unsigned char s_pSelGenre	[]	= "Select Genre";
static unsigned char s_pButDownTheme[]	= "Download Theme";

// Game settings menu
static unsigned char s_pGMCopy		[]	= "Copy";
static unsigned char s_pGMCopyL1	[]	= "Create a backup copy of \x22%s\x22.";
static unsigned char s_pGMCopyL2	[]	= "To protect your Playstation\xC2\xAE\x33 Blu-ray\xE2\x84\xA2 game disc,";
static unsigned char s_pGMCopyL3	[]	= "transfer its contents to internal or external hard disk drive.";

static unsigned char s_pGMDelete	[]	= "Delete";
static unsigned char s_pGMDeleteL1	[]	= "Permanently delete game files.";
static unsigned char s_pGMDeleteL2	[]	= "If you are running low on disk space, delete the game from your";
static unsigned char s_pGMDeleteL3	[]	= "hard disk drive. Use this option with caution!";

static unsigned char s_pGMRename	[]	= "Rename";
static unsigned char s_pGMRenameL1	[]	= "Pick a name of your choice for the game.";
static unsigned char s_pGMRenameL2	[]	= "You can use the on-screen keyboard or external USB keyboard";
static unsigned char s_pGMRenameL3	[]	= "for input. Use (TM), (R) and (C) to enter \xE2\x84\xA2, \xC2\xAE and \xC2\xA9 symbols.";

static unsigned char s_pGMUpdate	[]	= "Update";
static unsigned char s_pGMUpdateL1	[]	= "Check for game updates.";
static unsigned char s_pGMUpdateL2	[]	= "This feature allows you to download all available updates";
static unsigned char s_pGMUpdateL3	[]	= "or only the latest. Internet connection required.";

static unsigned char s_pGMTest		[]	= "Test";
static unsigned char s_pGMTestL1	[]	= "Verify all files and folders of the game.";
static unsigned char s_pGMTestL2	[]	= "Testing will report various data as Total Size, Number of files,";
static unsigned char s_pGMTestL3	[]	= "Big files (over 4GB) and if game contains split (.666##) files.";

static unsigned char s_pGMPerm		[]	= "Permissions";
static unsigned char s_pGMPermL1	[]	= "Re-apply file and folder access permissions.";
static unsigned char s_pGMPermL2	[]	= "On rare occasions it may be required to perform resetting of";
static unsigned char s_pGMPermL3	[]	= "ownership and execution flags of game contents.";

static unsigned char s_pPOPGS		[]	= ": Game Settings";
static unsigned char s_pPOPChangeS	[]	= ": Change Setting";
static unsigned char s_pPOPSysInfo	[]	= ": View System Information";
static unsigned char s_pPOPLang		[]	= ": Change Display Language";
static unsigned char s_pPOPCache	[]	= ": Clear Cached Files";
static unsigned char s_pPOPPhoto	[]	= ": View Photo";
static unsigned char s_pPOPMusic	[]	= ": Play Music";
static unsigned char s_pPOPST		[]	= ": Launch Showtime";
static unsigned char s_pPOPVideo	[]	= ": Play Video";
static unsigned char s_pPOPRefGames	[]	= ": Refresh List";
static unsigned char s_pPOPRefRoms	[]	= ": Refresh ROMs";
static unsigned char s_pPOPRom		[]	= ": Load Game ROM";

static unsigned char s_pPOPGrpGenre	[]	= ": Group Titles by Genre";
static unsigned char s_pPOPGrpEmu	[]	= ": Group ROMs by Emulator";
static unsigned char s_pPOPGrpName	[]	= ": Group Titles by Name";
static unsigned char s_pPOPSwitch	[]	= ": Switch Display";

static unsigned char s_pPOP1of1		[]	= "%s: %i of %i";
static unsigned char s_pPOPPlaying	[]	= "Playing";
static unsigned char s_pPOPPaused	[]	= "Paused";
static unsigned char s_pPOPVol		[]	= "[Volume: %i]";

/*
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

 { sizeof ( s_pDownUpdate ) - 1, s_pDownUpdate		}, //40
 { sizeof ( s_pDownCover  ) - 1, s_pDownCover		},
 { sizeof ( s_pDownFile	  ) - 1, s_pDownFile		},
 { sizeof ( s_pDownTheme  ) - 1, s_pDownTheme		},

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

 { sizeof ( s_pPleaseWait	) - 1, s_pPleaseWait	},
 { sizeof ( s_pWhatsNew		) - 1, s_pWhatsNew		},
 { sizeof ( s_pNewVer		) - 1, s_pNewVer		},
 { sizeof ( s_pNewVerDL		) - 1, s_pNewVerDL		},
 { sizeof ( s_pNewVerNN		) - 1, s_pNewVerNN		}, //60
 { sizeof ( s_pNewVerUSB	) - 1, s_pNewVerUSB		}, 

 { sizeof ( s_pGameUpdate1	) - 1, s_pGameUpdate1	}, //62
 { sizeof ( s_pGameUpdate2	) - 1, s_pGameUpdate2	},
 { sizeof ( s_pGameUpdate3	) - 1, s_pGameUpdate3	},
 { sizeof ( s_pGameUpdate5	) - 1, s_pGameUpdate5	},
 { sizeof ( s_pGameUpdate6	) - 1, s_pGameUpdate6	},
 { sizeof ( s_pGameUpdate7	) - 1, s_pGameUpdate7	}, //67

 { sizeof ( s_pSelTheme	    ) - 1, s_pSelTheme		},
 { sizeof ( s_pSelLang  	) - 1, s_pSelLang		},
 { sizeof ( s_pDelGameC		) - 1, s_pDelGameC		}, //70

 { sizeof ( s_pFMGames		) - 1, s_pFMGames		},
 { sizeof ( s_pFMUpdate		) - 1, s_pFMUpdate		},
 { sizeof ( s_pFMAbout		) - 1, s_pFMAbout		},
 { sizeof ( s_pFMHelp		) - 1, s_pFMHelp		},
 { sizeof ( s_pFMThemes		) - 1, s_pFMThemes		}, //75

 { sizeof ( s_pCMMulDir		) - 1, s_pCMMulDir		}, //76
 { sizeof ( s_pCMMulFile	) - 1, s_pCMMulFile		},

 { sizeof ( s_pCMCopy		) - 1, s_pCMCopy		}, //78
 { sizeof ( s_pCMMove		) - 1, s_pCMMove		},
 { sizeof ( s_pCMRename		) - 1, s_pCMRename		},
 { sizeof ( s_pCMDelete		) - 1, s_pCMDelete		},
 { sizeof ( s_pCMShortcut	) - 1, s_pCMShortcut	},
 { sizeof ( s_pCMShadow		) - 1, s_pCMShadow		},
 { sizeof ( s_pCMBDMirror	) - 1, s_pCMBDMirror	},
 { sizeof ( s_pCMNetHost	) - 1, s_pCMNetHost		},
 { sizeof ( s_pCMHexView	) - 1, s_pCMHexView		},
 { sizeof ( s_pCMProps		) - 1, s_pCMProps		},
 { sizeof ( s_pCMNewDir		) - 1, s_pCMNewDir		}, //88

 { sizeof ( s_pApplyTheme	) - 1, s_pApplyTheme	}, //89

 { sizeof ( s_pMMUpdate		) - 1, s_pMMUpdate		}, //90
 { sizeof ( s_pMMUpdateL1	) - 1, s_pMMUpdateL1	},
 { sizeof ( s_pMMUpdateL2	) - 1, s_pMMUpdateL2	},
 { sizeof ( s_pMMUpdateL3	) - 1, s_pMMUpdateL3	},
 { sizeof ( s_pMMUpdateL4	) - 1, s_pMMUpdateL4	},

 { sizeof ( s_pMMRefresh	) - 1, s_pMMRefresh		}, //95
 { sizeof ( s_pMMRefreshL1	) - 1, s_pMMRefreshL1	},
 { sizeof ( s_pMMRefreshL2	) - 1, s_pMMRefreshL2	},
 { sizeof ( s_pMMRefreshL3	) - 1, s_pMMRefreshL3	},
 { sizeof ( s_pMMRefreshL4	) - 1, s_pMMRefreshL4	},

 { sizeof ( s_pMMFileMan	) - 1, s_pMMFileMan		}, //100
 { sizeof ( s_pMMFileManL1	) - 1, s_pMMFileManL1	},
 { sizeof ( s_pMMFileManL2	) - 1, s_pMMFileManL2	},
 { sizeof ( s_pMMFileManL3	) - 1, s_pMMFileManL3	},
 { sizeof ( s_pMMFileManL4	) - 1, s_pMMFileManL4	},

 { sizeof ( s_pMMShowtimeST		) - 1, s_pMMShowtimeST		}, //105
 { sizeof ( s_pMMShowtimeSTL1	) - 1, s_pMMShowtimeSTL1	},
 { sizeof ( s_pMMShowtimeSTL2	) - 1, s_pMMShowtimeSTL2	},
 { sizeof ( s_pMMShowtimeSTL3	) - 1, s_pMMShowtimeSTL3	},
 { sizeof ( s_pMMShowtimeSTL4	) - 1, s_pMMShowtimeSTL4	}, 

 { sizeof ( s_pMMNTFS			) - 1, s_pMMNTFS			}, //110
 { sizeof ( s_pMMNTFSL1			) - 1, s_pMMNTFSL1			},
 { sizeof ( s_pMMNTFSL2			) - 1, s_pMMNTFSL2			},
 { sizeof ( s_pMMNTFSL3			) - 1, s_pMMNTFSL3			},
 { sizeof ( s_pMMNTFSL4			) - 1, s_pMMNTFSL4			},

 { sizeof ( s_pMMShowtimeLK		) - 1, s_pMMShowtimeLK		}, //115
 { sizeof ( s_pMMShowtimeLKL1	) - 1, s_pMMShowtimeLKL1	},
 { sizeof ( s_pMMShowtimeLKL2	) - 1, s_pMMShowtimeLKL2	},
 { sizeof ( s_pMMShowtimeLKL3	) - 1, s_pMMShowtimeLKL3	},
 { sizeof ( s_pMMShowtimeLKL4	) - 1, s_pMMShowtimeLKL4	}, 

 { sizeof ( s_pMMScrShot	) - 1, s_pMMScrShot		}, //120
 { sizeof ( s_pMMScrShotL1	) - 1, s_pMMScrShotL1	},
 { sizeof ( s_pMMScrShotL2	) - 1, s_pMMScrShotL2	},
 { sizeof ( s_pMMScrShotL3	) - 1, s_pMMScrShotL3	},
 { sizeof ( s_pMMScrShotL4	) - 1, s_pMMScrShotL4	}, 

 { sizeof ( s_pMMScrSave	) - 1, s_pMMScrSave		}, //125
 { sizeof ( s_pMMScrSaveL1	) - 1, s_pMMScrSaveL1	},
 { sizeof ( s_pMMScrSaveL2	) - 1, s_pMMScrSaveL2	},
 { sizeof ( s_pMMScrSaveL3	) - 1, s_pMMScrSaveL3	},
 { sizeof ( s_pMMScrSaveL4	) - 1, s_pMMScrSaveL4	}, 

 { sizeof ( s_pMMRestart	) - 1, s_pMMRestart		}, //130
 { sizeof ( s_pMMRestartL1	) - 1, s_pMMRestartL1	},
 { sizeof ( s_pMMRestartL2	) - 1, s_pMMRestartL2	},
 { sizeof ( s_pMMRestartL3	) - 1, s_pMMRestartL3	},
 { sizeof ( s_pMMRestartL4	) - 1, s_pMMRestartL4	}, 

 { sizeof ( s_pMMSetup		) - 1, s_pMMSetup		}, //135
 { sizeof ( s_pMMSetupL1	) - 1, s_pMMSetupL1		},
 { sizeof ( s_pMMSetupL2	) - 1, s_pMMSetupL2		},
 { sizeof ( s_pMMSetupL3	) - 1, s_pMMSetupL3		},
 { sizeof ( s_pMMSetupL4	) - 1, s_pMMSetupL4		}, 

 { sizeof ( s_pMMQuit		) - 1, s_pMMQuit		}, //140
 { sizeof ( s_pMMQuitL1		) - 1, s_pMMQuitL1		},
 { sizeof ( s_pMMQuitL2		) - 1, s_pMMQuitL2		},
 { sizeof ( s_pMMQuitL3		) - 1, s_pMMQuitL3		},
 { sizeof ( s_pMMQuitL4		) - 1, s_pMMQuitL4		}, 

 { sizeof ( s_pMMHelp		) - 1, s_pMMHelp		}, //145
 { sizeof ( s_pMMHelpL1		) - 1, s_pMMHelpL1		},
 { sizeof ( s_pMMHelpL2		) - 1, s_pMMHelpL2		},
 { sizeof ( s_pMMHelpL3		) - 1, s_pMMHelpL3		},
 { sizeof ( s_pMMHelpL4		) - 1, s_pMMHelpL4		}, 

 { sizeof ( s_pButNavigate	  ) - 1, s_pButNavigate	}, //150
 { sizeof ( s_pButSelect	  ) - 1, s_pButSelect	},
 { sizeof ( s_pButBack		  ) - 1, s_pButBack		},
 { sizeof ( s_pButCancel	  ) - 1, s_pButCancel	},
 { sizeof ( s_pButApply		  ) - 1, s_pButApply	},
 { sizeof ( s_pButConfirm	  ) - 1, s_pButConfirm	},
 { sizeof ( s_pButGenre		  ) - 1, s_pButGenre	},
 { sizeof ( s_pButDownload	  ) - 1, s_pButDownload	},
 { sizeof ( s_pButLoad		  ) - 1, s_pButLoad		},
 { sizeof ( s_pButPrev		  ) - 1, s_pButPrev		},
 { sizeof ( s_pButNext		  ) - 1, s_pButNext		}, //160
 { sizeof ( s_pButLast		  ) - 1, s_pButLast		},
 { sizeof ( s_pButFirst		  ) - 1, s_pButFirst	},

 { sizeof ( s_pSelGenre		  ) - 1, s_pSelGenre	}, //163
 { sizeof ( s_pButDownTheme	  ) - 1, s_pButDownTheme},

 { sizeof ( s_pGMCopy		) - 1, s_pGMCopy		}, //165
 { sizeof ( s_pGMCopyL1		) - 1, s_pGMCopyL1		},
 { sizeof ( s_pGMCopyL2		) - 1, s_pGMCopyL2		},
 { sizeof ( s_pGMCopyL3		) - 1, s_pGMCopyL3		},

 { sizeof ( s_pGMDelete		) - 1, s_pGMDelete		}, //169
 { sizeof ( s_pGMDeleteL1	) - 1, s_pGMDeleteL1	},
 { sizeof ( s_pGMDeleteL2	) - 1, s_pGMDeleteL2	},
 { sizeof ( s_pGMDeleteL3	) - 1, s_pGMDeleteL3	},

 { sizeof ( s_pGMRename		) - 1, s_pGMRename		}, //173
 { sizeof ( s_pGMRenameL1	) - 1, s_pGMRenameL1	},
 { sizeof ( s_pGMRenameL2	) - 1, s_pGMRenameL2	},
 { sizeof ( s_pGMRenameL3	) - 1, s_pGMRenameL3	},

 { sizeof ( s_pGMUpdate		) - 1, s_pGMUpdate		}, //177
 { sizeof ( s_pGMUpdateL1	) - 1, s_pGMUpdateL1	},
 { sizeof ( s_pGMUpdateL2	) - 1, s_pGMUpdateL2	},
 { sizeof ( s_pGMUpdateL3	) - 1, s_pGMUpdateL3	},

 { sizeof ( s_pGMTest		) - 1, s_pGMTest		}, //181
 { sizeof ( s_pGMTestL1		) - 1, s_pGMTestL1		},
 { sizeof ( s_pGMTestL2		) - 1, s_pGMTestL2		},
 { sizeof ( s_pGMTestL3		) - 1, s_pGMTestL3		},

 { sizeof ( s_pGMPerm		) - 1, s_pGMPerm		}, //185
 { sizeof ( s_pGMPermL1		) - 1, s_pGMPermL1		},
 { sizeof ( s_pGMPermL2		) - 1, s_pGMPermL2		},
 { sizeof ( s_pGMPermL3		) - 1, s_pGMPermL3		}, //188

 { sizeof ( s_pPOPGS		) - 1, s_pPOPGS			}, //189
 { sizeof ( s_pPOPChangeS	) - 1, s_pPOPChangeS	},
 { sizeof ( s_pPOPSysInfo	) - 1, s_pPOPSysInfo	},
 { sizeof ( s_pPOPLang		) - 1, s_pPOPLang		},
 { sizeof ( s_pPOPCache		) - 1, s_pPOPCache		},
 { sizeof ( s_pPOPPhoto		) - 1, s_pPOPPhoto		},
 { sizeof ( s_pPOPMusic		) - 1, s_pPOPMusic		},
 { sizeof ( s_pPOPST		) - 1, s_pPOPST			},
 { sizeof ( s_pPOPVideo		) - 1, s_pPOPVideo		},
 { sizeof ( s_pPOPRefGames	) - 1, s_pPOPRefGames	},
 { sizeof ( s_pPOPRefRoms	) - 1, s_pPOPRefRoms	},
 { sizeof ( s_pPOPRom		) - 1, s_pPOPRom		}, //200

 { sizeof ( s_pPOPGrpGenre	) - 1, s_pPOPGrpGenre	}, //201
 { sizeof ( s_pPOPGrpEmu	) - 1, s_pPOPGrpEmu		},
 { sizeof ( s_pPOPGrpName	) - 1, s_pPOPGrpName	},
 { sizeof ( s_pPOPSwitch	) - 1, s_pPOPSwitch		},

 { sizeof ( s_pPOP1of1		) - 1, s_pPOP1of1		}, //205
 { sizeof ( s_pPOPPlaying	) - 1, s_pPOPPlaying	},
 { sizeof ( s_pPOPPaused	) - 1, s_pPOPPaused		},
 { sizeof ( s_pPOPVol		) - 1, s_pPOPVol		}, //208


 /*,
 { sizeof ( s_p	  ) - 1, s_p	},
 { sizeof ( s_p	  ) - 1, s_p	},
 { sizeof ( s_p	  ) - 1, s_p	},
 { sizeof ( s_p	  ) - 1, s_p	},
 { sizeof ( s_p	  ) - 1, s_p	},
 { sizeof ( s_p	  ) - 1, s_p	},
 { sizeof ( s_p	  ) - 1, s_p	},
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
			for(int m=0; m<lSize; m++) if(lpBuff[m]=='|') lpBuff[m]='\n';

			while ( 1 ) 
			{
				while ( lpPtr != lpEnd && *lpPtr != '\r' ) ++lpPtr;

				*lpPtr = '\x00';
				s_MMStringUDF[ lIdx ].m_pStr = lpBuff;
				s_MMStringUDF[ lIdx ].m_Len  = lpPtr - lpBuff;

				if (  !s_MMStringUDF[ lIdx++ ].m_Len ||
				lpPtr++ == lpEnd               ||
				lIdx    == sizeof ( s_MMStringUDF ) / sizeof ( s_MMStringUDF[ 0 ] )
				) break;

				if ( *lpPtr  == '\r'  ) ++lpPtr;

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
