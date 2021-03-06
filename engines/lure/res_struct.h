/* ScummVM - Scumm Interpreter
 * Copyright (C) 2005-2006 The ScummVM project
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

#ifndef __lure_resstruct_h__
#define __lure_resstruct_h__

#include "lure/luredefs.h"
#include "common/list.h"

namespace Lure {

using namespace Common;

extern const char *actionList[];

/*-------------------------------------------------------------------------*/
/* Structure definitions                                                   */
/*                                                                         */
/*-------------------------------------------------------------------------*/

#if !defined(__GNUC__)
#pragma START_PACK_STRUCTS
#endif

struct VersionStructure {
	uint16 id;
	byte vMajor;
	byte vMinor;
} GCC_PACK;

struct FileEntry {
	uint16 id;
	byte unused;
	byte sizeExtension;
	uint16 size;
	uint16 offset;
} GCC_PACK;

struct HotspotResource {
	uint16 hotspotId;
	uint16 nameId;
	uint16 descId;
	uint16 descId2;
	uint32 actions;
	uint16 actionsOffset;
	uint16 roomNumber;
	byte layer;
	byte scriptLoadFlag;
	uint16 loadOffset;
	uint16 startX;
	uint16 startY;
	uint16 width;
	uint16 height;
	uint16 widthCopy;
	uint16 heightCopy;
	uint16 yCorrection;
	int16 walkX;
	uint16 walkY;
	int8 talkX;
	int8 talkY;
	uint16 colourOffset;
	uint16 animRecordId;
	uint16 sequenceOffset;
	uint16 tickProcOffset;
	uint16 tickTimeout;
	uint16 tickSequenceOffset;
	uint16 npcSchedule;
} GCC_PACK;

struct HotspotAnimResource {
	uint16 animRecordId;
	uint16 animId;
	uint16 flags;
	uint16 upOffset;
	uint16 downOffset;
	uint16 leftOffset;
	uint16 rightOffset;
	uint8 upFrame;
	uint8 downFrame;
	uint8 leftFrame;
	uint8 rightFrame;
} GCC_PACK;

struct MovementResource {
	uint16 frameNumber;
	int16 xChange;
	int16 yChange;
} GCC_PACK;

struct RoomRect {
	int16 xs, xe;
	int16 ys, ye;
};

struct RoomResource {
	uint16 roomNumber;
	uint16 descId;
	uint16 numLayers;
	uint16 layers[4];
	uint16 sequenceOffset;
	int16 clippingXStart;
	int16 clippingXEnd;
	RoomRect walkBounds;
	uint16 numExits;
} GCC_PACK;

struct RoomExitResource {
	int16 xs, xe, ys, ye;
	uint16 sequenceOffset;
	uint8 newRoom;
	uint8 direction;
	int16 newRoomX, newRoomY;
} GCC_PACK;

struct HotspotOverrideResource {
	uint16 hotspotId;
	int16 xs, xe, ys, ye;
} GCC_PACK;

struct RoomExitHotspotResource {
	uint16 hotspotId;
	int16 xs, xe;
	int16 ys, ye;
	uint16 cursorNum;
	uint16 destRoomNumber;
} GCC_PACK;

struct RoomExitJoinResource {
	uint16 hotspot1Id;
	byte h1CurrentFrame;
	byte h1DestFrame;
	uint8 h1OpenSound;
	uint8 h1CloseSound;
	uint16 hotspot2Id;
	byte h2CurrentFrame;
	byte h2DestFrame;
	uint8 h2OpenSound;
	uint8 h2CloseSound;
	byte blocked;
} GCC_PACK;

struct HotspotActionResource {
	byte action;
	uint16 sequenceOffset;
} GCC_PACK;

struct TalkHeaderResource {
	uint16 hotspotId;
	uint16 offset;
} GCC_PACK;

struct TalkDataHeaderResource {
	uint16 recordId;
	uint16 listOffset;
	uint16 responsesOffset;
} GCC_PACK;

struct TalkDataResource {
	uint16 preSequenceId;
	uint16 descId;
	uint16 postSequenceId;
} GCC_PACK;

struct TalkResponseResource {
	uint16 sequenceId1;
	uint16 sequenceId2;
	uint16 sequenceId3;
} GCC_PACK;

struct RoomExitCoordinateResource {
	int16 x;
	int16 y;
	uint16 roomNumber;
} GCC_PACK;

