/* ScummVM - Scumm Interpreter
 * Copyright (C) 2003 The ScummVM project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header$
 *
 */

#include "stdafx.h"
#include "queen/input.h"
#include "common/system.h"
#include "common/util.h"

namespace Queen {

const char* Input::_commandKeys[LANGUAGE_COUNT] = {
	"ocmglptu", // English
	"osbgpnre", // German
	"ofdnepau", // French
	"acsdgpqu", // Italian
	"ocmglptu", // Hebrew
	"acodmthu"  // Spanish
};  

Input::Input(Language language, OSystem *system) : 
	_system(system), _fastMode(false), _keyVerb(VERB_NONE), 
	_cutawayRunning(false), _cutawayQuit(false), _talkQuit(false),
	_inKey(0), _mouse_x(0), _mouse_y(0), _mouseButton(0) {

	switch (language) {
		case ENGLISH:
			_currentCommandKeys = _commandKeys[0];
			break;
		case GERMAN:
			_currentCommandKeys = _commandKeys[1];
			break;
		case FRENCH:
			_currentCommandKeys = _commandKeys[2];
			break;
		case ITALIAN:
			_currentCommandKeys = _commandKeys[3];
			break;
		case HEBREW:
			_currentCommandKeys = _commandKeys[4];
			break;
		case SPANISH:
			_currentCommandKeys = _commandKeys[5];
			break;
		default:
			error("Unknown language");
			break;
	}
}

void Input::delay() {
	delay(_fastMode ? DELAY_SHORT : DELAY_NORMAL);
}

void Input::delay(uint amount) { 

	OSystem::Event event;

	uint32 start = _system->get_msecs();
	uint32 cur = start;

	do {
		while (_system->poll_event(&event)) {
			switch (event.event_code) {
				case OSystem::EVENT_KEYDOWN:
					debug(1, "event.kbd.keycode = %i (%c)", 
							event.kbd.keycode,
							isprint(event.kbd.keycode) ? event.kbd.keycode : '.');

					_inKey = event.kbd.keycode;
					break;

				case OSystem::EVENT_MOUSEMOVE:
					_mouse_x = event.mouse.x;
					_mouse_y = event.mouse.y;
					break;

				case OSystem::EVENT_LBUTTONDOWN:
					_mouseButton |= MOUSE_LBUTTON;
#ifdef _WIN32_WCE
					_mouse_x = event.mouse.x;
					_mouse_y = event.mouse.y;
#endif
					break;

				case OSystem::EVENT_RBUTTONDOWN:
					_mouseButton |= MOUSE_RBUTTON;
					break;

				case OSystem::EVENT_QUIT:
					_system->quit();
					break;

				default:
					break;
			}
		}

		if (amount == 0)
			break;

		{
			uint this_delay = 20; // 1?
			if (this_delay > amount)
				this_delay = amount;
			_system->delay_msecs(this_delay);
		}
		cur = _system->get_msecs();
	} while (cur < start + amount);
}

void Input::checkKeys() {

	if (_inKey)
		debug(0, "[Input::checkKeys] _inKey = %i", _inKey);

	switch (_inKey) {
		case KEY_SPACE:
			_keyVerb = Verb(VERB_SKIP_TEXT);
			break;

		case KEY_COMMA:
			_keyVerb = Verb(VERB_SCROLL_UP);
			break;

		case KEY_DOT:
			_keyVerb = Verb(VERB_SCROLL_DOWN);
			break;

		case KEY_DIGIT_1:
			_keyVerb = Verb(VERB_DIGIT_1);
			break;

		case KEY_DIGIT_2:
			_keyVerb = Verb(VERB_DIGIT_2);
			break;

		case KEY_DIGIT_3:
			_keyVerb = Verb(VERB_DIGIT_3);
			break;

		case KEY_DIGIT_4:
			_keyVerb = Verb(VERB_DIGIT_4);
			break;

		case KEY_ESCAPE:
			if (_canQuit) {
				if (_cutawayRunning) {
					debug(0, "[Input::checkKeys] Setting _cutawayQuit to true!");
					_cutawayQuit = true;
				}

				// XXX if (_joeWalk == 3)	// Dialogue
				// XXX 	_talkQuit = true;
			}
			break;

		case KEY_F1:	// Use Journal
			if (_cutawayRunning) {
				if (_canQuit) {
					_keyVerb = Verb(VERB_USE_JOURNAL);
					_cutawayQuit = _talkQuit = true;
				}
			}
			else {
				_keyVerb = Verb(VERB_USE_JOURNAL);
				if (_canQuit)
					_talkQuit = true;
			}
			break;

		default:
			if(_inKey == _currentCommandKeys[0])
				_keyVerb = Verb(VERB_OPEN);
			else if(_inKey == _currentCommandKeys[1])
				_keyVerb = Verb(VERB_CLOSE);
			else if(_inKey == _currentCommandKeys[2])
				_keyVerb = Verb(VERB_MOVE);
			else if(_inKey == _currentCommandKeys[3])
				_keyVerb = Verb(VERB_GIVE);
			else if(_inKey == _currentCommandKeys[4])
				_keyVerb = Verb(VERB_LOOK_AT);
			else if(_inKey == _currentCommandKeys[5])
				_keyVerb = Verb(VERB_PICK_UP);
			else if(_inKey == _currentCommandKeys[6])
				_keyVerb = Verb(VERB_TALK_TO);
			else if(_inKey == _currentCommandKeys[7])
				_keyVerb = Verb(VERB_USE);
			break;
	}

	_inKey = 0;	//reset
}


} // End of namespace Queen

