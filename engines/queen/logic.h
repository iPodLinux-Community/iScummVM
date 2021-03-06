/* ScummVM - Scumm Interpreter
 * Copyright (C) 2003-2006 The ScummVM project
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

#ifndef QUEENLOGIC_H
#define QUEENLOGIC_H

#include "common/str.h"
#include "common/util.h"
#include "queen/structs.h"

namespace Queen {

enum RoomDisplayMode {
	RDM_FADE_NOJOE  = 0, // fade in, hide Joe
	RDM_FADE_JOE    = 1, // fade in, display Joe
	RDM_NOFADE_JOE  = 2, // screen does not dissolve into view
	RDM_FADE_JOE_XY = 3  // display Joe at the current X, Y coords
};

enum JoeWalkMode {
	JWM_NORMAL  = 0,
	JWM_MOVE    = 1,
	JWM_EXECUTE = 2,
	JWM_SPEAK   = 3
};

class Credits;
class Journal;
class LineReader;
class QueenEngine;

class Logic {

public:
	Logic(QueenEngine *vm);
	virtual ~Logic();

	uint16 currentRoom() const { return _currentRoom; }
	void currentRoom(uint16 room) {
		assert(room >= 1 && room <= _numRooms);
		_currentRoom = room;
	}

	uint16 oldRoom() const { return _oldRoom; }
	void oldRoom(uint16 room) {
		assert(room <= _numRooms);
		_oldRoom = room;
	}

	uint16 newRoom() const { return _newRoom; }
	void newRoom(uint16 room) {
		assert(room <= _numRooms);
		_newRoom = room;
	}

	ObjectData *objectData(int index) const;
	uint16 roomData(int room) const { return _roomData[room]; }
	GraphicData *graphicData(int index) const { return &_graphicData[index]; }
	ItemData *itemData(int index) const { return &_itemData[index]; }
	uint16 itemDataCount() const { return _numItems; }

	uint16 findBob(uint16 obj) const;
	uint16 findFrame(uint16 obj) const;
	uint16 objectForPerson(uint16 bobnum) const;
	WalkOffData *walkOffPointForObject(int16 obj) const;

	uint16 walkOffCount() const { return _numWalkOffs; }
	WalkOffData *walkOffData(int index) const { return &_walkOffData[index]; }
	uint16 currentRoomData() const { return _roomData[_currentRoom]; }
	GraphicAnim *graphicAnim(int index) const { return &_graphicAnim[index]; }
	uint16 graphicAnimCount() const { return _numGraphicAnim; }
	ObjectDescription *objectDescription(uint16 objNum) const { return &_objectDescription[objNum]; }
	uint16 objectDescriptionCount() const { return _numObjDesc; }
	uint16 currentRoomSfx() const { return _sfxName[_currentRoom]; }

	uint16 joeFacing() const { return _joe.facing; }
	uint16 joeX() const { return _joe.x; }
	uint16 joeY() const { return _joe.y; }
	JoeWalkMode joeWalk() const { return _joe.walk; }
	uint16 joeScale() const { return _joe.scale; }
	uint16 joeCutFacing() const { return _joe.cutFacing; }
	uint16 joePrevFacing() const { return _joe.prevFacing; }

	void joeFacing(uint16 dir) { _joe.facing = dir; }
	void joePos(uint16 x, uint16 y) { _joe.x = x; _joe.y = y; }
	void joeWalk(JoeWalkMode walking);
	void joeScale(uint16 scale) { _joe.scale = scale; }
	void joeCutFacing(uint16 dir) { _joe.cutFacing = dir; }
	void joePrevFacing(uint16 dir) { _joe.prevFacing = dir; }

	int16 gameState(int index) const;
	void gameState(int index, int16 newValue);

	TalkSelected *talkSelected(int index) { return &_talkSelected[index]; }

	const char *roomName(uint16 roomNum) const;
	const char *objectName(uint16 objNum) const;
	const char *objectTextualDescription(uint16 objNum) const;
	const char *joeResponse(int i) const;
	const char *verbName(Verb v) const;

	void eraseRoom();
	void setupRoom(const char *room, int comPanel, bool inCutaway);
	void displayRoom(uint16 room, RoomDisplayMode mode, uint16 joeScale, int comPanel, bool inCutaway);

	int16 entryObj() const { return _entryObj; }
	void entryObj(int16 obj) { _entryObj = obj; }

	ActorData *findActor(uint16 noun, const char *name = NULL) const;
	bool initPerson(uint16 noun, const char *actorName, bool loadBank, Person *pp);
	uint16 findPersonNumber(uint16 obj, uint16 room) const;

	//! load banks used for Joe animation
	void loadJoeBanks(const char *animBank, const char *standBank);

	//! load the various bobs needed to animate Joe
	void setupJoe();

	//! setup Joe at the right place when entering a room
	void setupJoeInRoom(bool autoPosition, uint16 scale);

	uint16 joeFace();
	void joeGrab(int16 grabState);

	//! change Joe clothes to dress
	void joeUseDress(bool showCut);

	//! restore Joe clothes
	void joeUseClothes(bool showCut);

	//! change Joe clothes to underwear
	void joeUseUnderwear();

	void makeJoeSpeak(uint16 descNum, bool objectType = false);
	void makePersonSpeak(const char *sentence, Person *person, const char *voiceFilePrefix);

	//! start the specified dialogue
	void startDialogue(const char *dlgFile, int personInRoom, char *cutaway);

	//! play the specified cutaway
	void playCutaway(const char *cutFile, char *next = NULL);

	//! initialize the inventory
	void inventorySetup();

	//! get the inventory item for the specified inventory slot
	uint16 findInventoryItem(int invSlot) const;

	//! refresh inventory contents
	void inventoryRefresh();
	int16 previousInventoryItem(int16 first) const;
	int16 nextInventoryItem(int16 first) const;
	void removeDuplicateItems();
	uint16 numItemsInventory() const;
	void inventoryInsertItem(uint16 itemNum, bool refresh = true);
	void inventoryDeleteItem(uint16 itemNum, bool refresh = true);
	void inventoryScroll(uint16 count, bool up);
	void removeHotelItemsFromInventory();

	//! copy data from dummy object to object
	void objectCopy(int dummyObjectIndex, int objectIndex);

	//! handle a particular event when Joe walks on this area
	void handleSpecialArea(Direction facing, uint16 areaNum, uint16 walkDataNum);

	//! handle the pinnacle room (== room chooser in the jungle)
	void handlePinnacleRoom();

	void update();

	void saveState(byte *&ptr);
	void loadState(uint32 ver, byte *&ptr);

	//! called after a save state has been loaded
	void setupRestoredGame();

	//! ugly hack from original code
	void sceneReset() { _scene = 0; }

	//! make a scene
	void sceneStart();

	//! stop making a scene
	void sceneStop();

	void changeRoom();

	//! enter the Journal (save/load, configuration)
	virtual void useJournal() = 0;

	//! execute a special move
	void executeSpecialMove(uint16 sm);

	void startCredits(const char *filename);
	void stopCredits();

	void start();

	enum {
		JOE_RESPONSE_MAX    = 40,
		DEFAULT_TALK_SPEED  = 7 * 3,
		GAME_STATE_COUNT    = 211,
		TALK_SELECTED_COUNT = 86
	};

protected:

	void initialise();

	void asmMakeJoeUseDress();
	void asmMakeJoeUseNormalClothes();
	void asmMakeJoeUseUnderwear();
	void asmSwitchToDressPalette();
	void asmSwitchToNormalPalette();
	void asmStartCarAnimation();
	void asmStopCarAnimation();
	void asmStartFightAnimation();
	void asmWaitForFrankPosition();
	void asmMakeFrankGrowing();
	void asmMakeRobotGrowing();
	void asmShrinkRobot();
	void asmEndGame();
	void asmPutCameraOnDino();
	void asmPutCameraOnJoe();
	void asmAltIntroPanRight();
	void asmAltIntroPanLeft();
	void asmSetAzuraInLove();
	void asmPanRightFromJoe();
	void asmSetLightsOff();
	void asmSetLightsOn();
	void asmSetManequinAreaOn();
	void asmPanToJoe();
	void asmTurnGuardOn();
	void asmPanLeft320To144();
	void asmSmooch();
	void asmMakeLightningHitPlane();
	void asmScaleBlimp();
	void asmScaleEnding();
	void asmWaitForCarPosition();
	void asmShakeScreen();
	void asmAttemptPuzzle();
	void asmScaleTitle();
	void asmPanRightToHugh();
	void asmMakeWhiteFlash();
	void asmPanRightToJoeAndRita();
	void asmPanLeftToBomb();
	void asmEndDemo();
	void asmInterviewIntro();
	void asmEndInterview();

	virtual bool preChangeRoom() = 0;
	virtual bool handleSpecialMove(uint16 sm) = 0;


	uint16 _currentRoom;
	uint16 _oldRoom;
	uint16 _newRoom;

	//! total number of room in game
	uint16 _numRooms;

	//! first object number in room
	uint16 *_roomData;

	//! background music to play in room
	uint16 *_sfxName;

	//! bounding box of object
	Box *_objectBox;

	//! inventory items
	ItemData *_itemData;
	uint16 _numItems;

	GraphicData *_graphicData;
	uint16 _numGraphics;

	ObjectData *_objectData;
	uint16 _numObjects;

	ObjectDescription *_objectDescription;
	uint16 _numObjDesc;

	ActorData *_actorData;
	uint16 _numActors;

	//! walk off point for an object
	WalkOffData *_walkOffData;
	uint16 _numWalkOffs;

	FurnitureData *_furnitureData;
	uint16 _numFurniture;

	GraphicAnim *_graphicAnim;
	uint16 _numGraphicAnim;

	//! actor initial position in room is _walkOffData[_entryObj]
	int16 _entryObj;

	//! object description (Look At)
	Common::StringList _objDescription;
	uint16 _numDescriptions;

	Common::StringList _objName;
	uint16 _numNames;

	//! room name, prefix for data files (PCX, LUM...)
	Common::StringList _roomName;

	Common::StringList _verbName;

	Common::StringList _joeResponse;

	//! actor animation strings
	Common::StringList _aAnim;
	uint16 _numAAnim;

	//! actor names
	Common::StringList _aName;
	uint16 _numAName;

	//! actor filenames
	Common::StringList _aFile;
	uint16 _numAFile;

	struct {
		uint16 x, y;
		uint16 facing, cutFacing, prevFacing;
		JoeWalkMode walk;
		uint16 scale;
	} _joe;

	int16 _gameState[GAME_STATE_COUNT];

	TalkSelected _talkSelected[TALK_SELECTED_COUNT];

	//! inventory items
	int16 _inventoryItem[4];

	//! puzzle counter for room T7
	uint8 _puzzleAttemptCount;

	//! cutscene counter
	int _scene;

	Credits *_credits;
	Journal *_journal;

	QueenEngine *_vm;
};

class LogicDemo : public Logic {
public:

	LogicDemo(QueenEngine *vm) : Logic(vm) {}
	void useJournal();

protected:

	bool preChangeRoom();
	bool handleSpecialMove(uint16 sm);
};

class LogicInterview : public Logic {
public:

	LogicInterview(QueenEngine *vm) : Logic(vm) {}
	void useJournal();

protected:

	bool preChangeRoom();
	bool handleSpecialMove(uint16 sm);
};

class LogicGame : public Logic {
public:

	LogicGame(QueenEngine *vm) : Logic(vm) {}
	void useJournal();

protected:

	bool preChangeRoom();
	bool handleSpecialMove(uint16 sm);
};


} // End of namespace Queen

#endif
