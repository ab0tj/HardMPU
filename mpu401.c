/*
 *  Copyright (C) 2002-2012  The DOSBox Team
 *  Copyright (C) 2013-2014  bjt, elianda
 *  Copyright (C) 2015		 ab0tj
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
 * --------------------------------------------
 * HardMPU by ab0tj - Hardware MPU-401 Emulator
 * --------------------------------------------
 * 
 * Based on mpu401.c from SoftMPU by bjt which was
 * based on original mpu401.c from DOSBox
 *
 */

/* SOFTMPU: Moved exported functions & types to header */
#include "export.h"

/* HardMPU includes */
#include <avr/sfr_defs.h>
#include <avr/io.h>

/* SOFTMPU: Additional defines, typedefs etc. for C */
typedef unsigned short Bit16u;
typedef int Bits;
typedef signed char Bit8s;

/*typedef void (*PIC_EventHandler)(Bitu val);*/ /* SOFTMPU */

/* SOFTMPU: Stubbed functions */
void PIC_Init(void); /* SOFTMPU */
void PIC_AddEvent(EventID event,Bitu delay);
void PIC_RemoveEvents(EventID event);

void MIDI_Init(bool delaysysex,bool fakeallnotesoff);
void MIDI_RawOutByte(Bit8u data);
bool MIDI_Available(void);

void MPU401_Event(void);
void MPU401_ResetDone(void); /* SOFTMPU */
void MPU401_EOIHandler(void);
static void MPU401_Reset(void);
static void MPU401_EOIHandlerDispatch(void);

#define MPU401_VERSION  0x15
#define MPU401_REVISION 0x01
#define MPU401_QUEUE 32
#define MPU401_TIMECONSTANT 58000 /* SOFTMPU: Was 60000 */
#define MPU401_RESETBUSY 27*(RTCFREQ/1000) /* SOFTMPU */

enum MpuMode { M_UART,M_INTELLIGENT };
typedef enum MpuMode MpuMode; /* SOFTMPU */
enum MpuDataType {T_OVERFLOW,T_MARK,T_MIDI_SYS,T_MIDI_NORM,T_COMMAND};
typedef enum MpuDataType MpuDataType; /* SOFTMPU */

/* Messages sent to MPU-401 from host */
#define MSG_EOX					0xf7
#define MSG_OVERFLOW			0xf8
#define MSG_MARK				0xfc

/* Messages sent to host from MPU-401 */
#define MSG_MPU_OVERFLOW		0xf8
#define MSG_MPU_COMMAND_REQ		0xf9
#define MSG_MPU_END				0xfc
#define MSG_MPU_CLOCK			0xfd
#define MSG_MPU_ACK				0xfe

/* HardMPU: MPU config bits */
#define CONFIG_SYSEXDELAY		(mpu.config & 0x80)
#define CONFIG_FAKEALLNOTESOFF	(mpu.config & 0x40)
#define CONFIG_VERSIONFIX		(mpu.config & 0x20)
#define CONFIG_MIDIPORT			(mpu.config & 0x10)

static struct {
	bool intelligent;
        Bit8u config;
        MpuMode mode;
	Bit8u queue[MPU401_QUEUE];
	Bit8u queue_pos,queue_used;
	struct track {
		Bits counter;
		Bit8u value[8],sys_val;
		Bit8u vlength,length;
		MpuDataType type;
	} playbuf[8],condbuf;
	struct {
		bool conductor,cond_req,cond_set, block_ack;
		bool playing,reset;
		bool wsd,wsm,wsd_start;
		bool run_irq,irq_pending;
		bool send_now;
		bool eoi_scheduled;
		Bit8s data_onoff;
		Bitu command_byte,cmd_pending;
		Bit8u tmask,cmask,amask;
		Bit16u midi_mask;
		Bit16u req_mask;
		Bit8u channel,old_chan;
	} state;
	struct {
		Bit8u timebase,old_timebase;
		Bit8u tempo,old_tempo;
		Bit8u tempo_rel,old_tempo_rel;
		Bit8u tempo_grad;
		Bit8u cth_rate,cth_counter;
		bool clock_to_host,cth_active;
	} clock;
} mpu;

