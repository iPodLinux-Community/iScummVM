/* ScummVM - Scumm Interpreter
 * Copyright (C) 2001  Ludvig Strigeus
 * Copyright (C) 2001-2006 The ScummVM project
 * Copyright (C) 2002-2006 Chris Apers - PalmOS Backend
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

#ifndef PALM_STRING_H
#define PALM_STRING_H

#include "palmversion.h"

#ifdef __cplusplus
extern "C" {
#endif

/* mapped to system functions */
#define memcmp			MemCmp
#define memcpy			MemMove
#define memmove			MemMove
#define memset(a,b,c)	MemSet(a,c,b)
#define strcat			StrCat
#define strncat			StrNCat
#define strchr			StrChr
#define strcmp			StrCompare
#define strcpy			StrCopy
#define strncpy			StrNCopy
#define stricmp			StrCaselessCompare
#define strnicmp		StrNCaselessCompare
#define strlen			StrLen
#define strncmp			StrNCompare
#define strstr			StrStr

/* missing functions in 68k MSL */
void 	*memchr		(const void *s, int c, UInt32 n);
Char 	*strdup		(const Char *strSource);

/* already defined in MSL */
Char 	*strtok		(Char *str, const Char *sep);
Char 	*strrchr	(const Char *s, int c);
Char 	*strpbrk	(const Char *s1, const Char *s2);
UInt32	 strspn		(const Char *s1, const Char *s2);
UInt32	 strcspn	(const Char *s1, const Char *s2);

#ifdef __cplusplus
}
#endif

#endif
