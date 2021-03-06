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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef MYST_SCRIPTS_DEMO_H
#define MYST_SCRIPTS_DEMO_H

#include "common/scummsys.h"
#include "common/util.h"
#include "mohawk/myst_stacks/intro.h"

namespace Mohawk {

struct MystScriptEntry;

namespace MystStacks {

#define DECLARE_OPCODE(x) void x(uint16 op, uint16 var, uint16 argc, uint16 *argv)

class Demo : public Intro {
public:
	Demo(MohawkEngine_Myst *vm);
	~Demo();

	void disablePersistentScripts();
	void runPersistentScripts();

private:
	void setupOpcodes();

	DECLARE_OPCODE(o_stopIntro);
	DECLARE_OPCODE(o_fadeFromBlack);
	DECLARE_OPCODE(o_fadeToBlack);

	DECLARE_OPCODE(o_returnToMenu_init);

	DECLARE_OPCODE(opcode_300);

	bool _returnToMenuRunning;
	uint16 _returnToMenuStep; // 42
	uint32 _returnToMenuNextTime; // 6

	void returnToMenu_run();
};

} // End of namespace MystStacks
} // End of namespace Mohawk

#undef DECLARE_OPCODE

#endif