Bit8u QueueUsed() {
	return mpu.queue_used;
}

static void QueueByte(Bit8u data) {
	if (mpu.state.block_ack) {mpu.state.block_ack=false;return;}
	if (mpu.queue_used==0 && mpu.intelligent) mpu.state.irq_pending=true;
	if (mpu.queue_used<MPU401_QUEUE) {
		Bit8u pos=mpu.queue_used+mpu.queue_pos;
		if (mpu.queue_pos>=MPU401_QUEUE) mpu.queue_pos-=MPU401_QUEUE;
		if (pos>=MPU401_QUEUE) pos-=MPU401_QUEUE;
		mpu.queue_used++;
		mpu.queue[pos]=data;
	} /*else LOG(LOG_MISC,LOG_NORMAL)("MPU401:Data queue full");*/ /* SOFTMPU */
}

static void ClrQueue(void) {
	mpu.queue_used=0;
	mpu.queue_pos=0;
}

void MPU401_WriteCommand(Bit8u val) { /* SOFTMPU */
	Bit8u i; /* SOFTMPU */
	if (mpu.state.reset) {
		if (mpu.state.cmd_pending || (val!=0x3f && val!=0xff)) {
			mpu.state.cmd_pending=val+1;
			return;
		}
		PIC_RemoveEvents(RESET_DONE);
		mpu.state.reset=false;
	}
	if (val<=0x2f) {
		switch (val&3) { /* MIDI stop, start, continue */
			case 1: {MIDI_RawOutByte(0xfc);break;}
			case 2: {MIDI_RawOutByte(0xfa);break;}
			case 3: {MIDI_RawOutByte(0xfb);break;}
		}
		/*if (val&0x20) LOG(LOG_MISC,LOG_ERROR)("MPU-401:Unhandled Recording Command %x",val);*/ /* SOFTMPU */
		switch (val&0xc) {
			case  0x4:      /* Stop */
				PIC_RemoveEvents(MPU_EVENT);
				mpu.state.playing=false;
				for (i=0xb0;i<0xbf;i++) {  /* All notes off */
					MIDI_RawOutByte(i);
					MIDI_RawOutByte(0x7b);
					MIDI_RawOutByte(0);
				}
				break;
			case 0x8:       /* Play */
				/*LOG(LOG_MISC,LOG_NORMAL)("MPU-401:Intelligent mode playback started");*/ /* SOFTMPU */
				mpu.state.playing=true;
				PIC_RemoveEvents(MPU_EVENT);
                PIC_AddEvent(MPU_EVENT,(Bitu)MPU401_TIMECONSTANT/(mpu.clock.tempo*mpu.clock.timebase));
				ClrQueue();
				break;
		}
	}
	else if (val>=0xa0 && val<=0xa7) {      /* Request play counter */
		if (mpu.state.cmask&(1<<(val&7))) QueueByte((Bit8u)mpu.playbuf[val&7].counter);
	}
	else if (val>=0xd0 && val<=0xd7) {      /* Send data */
		mpu.state.old_chan=mpu.state.channel;
		mpu.state.channel=val&7;
		mpu.state.wsd=true;
		mpu.state.wsm=false;
		mpu.state.wsd_start=true;
	}
	else
	switch (val) {
		case 0xdf:      /* Send system message */
			mpu.state.wsd=false;
			mpu.state.wsm=true;
			mpu.state.wsd_start=true;
			break;
		case 0x8e:      /* Conductor */
			mpu.state.cond_set=false;
			break;
		case 0x8f:
			mpu.state.cond_set=true;
			break;
		case 0x94: /* Clock to host */
			mpu.clock.clock_to_host=false;
			break;
		case 0x95:
			mpu.clock.clock_to_host=true;
			break;
		case 0xc2: /* Internal timebase */
                        mpu.clock.timebase=48/(RTCFREQ/1000); /* SOFTMPU */
			break;
		case 0xc3:
                        mpu.clock.timebase=72/(RTCFREQ/1000); /* SOFTMPU */
			break;
		case 0xc4:
                        mpu.clock.timebase=96/(RTCFREQ/1000); /* SOFTMPU */
			break;
		case 0xc5:
                        mpu.clock.timebase=120/(RTCFREQ/1000); /* SOFTMPU */
			break;
		case 0xc6:
                        mpu.clock.timebase=144/(RTCFREQ/1000); /* SOFTMPU */
			break;
		case 0xc7:
                        mpu.clock.timebase=168/(RTCFREQ/1000); /* SOFTMPU */
			break;
		case 0xc8:
                        mpu.clock.timebase=192/(RTCFREQ/1000); /* SOFTMPU */
			break;
		/* Commands with data byte */
		case 0xe0: case 0xe1: case 0xe2: case 0xe4: case 0xe6: 
		case 0xe7: case 0xec: case 0xed: case 0xee: case 0xef:
			mpu.state.command_byte=val;
			break;
		/* Commands 0xa# returning data */
		case 0xab:      /* Request and clear recording counter */
			QueueByte(MSG_MPU_ACK);
			QueueByte(0);
			return;
		case 0xac:      /* Request version */
                        if (CONFIG_VERSIONFIX)
                        {
                                /* SOFTMPU: Fix missing music in Gateway by reversing version and ACK */
                                QueueByte(MPU401_VERSION);
                                QueueByte(MSG_MPU_ACK);
                        }
                        else
                        {
                                QueueByte(MSG_MPU_ACK);
                                QueueByte(MPU401_VERSION);
                        }
			return;
		case 0xad:      /* Request revision */
			QueueByte(MSG_MPU_ACK);
			QueueByte(MPU401_REVISION);
			return;
		case 0xae:		/* HardMPU: Request config */
			QueueByte(MSG_MPU_ACK);
			QueueByte(mpu.config);
			return;
		case 0xaf:      /* Request tempo */
			QueueByte(MSG_MPU_ACK);
			QueueByte(mpu.clock.tempo);
			return;
		case 0xb1:      /* Reset relative tempo */
			mpu.clock.tempo_rel=40;
			break;
		case 0xb9:      /* Clear play map */
		case 0xb8:      /* Clear play counters */
			for (i=0xb0;i<0xbf;i++) {  /* All notes off */
				MIDI_RawOutByte((Bit8u)i);
				MIDI_RawOutByte(0x7b);
				MIDI_RawOutByte(0);
			}
			for (i=0;i<8;i++) {
				mpu.playbuf[i].counter=0;
				mpu.playbuf[i].type=T_OVERFLOW;
			}
			mpu.condbuf.counter=0;
			mpu.condbuf.type=T_OVERFLOW;
			if (!(mpu.state.conductor=mpu.state.cond_set)) mpu.state.cond_req=0;
			mpu.state.amask=mpu.state.tmask;
			mpu.state.req_mask=0;
			mpu.state.irq_pending=true;
			break;
		case 0xfe:		/* HardMPU: Reset with config byte */
			mpu.state.command_byte=val;
			break;
		case 0xff:      /* Reset MPU-401 */
			/*LOG(LOG_MISC,LOG_NORMAL)("MPU-401:Reset %X",val);*/ /* SOFTMPU */
            PIC_AddEvent(RESET_DONE,MPU401_RESETBUSY);
			mpu.state.reset=true;
			MPU401_Reset();
			if (mpu.mode==M_UART) return;//do not send ack in UART mode
			break;
		case 0x3f:      /* UART mode */
			/*LOG(LOG_MISC,LOG_NORMAL)("MPU-401:Set UART mode %X",val);*/ /* SOFTMPU */
			mpu.mode=M_UART;
			break;
		default:
			/*LOG(LOG_MISC,LOG_NORMAL)("MPU-401:Unhandled command %X",val);*/
			break;
	}
	QueueByte(MSG_MPU_ACK);
}

