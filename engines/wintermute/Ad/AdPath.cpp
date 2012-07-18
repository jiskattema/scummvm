/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

/*
 * This file is based on WME Lite.
 * http://dead-code.org/redir.php?target=wmelite
 * Copyright (c) 2011 Jan Nedoma
 */

#include "engines/wintermute/dcgf.h"
#include "engines/wintermute/Ad/AdPath.h"
#include "engines/wintermute/Base/BPoint.h"

namespace WinterMute {

IMPLEMENT_PERSISTENT(CAdPath, false)

//////////////////////////////////////////////////////////////////////////
CAdPath::CAdPath(CBGame *inGame): CBBase(inGame) {
	_currIndex = -1;
	_ready = false;
}


//////////////////////////////////////////////////////////////////////////
CAdPath::~CAdPath() {
	reset();
}


//////////////////////////////////////////////////////////////////////////
void CAdPath::reset() {
	for (int i = 0; i < _points.getSize(); i++)
		delete _points[i];

	_points.removeAll();
	_currIndex = -1;
	_ready = false;
}


//////////////////////////////////////////////////////////////////////////
CBPoint *CAdPath::getFirst() {
	if (_points.getSize() > 0) {
		_currIndex = 0;
		return _points[_currIndex];
	} else return NULL;
}


//////////////////////////////////////////////////////////////////////////
CBPoint *CAdPath::getNext() {
	_currIndex++;
	if (_currIndex < _points.getSize()) return _points[_currIndex];
	else return NULL;
}


//////////////////////////////////////////////////////////////////////////
CBPoint *CAdPath::getCurrent() {
	if (_currIndex >= 0 && _currIndex < _points.getSize()) return _points[_currIndex];
	else return NULL;
}


//////////////////////////////////////////////////////////////////////////
void CAdPath::addPoint(CBPoint *point) {
	_points.add(point);
}


//////////////////////////////////////////////////////////////////////////
bool CAdPath::setReady(bool ready) {
	bool orig = _ready;
	_ready = ready;

	return orig;
}


//////////////////////////////////////////////////////////////////////////
ERRORCODE CAdPath::persist(CBPersistMgr *persistMgr) {

	persistMgr->transfer(TMEMBER(_gameRef));

	persistMgr->transfer(TMEMBER(_currIndex));
	_points.persist(persistMgr);
	persistMgr->transfer(TMEMBER(_ready));

	return STATUS_OK;
}

} // end of namespace WinterMute
