/* ScummVM - Scumm Interpreter
 * Copyright (C) 2002-2006 The ScummVM project
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
 * $URL$
 * $Id$
 */

#ifndef COMMON_STRING_H
#define COMMON_STRING_H

#include "common/scummsys.h"
#include "common/array.h"

#include <assert.h>
#include <string.h>

namespace Common {

class String {
protected:
	char		*_str;
	int 		_len;
	mutable int *_refCount;
	int 		_capacity;

public:
#if !(defined(PALMOS_ARM) || defined(PALMOS_DEBUG) || defined(__GP32__) || defined(IPOD))
	static const String emptyString;
#else
	static const char *emptyString;
#endif

	String() : _str(0), _len(0), _refCount(0), _capacity(0) {}
	String(const char *str, int len = -1, int capacity = 16);
	String(const String &str);
	virtual ~String();

	String &operator  =(const char *str);
	String &operator  =(const String &str);
	String &operator  =(char c);
	String &operator +=(const char *str);
	String &operator +=(const String &str);
	String &operator +=(char c);

	bool operator ==(const String &x) const;
	bool operator ==(const char *x) const;
	bool operator !=(const String &x) const;
	bool operator !=(const char *x) const;
	bool operator <(const String &x) const;
	bool operator <=(const String &x) const;
	bool operator >(const String &x) const;
	bool operator >=(const String &x) const;

	bool equals(const String &x) const;
	bool equalsIgnoreCase(const String &x) const;
	int compareTo(const String &x) const;	// strcmp clone
	int compareToIgnoreCase(const String &x) const;	// stricmp clone

	bool equals(const char *x) const;
	bool equalsIgnoreCase(const char *x) const;
	int compareTo(const char *x) const;	// strcmp clone
	int compareToIgnoreCase(const char *x) const;	// stricmp clone

	bool hasSuffix(const String &x) const;
	bool hasSuffix(const char *x) const;

	bool hasPrefix(const String &x) const;
	bool hasPrefix(const char *x) const;

	bool contains(const String &x) const;
	bool contains(const char *x) const;
	bool contains(char x) const;

	/**
	 * Simple DOS-style pattern matching function (understands * and ? like used in DOS).
	 * Taken from exult/files/listfiles.cc
	 *
	 * Token meaning:
	 *		"*": any character, any amount of times.
	 *		"?": any character, only once.
	 *
	 * Example strings/patterns:
	 *		String: monkey.s01	 Pattern: monkey.s??	=> true
	 *		String: monkey.s101	 Pattern: monkey.s??	=> false
	 *		String: monkey.s99	 Pattern: monkey.s?1	=> false
	 *		String: monkey.s101	 Pattern: monkey.s*		=> true
	 *		String: monkey.s99	 Pattern: monkey.s*1	=> false
	 *
	 * @param str Text to be matched against the given pattern.
	 * @param pat Glob pattern.
	 * @param ignoreCase Whether to ignore the case when doing pattern match
	 * @param pathMode Whether to use path mode, i.e., whether slashes must be matched explicitly.
	 *
	 * @return true if str matches the pattern, false otherwise.
	 */
	bool matchString(const char *pat, bool ignoreCase = false, bool pathMode = false) const;
	bool matchString(const String &pat, bool ignoreCase = false, bool pathMode = false) const;

//	bool hasSuffix(const char *x) const;
//	bool hasPrefix(const char *x) const;

	const char *c_str() const		{ return _str ? _str : ""; }
	uint size() const				{ return _len; }

	bool empty() const	{ return (_len == 0); }
	char lastChar() const	{ return (_len > 0) ? _str[_len-1] : 0; }

	char operator [](int idx) const {
		assert(_str && idx >= 0 && idx < _len);
		return _str[idx];
	}

	char &operator [](int idx) {
		assert(_str && idx >= 0 && idx < _len);
		return _str[idx];
	}

	void deleteLastChar();
	void deleteChar(int p);
	void clear();
	void insertChar(char c, int p);

	void toLowercase();
	void toUppercase();

public:
	typedef char *        iterator;
	typedef const char *  const_iterator;

	iterator		begin() {
		return _str;
	}

	iterator		end() {
		return begin() + size();
	}

	const_iterator	begin() const {
		return _str;
	}

	const_iterator	end() const {
		return begin() + size();
	}

protected:
	void ensureCapacity(int new_len, bool keep_old);
	void incRefCount() const;
	void decRefCount();
};

// Append two strings to form a new (temp) string
String operator +(const String &x, const String &y);
String operator +(const char *x, const String &y);
String operator +(const String &x, const char *y);

// Some useful additional comparision operators for Strings
bool operator == (const char *x, const String &y);
bool operator != (const char *x, const String &y);

class StringList : public Array<String> {
public:
	void push_back(const char *str) {
		ensureCapacity(_size + 1);
		_data[_size++] = str;
	}

	void push_back(const String &str) {
		ensureCapacity(_size + 1);
		_data[_size++] = str;
	}
};

}	// End of namespace Common

#endif
