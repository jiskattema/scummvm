/* ScummVM - Scumm Interpreter
 * Copyright (C) 2003 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header$
 *
 */

#ifndef MUSICBASE_H
#define MUSICBASE_H

#include "stdafx.h"
#include "common/engine.h"
#include "disk.h"

#define FILES_PER_SECTION 4

typedef struct {
	bool doReInit, doStopMusic;
	uint8 musicToProcess;
} Actions;

class SkyChannelBase {
public:
	virtual void stopNote(void) = 0;
	virtual uint8 process(uint16 aktTime) = 0;
	virtual void updateVolume(uint16 pVolume) = 0;
private:
};

class SkyMusicBase {
public:
	SkyMusicBase(SkyDisk *pSkyDisk);
	virtual ~SkyMusicBase(void);
	void loadSectionMusic(uint8 pSection);
	void musicCommand(uint16 command);
	void startMusic(uint16 param) { _onNextPoll.musicToProcess = param & 0xF; }; // 4
	
protected:
	SkyDisk *_skyDisk;
	uint8 *_musicData;
	uint8 _allowedCommands;
	uint16 _musicDataLoc;
	uint16 _driverFileBase;

	uint16 _musicVolume, _numberOfChannels;
	uint8 _currentMusic, _currentSection;
	uint8 _musicTempo0; // can be changed by music stream
	uint8 _musicTempo1; // given once per music
	uint32 _tempo;      // calculated from musicTempo0 and musicTempo1
	uint32 _aktTime;
	Actions _onNextPoll;
	SkyChannelBase *_channels[10];
	
	virtual void setupPointers(void) = 0;
	virtual void setupChannels(uint8 *channelData) = 0;

	void updateTempo(void);
	void loadNewMusic(void);
	//-                           functions from CommandTable @0x90 (the main interface)
	virtual void startDriver(void) = 0;                                          // 0
	void StopDriver(void);                                                       // 1
	void setTempo(uint16 newTempo);                                              // 2
	void pollMusic();                                                            // 3
	void reinitFM(void) { _onNextPoll.doReInit = true; };                        // 6
	void stopMusic();                                                            // 7
	void setFMVolume(uint16 param);                                              // 13
};

#endif //MUSICBASE_H