Bit8u MPU401_ReadData(void) { /* SOFTMPU */
	Bit8u ret=MSG_MPU_ACK;	// HardMPU: we shouldn't be running this function if the queue is empty.
	//if (mpu.queue_used) {
		if (mpu.queue_pos>=MPU401_QUEUE) mpu.queue_pos-=MPU401_QUEUE;
		ret=mpu.queue[mpu.queue_pos];
		mpu.queue_pos++;mpu.queue_used--;
	//}
	if (!mpu.intelligent) return ret;

	if (ret>=0xf0 && ret<=0xf7) { /* MIDI data request */
		mpu.state.channel=ret&7;
		mpu.state.data_onoff=0;
		mpu.state.cond_req=false;
	}
	if (ret==MSG_MPU_COMMAND_REQ) {
		mpu.state.data_onoff=0;
		mpu.state.cond_req=true;
		if (mpu.condbuf.type!=T_OVERFLOW) {
			mpu.state.block_ack=true;
			MPU401_WriteCommand(mpu.condbuf.value[0]);
			if (mpu.state.command_byte) MPU401_WriteData(mpu.condbuf.value[1]);
		}
	mpu.condbuf.type=T_OVERFLOW;
	}
	if (ret==MSG_MPU_END || ret==MSG_MPU_CLOCK || ret==MSG_MPU_ACK) {
		mpu.state.data_onoff=-1;
		MPU401_EOIHandlerDispatch();
	}
	return ret;
}

