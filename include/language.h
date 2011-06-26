// ***********************************************************************************

#define STR_DEBUG_MODE	MM_STRING(   0 )
#define STR_QUIT		MM_STRING(   1 )
#define STR_QUIT1		MM_STRING(   2 )
#define STR_RESTART		MM_STRING(   3 )
#define STR_WARN_FTP	MM_STRING(   4 )
#define STR_WARN_SNES	MM_STRING(   5 )
#define STR_WARN_GEN	MM_STRING(   6 )
#define STR_WARN_FCEU	MM_STRING(   7 )
#define STR_WARN_VBA	MM_STRING(   8 )
#define STR_WARN_FBA	MM_STRING(   9 )

#define STR_COPY0		MM_STRING(  10 )
#define STR_COPY1		MM_STRING(  11 )
#define STR_COPY2		MM_STRING(  12 )
#define STR_COPY3		MM_STRING(  13 )
#define STR_COPY4		MM_STRING(  14 )
#define STR_COPY5		MM_STRING(  15 )
#define STR_COPY6		MM_STRING(  16 )
#define STR_COPY7		MM_STRING(  17 )
#define STR_COPY8		MM_STRING(  18 )
#define STR_COPY9		MM_STRING(  19 )
#define STR_COPY10		MM_STRING(  20 )
#define STR_COPY11		MM_STRING(  21 )
#define STR_COPY12		MM_STRING(  22 )

#define STR_NETCOPY0	MM_STRING(  23 )
#define STR_NETCOPY1	MM_STRING(  24 )
#define STR_NETCOPY2	MM_STRING(  25 )
#define STR_NETCOPY3	MM_STRING(  26 )
#define STR_NETCOPY4	MM_STRING(  27 )

#define STR_MOVE0		MM_STRING(  28 )
#define STR_MOVE1		MM_STRING(  29 )
#define STR_MOVE2		MM_STRING(  30 )
#define STR_MOVE3		MM_STRING(  31 )
#define STR_MOVE4		MM_STRING(  32 )

#define STR_WARN_INET	MM_STRING(  33 )
#define STR_ERR_SRV0	MM_STRING(  34 )
#define STR_ERR_UPD0	MM_STRING(  35 )
#define STR_ERR_UPD1	MM_STRING(  36 )

#define STR_ERR_MNT		MM_STRING(  37 )
#define STR_ERR_MVGAME	MM_STRING(  38 )
#define STR_ERR_MVAV	MM_STRING(  39 )

#define STR_DOWN_UPD	MM_STRING(  40 )
#define STR_DOWN_COVER	MM_STRING(  41 )
#define STR_DOWN_FILE	MM_STRING(  42 )
#define STR_DOWN_THM	MM_STRING(  43 )

#define STR_DOWN_MSG0	MM_STRING(  44 )
#define STR_DOWN_MSG1	MM_STRING(  45 )
#define STR_DOWN_MSG2	MM_STRING(  46 )

#define STR_PARAM_VER	MM_STRING(  47 )
#define STR_LP_DATA		MM_STRING(  48 )

#define STR_SET_ACCESS	MM_STRING(  49 )
#define STR_SET_ACCESS1	MM_STRING(  50 )

#define STR_PREPROCESS	MM_STRING(  51 )

#define STR_ERR_NOSPACE0	MM_STRING(  52 )
#define STR_ERR_NOSPACE1	MM_STRING(  53 )

#define STR_ERR_NOMEM_WEB	MM_STRING(  54 )
#define STR_ERR_NOMEM	MM_STRING(  55 )




#define	STR_LAST_ID 56

// ***********************************************************************************

typedef struct MMString {
	unsigned int   m_Len;
	unsigned char* m_pStr;
} MMString __attribute__(   (  aligned( 4 )  )   );

#define MM_STRING( ID ) g_MMString[ ID ].m_pStr

extern MMString g_MMString [];
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */
long MM_LocaleInit ( char *lang_file );
void MM_LocaleSet  ( bool mm_language );
#ifdef __cplusplus
}
#endif  /* __cplusplus */