#define ROOM_EXIT_COORDINATES_NUM_ENTRIES 6
#define ROOM_EXIT_COORDINATES_NUM_ROOMS 52

struct RoomExitCoordinateEntryResource {
	RoomExitCoordinateResource entries[ROOM_EXIT_COORDINATES_NUM_ENTRIES];
	uint8 roomIndex[ROOM_EXIT_COORDINATES_NUM_ROOMS];
} GCC_PACK;

#define MAX_SCHEDULE_ENTRY_PARAMS 5

struct CharacterScheduleResource {
	uint16 action;
	uint16 params[MAX_SCHEDULE_ENTRY_PARAMS];
};

struct RoomExitIndexedHotspotResource {
	uint8 roomNumber;
	uint8 hotspotIndex;
	uint16 hotspotId;
};

#if !defined(__GNUC__)
#pragma END_PACK_STRUCTS
#endif

// Class template for a derived list that destroys the contained
// object when the record containing it is destroyed. It's not
// perfect, since the underlying list doesn't have virtual
// methods, but it's sufficient for my usage

template <class T>
class ManagedList: public Common::List<T> {
	typedef typename Common::List<T> Common_List;
public:
	~ManagedList() {
		clear();
	}

	void clear() {
		typename Common::List<T>::iterator i;
		for (i = Common_List::begin(); i != Common_List::end(); ++i)
			delete *i;
		Common_List::clear();		
	}

	typename Common::List<T>::iterator erase(typename Common::List<T>::iterator pos) {
		delete *pos;
		return Common_List::erase(pos);
	}

	typename Common::List<T>::iterator erase(typename Common::List<T>::iterator first, 
			typename Common::List<T>::iterator last) {
		typename Common::List<T>::iterator i;
		for (i = first; i != last; ++i)
			delete *i;
		return Common_List::erase(first, last);
	}
};

// Enumeration used for direction facings

enum Direction {UP, DOWN, LEFT, RIGHT, NO_DIRECTION};

// Support classes to hold loaded resources

class RoomExitHotspotData {
public:
	RoomExitHotspotData(RoomExitHotspotResource *rec);

	uint16 hotspotId;
	int16 xs, xe;
	int16 ys, ye;
	uint16 cursorNum;
	uint16 destRoomNumber;
};

typedef ManagedList<RoomExitHotspotData *> RoomExitHotspotList;

class RoomExitData {
public:
	RoomExitData(RoomExitResource *rec);
	bool insideRect(int16 xp, int16 yp);

	int16 xs, xe, ys, ye;
	uint16 sequenceOffset;
	Direction direction;
	uint8 roomNumber;
	uint16 x, y;
};

class RoomExitList: public ManagedList<RoomExitData *> {
public:
	RoomExitData *checkExits(int16 xp, int16 yp);
};

#define ROOM_PATHS_WIDTH 40
#define ROOM_PATHS_HEIGHT 24
#define ROOM_PATHS_SIZE (ROOM_PATHS_WIDTH / 8 * ROOM_PATHS_HEIGHT)
#define DECODED_PATHS_WIDTH 42
#define DECODED_PATHS_HEIGHT 26

typedef uint16 RoomPathsDecompressedData[DECODED_PATHS_WIDTH * DECODED_PATHS_HEIGHT];

class RoomPathsData {
private:
	byte _data[ROOM_PATHS_HEIGHT * ROOM_PATHS_WIDTH];
public:
	RoomPathsData() {};
	RoomPathsData(byte *srcData) { load(srcData); }

	void load(byte *srcData) {
		memcpy(_data, srcData, ROOM_PATHS_SIZE);
	}
	bool isOccupied(int x, int y);
	void setOccupied(int x, int y, int width);
	void clearOccupied(int x, int y, int width);
	void decompress(RoomPathsDecompressedData &dataOut, int characterWidth);
};

#define MAX_NUM_LAYERS 4

class RoomData {
public:
	RoomData(RoomResource *rec, MemoryBlock *pathData);

	uint16 roomNumber;
	uint16 descId;
	uint16 numLayers;
	uint16 layers[MAX_NUM_LAYERS];
	uint16 sequenceOffset;
	int16 clippingXStart;
	int16 clippingXEnd;
	Common::Rect walkBounds;
	RoomExitHotspotList exitHotspots;
	RoomExitList exits;
	RoomPathsData paths;
};

typedef ManagedList<RoomData *> RoomDataList;

class RoomExitJoinData {
public:
	RoomExitJoinData(RoomExitJoinResource *rec);