void MPU401_WriteData(Bit8u val) { /* SOFTMPU */
	static Bit8u length,cnt,posd; /* SOFTMPU */
	if (mpu.mode==M_UART) { MIDI_RawOutByte(val); return; }
	switch (mpu.state.command_byte) {       /* 0xe# command data */
		case 0x00:
			break;
		case 0xe0:      /* Set tempo */
			mpu.state.command_byte=0;
			mpu.clock.tempo=val;
			return;
		case 0xe1:      /* Set relative tempo */
			mpu.state.command_byte=0;
			/*if (val!=0x40) //default value
				LOG(LOG_MISC,LOG_ERROR)("MPU-401:Relative tempo change not implemented");*/ /* SOFTMPU */
			return;
		case 0xe7:      /* Set internal clock to host interval */
			mpu.state.command_byte=0;
			mpu.clock.cth_rate=val>>2;
			return;
		case 0xec:      /* Set active track mask */
			mpu.state.command_byte=0;
			mpu.state.tmask=val;
			return;
		case 0xed: /* Set play counter mask */
			mpu.state.command_byte=0;
			mpu.state.cmask=val;
			return;
		case 0xee: /* Set 1-8 MIDI channel mask */
			mpu.state.command_byte=0;
			mpu.state.midi_mask&=0xff00;
			mpu.state.midi_mask|=val;
			return;
		case 0xef: /* Set 9-16 MIDI channel mask */
			mpu.state.command_byte=0;
			mpu.state.midi_mask&=0x00ff;
			mpu.state.midi_mask|=((Bit16u)val)<<8;
			return;
		//case 0xe2:    /* Set graduation for relative tempo */
		//case 0xe4:    /* Set metronome */
		//case 0xe6:    /* Set metronome measure length */
		case 0xfe: /* Config byte */
			mpu.state.command_byte=0;
			mpu.config=val;
			MPU401_Init();
			QueueByte(MSG_MPU_ACK);
			return;
		default:
			mpu.state.command_byte=0;
			return;
	}
	if (mpu.state.wsd) {    /* Directly send MIDI message */
		if (mpu.state.wsd_start) {
			mpu.state.wsd_start=0;
			cnt=0;
				switch (val&0xf0) {
					case 0xc0:case 0xd0:
						mpu.playbuf[mpu.state.channel].value[0]=val;
						length=2;
						break;
					case 0x80:case 0x90:case 0xa0:case 0xb0:case 0xe0:
						mpu.playbuf[mpu.state.channel].value[0]=val;
						length=3;
						break;
					case 0xf0:
						/*LOG(LOG_MISC,LOG_ERROR)("MPU-401:Illegal WSD byte");*/ /* SOFTMPU */
						mpu.state.wsd=0;
						mpu.state.channel=mpu.state.old_chan;
						return;
					default: /* MIDI with running status */
						cnt++;
						MIDI_RawOutByte(mpu.playbuf[mpu.state.channel].value[0]);
				}
		}
		if (cnt<length) {MIDI_RawOutByte(val);cnt++;}
		if (cnt==length) {
			mpu.state.wsd=0;
			mpu.state.channel=mpu.state.old_chan;
		}
		return;
	}
	if (mpu.state.wsm) {    /* Directly send system message */
		if (val==MSG_EOX) {MIDI_RawOutByte(MSG_EOX);mpu.state.wsm=0;return;}
		if (mpu.state.wsd_start) {
			mpu.state.wsd_start=0;
			cnt=0;
			switch (val) {
				case 0xf2:{ length=3; break;}
				case 0xf3:{ length=2; break;}
				case 0xf6:{ length=1; break;}
				case 0xf0:{ length=0; break;}
				default:
					length=0;
			}
		}
		if (!length || cnt<length) {MIDI_RawOutByte(val);cnt++;}
		if (cnt==length) mpu.state.wsm=0;
		return;
	}
	if (mpu.state.cond_req) { /* Command */
		switch (mpu.state.data_onoff) {
			case -1:
				return;
			case  0: /* Timing byte */
				mpu.condbuf.vlength=0;
				if (val<0xf0) mpu.state.data_onoff++;
				else {
					mpu.state.data_onoff=-1;
					MPU401_EOIHandlerDispatch();
					return;
				}
				if (val==0) mpu.state.send_now=true;
				else mpu.state.send_now=false;
				mpu.condbuf.counter=val;
				break;
			case  1: /* Command byte #1 */
				mpu.condbuf.type=T_COMMAND;
				if (val==0xf8 || val==0xf9) mpu.condbuf.type=T_OVERFLOW;
				mpu.condbuf.value[mpu.condbuf.vlength]=val;
				mpu.condbuf.vlength++;
				if ((val&0xf0)!=0xe0) MPU401_EOIHandlerDispatch();
				else mpu.state.data_onoff++;
				break;
			case  2:/* Command byte #2 */
				mpu.condbuf.value[mpu.condbuf.vlength]=val;
				mpu.condbuf.vlength++;
				MPU401_EOIHandlerDispatch();
				break;
		}
		return;
	}
	switch (mpu.state.data_onoff) { /* Data */
		case   -1:
			return;
		case    0: /* Timing byte */
			if (val<0xf0) mpu.state.data_onoff=1;
			else {
				mpu.state.data_onoff=-1;
				MPU401_EOIHandlerDispatch();
				return;
			}
			if (val==0) mpu.state.send_now=true;
			else mpu.state.send_now=false;
			mpu.playbuf[mpu.state.channel].counter=val;
			break;
		case    1: /* MIDI */
			mpu.playbuf[mpu.state.channel].vlength++;
			posd=mpu.playbuf[mpu.state.channel].vlength;
			if (posd==1) {
				switch (val&0xf0) {
					case 0xf0: /* System message or mark */
						if (val>0xf7) {
							mpu.playbuf[mpu.state.channel].type=T_MARK;
							mpu.playbuf[mpu.state.channel].sys_val=val;
							length=1;
						} else {
							/*LOG(LOG_MISC,LOG_ERROR)("MPU-401:Illegal message");*/ /* SOFTMPU */
							mpu.playbuf[mpu.state.channel].type=T_MIDI_SYS;
							mpu.playbuf[mpu.state.channel].sys_val=val;
							length=1;
						}
						break;
					case 0xc0: case 0xd0: /* MIDI Message */
						mpu.playbuf[mpu.state.channel].type=T_MIDI_NORM;
						length=mpu.playbuf[mpu.state.channel].length=2;
						break;
					case 0x80: case 0x90: case 0xa0:  case 0xb0: case 0xe0: 
						mpu.playbuf[mpu.state.channel].type=T_MIDI_NORM;
						length=mpu.playbuf[mpu.state.channel].length=3;
						break;
					default: /* MIDI data with running status */
						posd++;
						mpu.playbuf[mpu.state.channel].vlength++;
						mpu.playbuf[mpu.state.channel].type=T_MIDI_NORM;
						length=mpu.playbuf[mpu.state.channel].length;
						break;
				}
			}
			if (!(posd==1 && val>=0xf0)) mpu.playbuf[mpu.state.channel].value[posd-1]=val;
			if (posd==length) MPU401_EOIHandlerDispatch();
	}
}

