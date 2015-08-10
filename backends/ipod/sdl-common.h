/* ScummVM - Scumm Interpreter
 * Copyright (C) 2001  Ludvig Strigeus
 * Copyright (C) 2001-2006 The ScummVM project
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

#ifndef SDL_COMMON_H
#define SDL_COMMON_H

#include <SDL.h>

#include "common/stdafx.h"
#include "common/scummsys.h"
#include "common/system.h"
#include "graphics/scaler.h"
#include "backends/intern.h"

// AUDIO_QUALITY vs. "SYNCHRO"
#define AUDIO_QUALITY 1 // good audio quality with much latency

//#define FSKIP 1 //skips 5 frames - look ar graphics.cpp for internUpdateScreen()
//#define FRAMES_SKIPPED 10
//#define USE_COP 1

// Uncomment this to enable the 'on screen display' code.
#define USE_OSD	1

//#define DISABLE_SCALERS 1
#define DISABLE_HQ_SCALERS 1




enum remapKeyResult { NO_REMAP, EVENT_COMPLETED, IGNORE_KEYPRESS };

enum {
	GFX_NORMAL = 0,
	GFX_HALF = 1,
	GFX_DOUBLESIZE = 2,
	GFX_TRIPLESIZE = 3,
	GFX_2XSAI = 4,
	GFX_SUPER2XSAI = 5,
	GFX_SUPEREAGLE = 6,
	GFX_ADVMAME2X = 7,
	GFX_ADVMAME3X = 8,
	GFX_HQ2X = 9,
	GFX_HQ3X = 10,
	GFX_TV2X = 11,
	GFX_DOTMATRIX = 12
};


//enum { GFX_NORMAL, GFX_HALF }; 

class OSystem_IPOD : public OSystem {
public:
	OSystem_IPOD();
	virtual ~OSystem_IPOD();

	virtual void initBackend();

	void beginGFXTransaction(void);
	void endGFXTransaction(void);

	// Set the size of the video bitmap.
	// Typically, 320x200
	virtual void initSize(uint w, uint h); // overloaded by CE backend

	// Set colors of the palette
	void setPalette(const byte *colors, uint start, uint num);

	// Get colors of the palette
	void grabPalette(byte *colors, uint start, uint num);

	// Draw a bitmap to screen.
	// The screen will not be updated to reflect the new bitmap
	virtual void copyRectToScreen(const byte *src, int pitch, int x, int y, int w, int h); // overloaded by CE backend (FIXME)

	// Copies the screen to a buffer
	bool grabRawScreen(Graphics::Surface *surf);

	// Clear the screen
	void clearScreen();

	// Update the dirty areas of the screen
	//void updateScreen();

	// Either show or hide the mouse cursor
	bool showMouse(bool visible);

	// Warp the mouse cursor. Where set_mouse_pos() only informs the
	// backend of the mouse cursor's current position, this function
	// actually moves the cursor to the specified position.
	virtual void warpMouse(int x, int y); // overloaded by CE backend (FIXME)

	// Set the bitmap that's used when drawing the cursor.
	virtual void setMouseCursor(const byte *buf, uint w, uint h, int hotspot_x, int hotspot_y, byte keycolor, int cursorTargetScale); // overloaded by CE backend (FIXME)

	// Set colors of cursor palette
	void setCursorPalette(const byte *colors, uint start, uint num);

	// Disables or enables cursor palette
	void disableCursorPalette(bool disable) {
		_cursorPaletteDisabled = disable;
		blitCursor();
	};

	// Shaking is used in SCUMM. Set current shake position.
	void setShakePos(int shake_pos);

	// Get the number of milliseconds since the program was started.
	uint32 getMillis();

	// Delay for a specified amount of milliseconds
	void delayMillis(uint msecs);

	// Get the next event.
	// Returns true if an event was retrieved.
	virtual bool pollEvent(Event &event); // overloaded by CE backend

	// Set function that generates samples
	virtual bool setSoundCallback(SoundProc proc, void *param); // overloaded by CE backend

	void clearSoundCallback();

	// Poll CD status
	// Returns true if cd audio is playing
	bool pollCD();

	// Play CD audio track
	void playCD(int track, int num_loops, int start_frame, int duration);

	// Stop CD audio track
	void stopCD();

	// Update CD audio status
	void updateCD();

	// Quit
	virtual void quit(); // overloaded by CE backend


	// Add a callback timer
	void setTimerCallback(TimerProc callback, int timer);

	// Mutex handling
	MutexRef createMutex();
	void lockMutex(MutexRef mutex);
	void unlockMutex(MutexRef mutex);
	void deleteMutex(MutexRef mutex);

	// Overlay
	virtual void showOverlay(); // WinCE FIXME
	virtual void hideOverlay(); // WinCE FIXME
	virtual void clearOverlay();
	virtual void grabOverlay(OverlayColor *buf, int pitch);
	virtual void copyRectToOverlay(const OverlayColor *buf, int pitch, int x, int y, int w, int h); // WinCE FIXME
	virtual int16 getHeight();
	virtual int16 getWidth();
	virtual int16 getOverlayHeight()  { return _overlayHeight; }
	virtual int16 getOverlayWidth()   { return _overlayWidth; }

	// Methods that convert RGB to/from colors suitable for the overlay.
	virtual OverlayColor RGBToColor(uint8 r, uint8 g, uint8 b);
	virtual void colorToRGB(OverlayColor color, uint8 &r, uint8 &g, uint8 &b);


	virtual const GraphicsMode *getSupportedGraphicsModes() const;
	virtual int getDefaultGraphicsMode() const;
	virtual bool setGraphicsMode(int mode);
	virtual int getGraphicsMode() const;

	virtual void setWindowCaption(const char *caption);
	virtual bool openCD(int drive);
	virtual int getOutputSampleRate() const;

	virtual bool hasFeature(Feature f);
	virtual void setFeatureState(Feature f, bool enable);
	virtual bool getFeatureState(Feature f);

#ifdef USE_OSD
	void displayMessageOnOSD(const char *msg);
#endif
	// Update the dirty areas of the screen
	void updateScreen();
	void internUpdateScreen_Real();
	
	int _rotation_modulation;
	
	//int _ipod_model;
	//bool _audio_quality;
	
	int _hwScreenOffsetX;
	int _hwScreenOffsetY;
	
	/*
	#ifdef USE_COP
	void ipod_init_cop();
	#endif
	*/
