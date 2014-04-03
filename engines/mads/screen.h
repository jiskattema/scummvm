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

#ifndef MADS_SCREEN_H
#define MADS_SCREEN_H

#include "common/scummsys.h"
#include "common/array.h"
#include "mads/msurface.h"

namespace MADS {

#define MADS_SCREEN_WIDTH 320
#define MADS_SCREEN_HEIGHT 200

enum ScrCategory {
	CAT_NONE = 0, CAT_COMMAND = 1, CAT_INV_LIST = 2, CAT_INV_VOCAB = 3,
	CAT_HOTSPOT = 4, CAT_INV_ANIM = 5, CAT_TALK_ENTRY = 6, CAT_INV_SCROLLER = 7,
	CAT_12 = 12
};

enum Layer {
	LAYER_GUI = 19
};

enum ScreenTransition {
	kTransitionNone = 0,
	kTransitionFadeIn, kTransitionFadeOutIn,
	kTransitionBoxInBottomLeft, kTransitionBoxInBottomRight,
	kTransitionBoxInTopLeft, kTransitionBoxInTopRight,
	kTransitionPanLeftToRight, kTransitionPanRightToLeft,
	kTransitionCircleIn1, kTransitionCircleIn2,
	kTransitionCircleIn3, kTransitionCircleIn4,
	kVertTransition1, kVertTransition2, kVertTransition3,
	kVertTransition4, kVertTransition5, kVertTransition6,
	kVertTransition7, kCenterVertTransition
};

class SpriteSlot;
class TextDisplay;
class UISlot;

class DirtyArea {
private:
	static MADSEngine *_vm;
	friend class DirtyAreas;
public:
	Common::Rect _bounds;
	Common::Rect _bounds2;
	bool _textActive;
	bool _active;

	DirtyArea();

	void setArea(int width, int height, int maxWidth, int maxHeight);

	/**
	 * Set up a dirty area for a sprite slot
	 */
	void setSpriteSlot(const SpriteSlot *spriteSlot);

	/**
	* Set up a dirty area for a text display
	*/
	void setTextDisplay(const TextDisplay *textDisplay);

	/**
	 * Set up a dirty area for a UI slot
	 */
	void setUISlot(const UISlot *slot);
};

class DirtyAreas : public Common::Array<DirtyArea> {
private:
	MADSEngine *_vm;
public:
	DirtyAreas(MADSEngine *vm);

	/**
	* Merge together any designated dirty areas that overlap
	* @param startIndex	1-based starting dirty area starting index
	* @param count			Number of entries to process
	*/
	void merge(int startIndex, int count);

	bool intersects(int idx1, int idx2);
	void mergeAreas(int idx1, int idx2);

	/**
	* Copy the data specified by the dirty rect list between surfaces
	* @param srcSurface	Source surface
	* @param destSurface	Dest surface
	* @param posAdjust		Position adjustment
	*/
	void copy(MSurface *srcSurface, MSurface *destSurface, const Common::Point &posAdjust);

	/**
	* Use the lsit of dirty areas to copy areas of the screen surface to
	* the physical screen
	* @param posAdjust		Position adjustment	 */
	void copyToScreen(const Common::Point &posAdjust);

	void reset();
};


class ScreenObject {
public:
	bool _active;
	Common::Rect _bounds;
	ScrCategory _category;
	int _descId;
	int _layer;

	ScreenObject();
};

class ScreenObjects : public Common::Array<ScreenObject> {
private:
	MADSEngine *_vm;
	int _objectY;
	int _scrollerY;

	int scanBackwards(const Common::Point &pt, int layer);

	/**
	 * Checks for the mouse being on the user interface inventory scroller,
	 * and update the scroller and selected inventory object as necessary
	 */
	void checkScroller();
public:
	int _v832EC;
	int _v7FED6;
	int _v8332A;
	int _v8333C;
	int _selectedObject;
	ScrCategory _category;
	int _newDescId;
	int _currentDescId;
	bool _released;
	int _uiCount;
	uint32 _milliTime;
	bool _eventFlag;
	uint32 _baseTime;

	/*
	* Constructor
	*/
	ScreenObjects(MADSEngine *vm);

	/**
	* Add a new item to the list
	*/
	void add(const Common::Rect &bounds, Layer layer, ScrCategory category, int descId);

	/**
	*/
	void check(bool scanFlag);

	/**
	 * Handle an element being highlighted on the screen, and make it active.
	 */
	void elementHighlighted();

	/**
	 * Retrieve a ScreenObject from the list
	 * @remarks	This array is 1-based indexed by the game
	 */
	ScreenObject &operator[](int idx) {
		assert(idx > 0);
		return Common::Array<ScreenObject>::operator[](idx - 1);
	}

	/**
	 * Sets an item identified by category and Desc Id as active or not
	 * @param category		Screen category
	 * @param descId		Description for item
	 * @param active		Whether to set item as active or not
	 */
	void setActive(ScrCategory category, int descId, bool active);
};

class ScreenSurface : public MSurface {
private:
	/**
	 * Handles screen fade out
	 */
	void fadeOut();

	/**
	 * Handles screen fade in
	 */
	void fadeIn();
public:
	Common::Point _offset;
	byte *_dataP;
public:
	/**
	 * Constructor
	 */
	ScreenSurface();

	/**
	 * Initialise the surface
	 */
	void init();

	void setPointer(MSurface *s);

	/**
	 * Copys an area of the screen surface to a given destination position on
	 * the ScummVM physical screen buffer
	 * @param destPos	Destination position
	 * @param bounds	Area of screen surface to copy
	 */
	void copyRectToScreen(const Common::Point &destPos, const Common::Rect &bounds);

	/**
	 * Copys an area of the screen surface to the ScmmVM physical screen buffer
	 * @param bounds	Area of screen surface to copy
	 */
	void copyRectToScreen(const Common::Rect &bounds);

	/**
	 * Updates the screen with the contents of the surface
	 */
	void updateScreen();

	void transition(ScreenTransition transitionType, bool surfaceFlag);
};

} // End of namespace MADS

#endif /* MADS_SCREEN_H */