static void MPU401_IntelligentOut(Bit8u chan) {
	Bit8u val, i; /* SOFTMPU */
	switch (mpu.playbuf[chan].type) {
		case T_OVERFLOW:
			break;
		case T_MARK:
			val=mpu.playbuf[chan].sys_val;
			if (val==0xfc) {
				MIDI_RawOutByte(val);
				mpu.state.amask&=~(1<<chan);
				mpu.state.req_mask&=~(1<<chan);
			}
			break;
		case T_MIDI_NORM:
			for (i=0;i<mpu.playbuf[chan].vlength;i++)
				MIDI_RawOutByte(mpu.playbuf[chan].value[i]);
			break;
		default:
			break;
	}
}

static void UpdateTrack(Bit8u chan) {
	MPU401_IntelligentOut(chan);
	if (mpu.state.amask&(1<<chan)) {
		mpu.playbuf[chan].vlength=0;
		mpu.playbuf[chan].type=T_OVERFLOW;
		mpu.playbuf[chan].counter=0xf0;
		mpu.state.req_mask|=(1<<chan);
	} else {
		if (mpu.state.amask==0 && !mpu.state.conductor) mpu.state.req_mask|=(1<<12);
	}
}

static void UpdateConductor(void) {
	if (mpu.condbuf.value[0]==0xfc) {
		mpu.condbuf.value[0]=0;
		mpu.state.conductor=false;
		mpu.state.req_mask&=~(1<<9);
		if (mpu.state.amask==0) mpu.state.req_mask|=(1<<12);
		return;
	}
	mpu.condbuf.vlength=0;
	mpu.condbuf.counter=0xf0;
	mpu.state.req_mask|=(1<<9);
}