protected:
	bool _inited;

	int _frameskip;
//
#ifdef USE_OSD
	SDL_Surface *_osdSurface;
	Uint8 _osdAlpha;			// Transparency level of the OSD
	uint32 _osdFadeStartTime;	// When to start the fade out
	enum {
		kOSDFadeOutDelay = 2 * 1000,	// Delay before the OSD is faded out (in milliseconds)
		kOSDFadeOutDuration = 500,		// Duration of the OSD fade out (in milliseconds)
		kOSDColorKey = 1,
		kOSDInitialAlpha = 80			// Initial alpha level, in percent
	};
#endif

	// hardware screen
	SDL_Surface *_hwscreen;

	// unseen game screen
	SDL_Surface *_screen;
	int _screenWidth, _screenHeight;

	// temporary screen (for scalers)
	SDL_Surface *_tmpscreen;
	SDL_Surface *_tmpscreen2;

	// overlay
	SDL_Surface *_overlayscreen;
	int _overlayWidth, _overlayHeight;
	bool _overlayVisible;

	// Audio
	int _samplesPerSec;
	pthread_t _sound_thread;
	
	// CD Audio
	SDL_CD *_cdrom;
	int _cdTrack, _cdNumLoops, _cdStartFrame, _cdDuration;
	uint32 _cdEndTime, _cdStopTime;

	enum {
		DF_WANT_RECT_OPTIM			= 1 << 0,
		DF_UPDATE_EXPAND_1_PIXEL	= 1 << 1
	};

	enum {
		kTransactionNone = 0,
		kTransactionCommit = 1,
		kTransactionActive = 2
	};

	struct TransactionDetails {
		int mode;
		bool modeChanged;
		int w;
		int h;
		bool sizeChanged;
		bool fs;
		bool fsChanged;
		bool ar;
		bool arChanged;
		bool needHotswap;
		bool needUpdatescreen;
		bool needUnload;
		bool needToggle;
		bool normal1xScaler;
	};
	TransactionDetails _transactionDetails;

	struct VideoState {
		bool setup;

		bool fullscreen;
		bool aspectRatio;

		int mode;
		double scaleFactor;

		int screenWidth, screenHeight;
		int overlayWidth, overlayHeight;
	};
	VideoState _videoMode, _oldVideoMode;

	void setGraphicsModeIntern();
	
	/** Force full redraw on next updateScreen */
	bool _forceFull;
	ScalerProc *_scalerProc;
	int _scalerType;
	//int _scaleFactor;
	double _scaleFactor;
	int _mode;
	int _transactionMode;
	bool _fullscreen;

	/** Current video mode flags (see DF_* constants) */
	uint32 _modeFlags;
	bool _modeChanged;

	/** True if aspect ratio correction is enabled. */
	bool _adjustAspectRatio;

	enum {
		NUM_DIRTY_RECT = 100,

		MAX_MOUSE_W = 80,
		MAX_MOUSE_H = 80,
		MAX_SCALING = 3
	};

	// Dirty rect management
	SDL_Rect _dirtyRectList[NUM_DIRTY_RECT];
	int _numDirtyRects;
	uint32 *_dirtyChecksums;
	bool _cksumValid;
	int _cksumNum;

	// Keyboard mouse emulation.  Disabled by fingolfin 2004-12-18.
	// I am keeping the rest of the code in for now, since the joystick
	// code (or rather, "hack") uses it, too.
	struct KbdMouse {
		int16 x, y, x_vel, y_vel, x_max, y_max, x_down_count, y_down_count;
		uint32 last_time, delay_time, x_down_time, y_down_time;
	};

	struct MousePos {
		// The mouse position, using either virtual (game) or real
		// (overlay) coordinates.
		int16 x, y;

		// The size and hotspot of the original cursor image.
	    	int16 w, h;
		int16 hotX, hotY;

		// The size and hotspot of the pre-scaled cursor image, in real
		// coordinates.
		int16 rW, rH;
		int16 rHotX, rHotY;

		// The size and hotspot of the pre-scaled cursor image, in game
		// coordinates.
		int16 vW, vH;
		int16 vHotX, vHotY;

		MousePos() : x(0), y(0), w(0), h(0), hotX(0), hotY(0),
		             rW(0), rH(0), rHotX(0), rHotY(0), vW(0), vH(0),
		             vHotX(0), vHotY(0)
			{ }
	};

	// mouse
	KbdMouse _km;
	bool _mouseVisible;
	bool _mouseDrawn;
	byte *_mouseData;
	SDL_Rect _mouseBackup;
	MousePos _mouseCurState;
	byte _mouseKeyColor;
	int _cursorTargetScale;
	bool _cursorPaletteDisabled;
	SDL_Surface *_mouseOrigSurface;
	SDL_Surface *_mouseSurface;
	enum {
		kMouseColorKey = 1
	};

	// joystick
	SDL_Joystick *_joystick;

	// Shake mode
	int _currentShakePos;
	int _newShakePos;

	// Palette data
	SDL_Color *_currentPalette;
	uint _paletteDirtyStart, _paletteDirtyEnd;

	// Cursor palette data
	SDL_Color *_cursorPalette;

	/**
	 * Mutex which prevents multiple threads from interfering with each other
	 * when accessing the screen.
	 */
	MutexRef _graphicsMutex;


	void addDirtyRgnAuto(const byte *buf);
	void makeChecksums(const byte *buf);

	virtual void addDirtyRect(int x, int y, int w, int h, bool realCoordinates = false); // overloaded by CE backend

	virtual void drawMouse(); // overloaded by CE backend
	virtual void undrawMouse(); // overloaded by CE backend (FIXME)
	virtual void blitCursor(); // overloaded by CE backend (FIXME)
 
	/** Set the position of the virtual mouse cursor. */
	void setMousePos(int x, int y);
	virtual void fillMouseEvent(Event &event, int x, int y); // overloaded by CE backend
	void toggleMouseGrab();

	virtual void internUpdateScreen(); // overloaded by CE backend
	

	virtual void loadGFXMode(); // overloaded by CE backend
	virtual void unloadGFXMode(); // overloaded by CE backend
	virtual void hotswapGFXMode(); // overloaded by CE backend

	void setFullscreenMode(bool enable);
	void setAspectRatioCorrection(bool enable);

	virtual bool saveScreenshot(const char *filename); // overloaded by CE backend

	int effectiveScreenHeight() const { return (_adjustAspectRatio ? 240 : _screenHeight) * _scaleFactor; }

	void setupIcon();
	void handleKbdMouse();

	virtual bool remapKey(SDL_Event &ev, Event &event);

	void handleScalerHotkeys(const SDL_KeyboardEvent &key);
};

void Scaleo5x(const uint8 *srcPtr, uint32 srcPitch, uint8 *dstPtr, uint32 dstPitch, int width, int height);

#ifdef USE_COP
	void ipod_init_cop(OSystem_IPOD *ipod_g_system);
	void ipod_cop_update_screen();
#endif

#endif
typedef struct {
	OSystem::SoundProc sound_proc;
	void *param;
} THREAD_PARAM;
