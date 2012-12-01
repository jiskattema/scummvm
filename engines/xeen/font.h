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

#ifndef XEEN_FONT_H
#define XEEN_FONT_H

#include "common/scummsys.h"

namespace XEEN
{
    class ImageBuffer;
    class CCFile;

    class Font
    {
        static const unsigned CHARACTER_COUNT = 256;
    
        public:
            Font(CCFile& cc);
            
            void drawString(ImageBuffer& out, Common::Point pen, const char* text) const;
            
        private:
            struct Glyph
            {
                byte pixels[8 * 8];
                uint8 spacing;
            };
            
            Glyph _glyphs[CHARACTER_COUNT];
    };
}

#endif // XEEN_FONT_H