void MPU401_Event(void) {
	/* SOFTMPU */
	Bit8u i;
	Bitu new_time;
	if (mpu.mode==M_UART) return;
	if (mpu.state.irq_pending) goto next_event;
	for (i=0;i<8;i++) { /* Decrease counters */
		if (mpu.state.amask&(1<<i)) {
			mpu.playbuf[i].counter--;
			if (mpu.playbuf[i].counter<=0) UpdateTrack(i);
		}
	}               
	if (mpu.state.conductor) {
		mpu.condbuf.counter--;
		if (mpu.condbuf.counter<=0) UpdateConductor();
	}
	if (mpu.clock.clock_to_host) {
		mpu.clock.cth_counter++;
		if (mpu.clock.cth_counter >= mpu.clock.cth_rate) {
			mpu.clock.cth_counter=0;
			mpu.state.req_mask|=(1<<13);
		}
	}
	if (!mpu.state.irq_pending && mpu.state.req_mask) MPU401_EOIHandler();
next_event:
	PIC_RemoveEvents(MPU_EVENT);
	if ((new_time=mpu.clock.tempo*mpu.clock.timebase)==0) return;
        PIC_AddEvent(MPU_EVENT,(Bitu)MPU401_TIMECONSTANT/new_time);
}

static void MPU401_EOIHandlerDispatch(void) {
	if (mpu.state.send_now) {
		mpu.state.eoi_scheduled=true;
                PIC_AddEvent(EOI_HANDLER,1); // Possible a bit longer
	}
	else if (!mpu.state.eoi_scheduled) MPU401_EOIHandler();
}