	uint16 hotspot1Id;
	byte h1CurrentFrame;
	byte h1DestFrame;
	uint8 h1OpenSound;
	uint8 h1CloseSound;
	uint16 hotspot2Id;
	byte h2CurrentFrame;
	byte h2DestFrame;
	uint8 h2OpenSound;
	uint8 h2CloseSound;
	byte blocked;
	uint32 unknown;
};

typedef ManagedList<RoomExitJoinData *> RoomExitJoinList;

class HotspotActionData {
public:
	HotspotActionData(HotspotActionResource *rec);

	Action action;
	uint16 sequenceOffset;
};

class HotspotActionList: public ManagedList<HotspotActionData *> {
public:
	uint16 recordId;

	HotspotActionList(uint16 id, byte *data);
	uint16 getActionOffset(Action action);
};

class HotspotActionSet: public ManagedList<HotspotActionList *> {
public:
	HotspotActionList *getActions(uint16 recordId);
};

enum CharacterMode {CHARMODE_NONE, CHARMODE_1, CHARMODE_IDLE, CHARMODE_PAUSED,
	CHARMODE_4, CHARMODE_5, CHARMODE_6, CHARMODE_7};

class HotspotData {
public:
	HotspotData(HotspotResource *rec);
	
	uint16 hotspotId;
	uint16 nameId;
	uint16 descId;
	uint16 descId2;
	uint32 actions;
	uint16 actionsOffset;
	byte flags;
	uint16 roomNumber;
	byte layer;
	byte scriptLoadFlag;
	uint16 loadOffset;
	int16 startX;
	int16 startY;
	uint16 width;
	uint16 height;
	uint16 widthCopy;
	uint16 heightCopy;
	uint16 yCorrection;
	int16 walkX;
	uint16 walkY;
	int8 talkX;
	int8 talkY;
	uint16 colourOffset;
	uint16 animRecordId;
	uint16 sequenceOffset;
	uint16 tickProcOffset;
	uint16 tickTimeout;
	uint16 tickSequenceOffset;
	uint16 npcSchedule;

	uint16 delayCtr;
	CharacterMode characterMode;
	bool coveredFlag;

	void enable() { flags |= 0x80; }
	void disable() { flags &= 0x7F; }
	Direction nonVisualDirection() { return (Direction) scriptLoadFlag; }
};

typedef ManagedList<HotspotData *> HotspotDataList;

class HotspotOverrideData {
public:
	HotspotOverrideData(HotspotOverrideResource *rec);

	uint16 hotspotId;
	int16 xs, xe, ys, ye;
};

typedef ManagedList<HotspotOverrideData *> HotspotOverrideList;

class MovementData {
public:
	MovementData(MovementResource *);

	uint16 frameNumber;
	int16 xChange;
	int16 yChange;
};

class MovementDataList: public ManagedList<MovementData *> {
public:
	bool getFrame(uint16 currentFrame, int16 &xChange, int16 &yChange, 
		uint16 &nextFrame);
};

class HotspotAnimData {
public:
	HotspotAnimData(HotspotAnimResource *rec);

	uint16 animRecordId;
	uint16 animId;
	uint16 flags;
	uint8 upFrame;
	uint8 downFrame;
	uint8 leftFrame;
	uint8 rightFrame;

	MovementDataList leftFrames, rightFrames;
	MovementDataList upFrames, downFrames;
};

typedef ManagedList<HotspotAnimData *> HotspotAnimList;

// Talk header list

class TalkHeaderData {
private:
	uint16 *_data;
	int _numEntries;
public:
	TalkHeaderData(uint16 charId, uint16 *entries);
	~TalkHeaderData();

	uint16 characterId;
	uint16 getEntry(int index);
};

typedef ManagedList<TalkHeaderData *> TalkHeaderList;

class TalkEntryData {
public:
	TalkEntryData(TalkDataResource *rec);

	uint16 preSequenceId;
	uint16 descId;
	uint16 postSequenceId;
};

typedef ManagedList<TalkEntryData *> TalkEntryList;

class TalkData {
public:
	TalkData(uint16 id);
	~TalkData();

	uint16 recordId;
	TalkEntryList entries;
	TalkEntryList responses;

	TalkEntryData *getResponse(int index);
};

typedef ManagedList<TalkData *> TalkDataList;

struct RoomExitCoordinateData {
	int16 x;
	int16 y;
	uint16 roomNumber;
	byte hotspotIndexId;
};

