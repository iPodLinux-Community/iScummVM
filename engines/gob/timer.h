/* ScummVM - Scumm Interpreter
 * Copyright (C) 2004 Ivan Dubrov
 * Copyright (C) 2004-2006 The ScummVM project
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */
#ifndef GOB_TIMER_H
#define GOB_TIMER_H

namespace Gob {

class GTimer {
public:
	typedef void (* TickHandler) (void);

	void enableTimer(void);
	void disableTimer(void);
	void setHandler(void);
	void restoreHandler(void);
	void addTicks(int16 ticks);
	void setTickHandler(TickHandler handler);
	int32 getTicks(void);
};

}				// End of namespace Gob

#endif