//Updates counters and requests new data on "End of Input"
void MPU401_EOIHandler(void) {
	Bit8u i=0; /* SOFTMPU */
	mpu.state.eoi_scheduled=false;
	if (mpu.state.send_now) {
		mpu.state.send_now=false;
		if (mpu.state.cond_req) UpdateConductor();
		else UpdateTrack(mpu.state.channel);
	}
	mpu.state.irq_pending=false;
	if (!mpu.state.playing || !mpu.state.req_mask) return;
	do {
		if (mpu.state.req_mask&(1<<i)) {
			QueueByte(0xf0+i);
			mpu.state.req_mask&=~(1<<i);
			break;
		}
	} while ((i++)<16);
}

void MPU401_ResetDone(void) { /* SOFTMPU */
	mpu.state.reset=false;
        if (mpu.state.cmd_pending) {
		MPU401_WriteCommand(mpu.state.cmd_pending-1);
		mpu.state.cmd_pending=0;
	}
}

static void MPU401_Reset(void) {
	Bit8u i; /* SOFTMPU */

	mpu.mode=(mpu.intelligent ? M_INTELLIGENT : M_UART);
	PIC_RemoveEvents(EOI_HANDLER);
	mpu.state.eoi_scheduled=false;
	mpu.state.wsd=false;
	mpu.state.wsm=false;
	mpu.state.conductor=false;
	mpu.state.cond_req=false;
	mpu.state.cond_set=false;
	mpu.state.playing=false;
	mpu.state.run_irq=false;
	mpu.state.irq_pending=false;
	mpu.state.cmask=0xff;
	mpu.state.amask=mpu.state.tmask=0;
	mpu.state.midi_mask=0xffff;
	mpu.state.data_onoff=0;
	mpu.state.command_byte=0;
	mpu.state.block_ack=false;
	mpu.clock.tempo=mpu.clock.old_tempo=100;
        mpu.clock.timebase=mpu.clock.old_timebase=120/(RTCFREQ/1000);
	mpu.clock.tempo_rel=mpu.clock.old_tempo_rel=40;
	mpu.clock.tempo_grad=0;
	mpu.clock.clock_to_host=false;
	mpu.clock.cth_rate=60;
	mpu.clock.cth_counter=0;
	ClrQueue();
	mpu.state.req_mask=0;
	mpu.condbuf.counter=0;
	mpu.condbuf.type=T_OVERFLOW;
	for (i=0;i<8;i++) {mpu.playbuf[i].type=T_OVERFLOW;mpu.playbuf[i].counter=0;}
}

/* HardMPU: Initialisation */
void MPU401_Init()
{
	/* Initalise PIC code */
	PIC_Init();

	/* Initialise MIDI handler */
        MIDI_Init(CONFIG_SYSEXDELAY,CONFIG_FAKEALLNOTESOFF);
	if (!MIDI_Available()) return;

	mpu.queue_used=0;
	mpu.queue_pos=0;
	mpu.mode=M_UART;

        mpu.intelligent=true; /* Default is on */
	if (!mpu.intelligent) return;

        /* SOFTMPU: Moved IRQ 9 handler init to asm */
	MPU401_Reset();
}

/* HardMPU: Output a byte to the physical UART */
void output_to_uart(Bit8u val)
{
	if (mpu.config & CONFIG_MIDIPORT)
	{
		UDR1 = val;
	}
	else
	{
		UDR0 = val;
	}
}

/* HardMPU: Wait for UART TX buffer to be empty */
void wait_for_uart()
{
	if (mpu.config & CONFIG_MIDIPORT)
	{
		loop_until_bit_is_set(UCSR1A, UDRE1);
	}
	else
	{
		loop_until_bit_is_set(UCSR0A, UDRE0);
	}
}

/* HardMPU: Check UART TX status, returns 0 for ready */
Bit8u uart_tx_status()
{
	if (mpu.config & CONFIG_MIDIPORT)
	{
		return bit_is_clear(UCSR1A, UDRE1);
	}
	else
	{
		return bit_is_clear(UCSR0A, UDRE0);
	}
}