/* ScummVM - Scumm Interpreter
 * Copyright (C) 2006 The ScummVM project
 *
 * cinE Engine is (C) 2004-2005 by CinE Team
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
 * $URL$
 * $Id$
 *
 */

#ifndef CINE_RESOURCE_H_
#define CINE_RESOURCE_H_

#include "common/stdafx.h"
#include "common/scummsys.h"

namespace Cine {

struct BasesonEntry {
	char name[14];
	uint32 offset;
	uint32 size;
	uint32 unpackedSize;
};

void checkDataDisk(int16 param);
extern int snd_loadBasesonEntries(const char *fileName);
extern void snd_clearBasesonEntries();
extern byte *snd_loadBasesonEntry(const char *entryName);

} // End of namespace Cine

#endif