class RoomExitCoordinates {
private:
	RoomExitCoordinateData _entries[ROOM_EXIT_COORDINATES_NUM_ENTRIES];
	uint8 _roomIndex[ROOM_EXIT_COORDINATES_NUM_ROOMS];
public:
	RoomExitCoordinates(RoomExitCoordinateEntryResource *rec);
	RoomExitCoordinateData &getData(uint16 destRoomNumber);
};

class RoomExitCoordinatesList: public ManagedList<RoomExitCoordinates *> {
public:
	RoomExitCoordinates &getEntry(uint16 roomNumber);
};

class RoomExitIndexedHotspotData {
public:
	RoomExitIndexedHotspotData(RoomExitIndexedHotspotResource *rec);

	uint16 roomNumber;
	uint8 hotspotIndex;
	uint16 hotspotId;
};

class RoomExitIndexedHotspotList: public ManagedList<RoomExitIndexedHotspotData *> {
public:
	uint16 getHotspot(uint16 roomNumber, uint8 hotspotIndexId);
};

// The following classes hold any sequence offsets that are being delayed

class SequenceDelayData {
	friend class SequenceDelayList;
private:
	uint32 _timeoutCtr;
	uint16 _sequenceOffset;
public:
	SequenceDelayData(uint16 delay, uint16 seqOffset);
};

class SequenceDelayList: public ManagedList<SequenceDelayData *> {
public:
	void addSequence(uint16 delay, uint16 seqOffset);
	void tick();
};

// The following classes holds the data for NPC schedules

extern int actionNumParams[NPC_JUMP_ADDRESS+1];

class CharacterScheduleSet;

class CharacterScheduleEntry {
private:
	CharacterScheduleSet *_parent;
	Action _action;
	uint16 _params[MAX_SCHEDULE_ENTRY_PARAMS];
public:
	CharacterScheduleEntry() { _action = NONE; _parent = NULL; }
	CharacterScheduleEntry(Action theAction, ...);
	CharacterScheduleEntry(CharacterScheduleSet *parentSet, 
		CharacterScheduleResource *&rec);

	Action action() { return _action; }
	int numParams();
	uint16 param(int index);
	void setDetails(Action theAction, ...);
	CharacterScheduleEntry *next();
	CharacterScheduleSet *parent() { return _parent; }
	uint16 id();
};

class CharacterScheduleSet: public ManagedList<CharacterScheduleEntry *> {
private:
	uint16 _id;
public:
	CharacterScheduleSet(CharacterScheduleResource *rec, uint16 setId); 
	uint16 getId(CharacterScheduleEntry *rec);
	uint16 id() { return _id; }
};

class CharacterScheduleList: public ManagedList<CharacterScheduleSet *> {
public:
	CharacterScheduleEntry *getEntry(uint16 id, CharacterScheduleSet *currentSet = NULL);
};

typedef List<uint16> CharacterScheduleOffsets;

// The following class holds the field list used by the script engine as 
// well as miscellaneous fields used by the game.                          

#define NUM_VALUE_FIELDS 90

enum FieldName {
	ROOM_NUMBER = 0, 
	CHARACTER_HOTSPOT_ID = 1, 
	USE_HOTSPOT_ID = 2,
	ACTIVE_HOTSPOT_ID = 3,
	SEQUENCE_RESULT = 4,
	GENERAL = 5,
	NEW_ROOM_NUMBER = 7,
	OLD_ROOM_NUMBER = 8,
	CELL_DOOR_STATE = 9,
	TORCH_HIDE = 10,
	PRISONER_DEAD = 15,
	BOTTLE_FILLED = 18,
	TALK_INDEX = 19,
	SACK_CUT = 20
};

struct PlayerNewPosition {
	Point position;
	uint16 roomNumber;
};

class ValueTableData {
private:
	uint16 _numGroats;
	PlayerNewPosition _playerNewPos;

	uint16 _fieldList[NUM_VALUE_FIELDS];
	bool isKnownField(uint16 fieldIndex);
public:
	ValueTableData();
	uint16 getField(uint16 fieldIndex);
	uint16 getField(FieldName fieldName);

	void setField(uint16 fieldIndex, uint16 value);
	void setField(FieldName fieldName, uint16 value);
	uint16 &numGroats() { return _numGroats; }
	PlayerNewPosition &playerNewPos() { return _playerNewPos; }
};

} // End of namespace Lure

#endif
