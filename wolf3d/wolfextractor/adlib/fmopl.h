/* tab setting : 4
 *
 * FM OPL2 synth
 *
 * Copyright (C) 1999,2000 Tatsuyuki Satoh , MultiArcadeMachineEmulator development
 * Modified for Wolfenstein 3D by Steven Fuller
 * Future Modifications by DarkOne for WolfGL! (wolfgl.narod.ru)
 */
#ifndef __FMOPL_H_
#define __FMOPL_H_

//	Register addresses
// Operator stuff
#define	alChar		0x20
#define	alScale		0x40
#define	alAttack	0x60
#define	alSus			0x80
#define	alWave		0xe0
// Channel stuff
#define	alFreqL		0xa0
#define	alFreqH		0xb0
#define	alFeedCon	0xc0
// Global stuff
#define	alEffects	0xbd

#define OPL_OUTPUT_BIT 16

typedef unsigned char	UINT8;	/* unsigned  8bit */
typedef unsigned short	UINT16;	/* unsigned 16bit */
typedef unsigned long	UINT32;	/* unsigned 32bit */
typedef signed char		INT8;	/* signed  8bit   */
typedef signed short	INT16;	/* signed 16bit   */
typedef signed long		INT32;	/* signed 32bit   */

#if (OPL_OUTPUT_BIT==16)
typedef INT16 OPLSAMPLE;
#endif
#if (OPL_OUTPUT_BIT==8)
typedef unsigned char  OPLSAMPLE;
#endif

/* ---------- OPL one of slot  ---------- */
typedef struct fm_opl_slot
{
	INT32 TL;			/* total level     :TL << 8            */
	INT32 TLL;		/* adjusted now TL                     */
	UINT8  KSR;		/* key scale rate  :(shift down bit)   */
	INT32 *AR;		/* attack rate     :&AR_TABLE[AR<<2]   */
	INT32 *DR;		/* decay rate      :&DR_TALBE[DR<<2]   */
	INT32 SL;			/* sustin level    :SL_TALBE[SL]       */
	INT32 *RR;		/* release rate    :&DR_TABLE[RR<<2]   */
	UINT8 ksl;		/* keyscale level  :(shift down bits)  */
	UINT8 ksr;		/* key scale rate  :kcode>>KSR         */
	UINT32 mul;		/* multiple        :ML_TABLE[ML]       */
	UINT32 Cnt;		/* frequency count :                   */
	UINT32 Incr;	/* frequency step  :                   */
	/* envelope generator state */
	UINT8 eg_typ;	/* envelope type flag                  */
	UINT8 evm;		/* envelope phase                      */
	INT32 evc;		/* envelope counter                    */
	INT32 eve;		/* envelope counter end point          */
	INT32 evs;		/* envelope counter step               */
	INT32 evsa;		/* envelope step for AR :AR[ksr]       */
	INT32 evsd;		/* envelope step for DR :DR[ksr]       */
	INT32 evsr;		/* envelope step for RR :RR[ksr]       */
	/* LFO */
	UINT8 ams;		/* ams flag                            */
	UINT8 vib;		/* vibrate flag                        */
	/* wave selector */
	INT32 **wavetable;
} OPL_SLOT;

/* ---------- OPL one of channel  ---------- */
typedef struct fm_opl_channel
{
	OPL_SLOT SLOT[2];
	UINT8 CON;					/* connection type                     */
	UINT8 FB;						/* feed back       :(shift down bit)   */
	INT32 *connect1;		/* slot1 output pointer                */
	INT32 *connect2;		/* slot2 output pointer                */
	INT32 op1_out[2];		/* slot1 output for selfeedback        */
	/* phase generator state */
	UINT32  block_fnum;	/* block+fnum      :                   */
	UINT8 kcode;				/* key code        : KeyScaleCode      */
	UINT32  fc;					/* Freq. Increment base                */
	UINT32  ksl_base;		/* KeyScaleLevel Base step             */
	UINT8 keyon;				/* key on/off flag                     */
} OPL_CH;

/* OPL state */
typedef struct fm_opl_f {
	UINT8 type;					/* chip type                        */
	int clock;					/* master clock  (Hz)                */
	int rate;						/* sampling rate (Hz)                */
	double freqbase;		/* frequency base                    */
	UINT8 address;			/* address register                  */
	UINT32 mode;				/* Reg.08 : CSM , notesel,etc.       */
	/* FM channel slots */
	OPL_CH *P_CH;				/* pointer of CH       */
	int	max_ch;					/* maximum channel     */
	/* Rythm sention */
	UINT8 rythm;				/* Rythm mode , key flag */

	INT32 AR_TABLE[75];			/* attack rate tables */
	INT32 DR_TABLE[75];			/* decay rate tables   */
	UINT32 FN_TABLE[1024];  /* fnumber -> increment counter */
	/* LFO */
	INT32 *ams_table;
	INT32 *vib_table;
	INT32 amsCnt;
	INT32 amsIncr;
	INT32 vibCnt;
	INT32 vibIncr;
	/* wave selector enable flag */
	UINT8 wavesel;
} FM_OPL;

/* ---------- Generic interface section ---------- */
#define OPL_TYPE_YM3812 0

FM_OPL *OPLCreate(int type, int clock, int rate);
void OPLDestroy(FM_OPL *OPL);

void OPLResetChip(FM_OPL *OPL);
void OPLWrite(FM_OPL *OPL,int a,int v);
unsigned char OPLRead(FM_OPL *OPL,int a);

void YM3812UpdateOne(FM_OPL *OPL, void *buffer, int length);

#endif /* __FMPOL_H */
/* end of file */



