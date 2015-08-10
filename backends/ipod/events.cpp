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

#include "backends/ipod/sdl-common.h"
#include "common/util.h"

// FIXME move joystick defines out and replace with confile file options
// we should really allow users to map any key to a joystick button
#define JOY_DEADZONE 3200

// #define JOY_INVERT_Y
#define JOY_XAXIS 0
#define JOY_YAXIS 1
// buttons
#define JOY_BUT_LMOUSE 0
#define JOY_BUT_RMOUSE 2
#define JOY_BUT_ESCAPE 3
#define JOY_BUT_PERIOD 1
#define JOY_BUT_SPACE 4
#define JOY_BUT_F5 5




static int mapKey(SDLKey key, SDLMod mod, Uint16 unicode)
{
	if (key >= SDLK_F1 && key <= SDLK_F9) {
		return key - SDLK_F1 + 315;
	} else if (key >= SDLK_KP0 && key <= SDLK_KP9) {
		return key - SDLK_KP0 + '0';
	} else if (key >= SDLK_UP && key <= SDLK_PAGEDOWN) {
		return key;
	} else if (unicode) {
		return unicode;
	} else if (key >= 'a' && key <= 'z' && mod & KMOD_SHIFT) {
		return key & ~0x20;
	} else if (key >= SDLK_NUMLOCK && key <= SDLK_EURO) {
		return 0;
	}
	return key;
}

void OSystem_IPOD::fillMouseEvent(Event &event, int x, int y) {
	event.mouse.x = x;
	event.mouse.y = y;

	// Update the "keyboard mouse" coords
	_km.x = x;
	_km.y = y;

	// Adjust for the screen scaling
	if (!_overlayVisible) {
		event.mouse.x /= _scaleFactor;
		event.mouse.y /= _scaleFactor;
		if (_adjustAspectRatio)
			event.mouse.y = aspect2Real(event.mouse.y);
	}
}

void OSystem_IPOD::handleKbdMouse() {
	uint32 curTime = getMillis();
	if (curTime >= _km.last_time + _km.delay_time) {
		_km.last_time = curTime;
		if (_km.x_down_count == 1) {
			_km.x_down_time = curTime;
			_km.x_down_count = 2;
		}
		if (_km.y_down_count == 1) {
			_km.y_down_time = curTime;
			_km.y_down_count = 2;
		}

		if (_km.x_vel || _km.y_vel) {
			if (_km.x_down_count) {
				if (curTime > _km.x_down_time + _km.delay_time * 12) {
					if (_km.x_vel > 0)
						_km.x_vel++;
					else
						_km.x_vel--;
				} else if (curTime > _km.x_down_time + _km.delay_time * 8) {
					if (_km.x_vel > 0)
						_km.x_vel = 5;
					else
						_km.x_vel = -5;
				}
			}
			if (_km.y_down_count) {
				if (curTime > _km.y_down_time + _km.delay_time * 12) {
					if (_km.y_vel > 0)
						_km.y_vel++;
					else
						_km.y_vel--;
				} else if (curTime > _km.y_down_time + _km.delay_time * 8) {
					if (_km.y_vel > 0)
						_km.y_vel = 5;
					else
						_km.y_vel = -5;
				}
			}

			_km.x += _km.x_vel;
			_km.y += _km.y_vel;

			if (_km.x < 0) {
				_km.x = 0;
				_km.x_vel = -1;
				_km.x_down_count = 1;
			} else if (_km.x > _km.x_max) {
				_km.x = _km.x_max;
				_km.x_vel = 1;
				_km.x_down_count = 1;
			}

			if (_km.y < 0) {
				_km.y = 0;
				_km.y_vel = -1;
				_km.y_down_count = 1;
			} else if (_km.y > _km.y_max) {
				_km.y = _km.y_max;
				_km.y_vel = 1;
				_km.y_down_count = 1;
			}

			SDL_WarpMouse(_km.x, _km.y);
		}
	}
}

static byte SDLModToOSystemKeyFlags(SDLMod mod) {
	byte b = 0;
#ifdef LINUPY
	// Yopy has no ALT key, steal the SHIFT key
	// (which isn't used much anyway)
	if (mod & KMOD_SHIFT)
		b |= OSystem::KBD_ALT;
#else
	if (mod & KMOD_SHIFT)
		b |= OSystem::KBD_SHIFT;
	if (mod & KMOD_ALT)
		b |= OSystem::KBD_ALT;
#endif
	if (mod & KMOD_CTRL)
		b |= OSystem::KBD_CTRL;

	return b;
}

	#define TOUCH_U     0 // North
	#define TOUCH_UR    1 // North-East
	#define TOUCH_R     2 // East
	#define TOUCH_DR    3 // South-East
	#define TOUCH_D     4 // South
	#define TOUCH_DL    5 // South-West
	#define TOUCH_L     6 // West
	#define TOUCH_UL    7 // North-West
	#define TOUCH_NULL -1 // No wheel touch event

	#define outl(a, b) (*(volatile unsigned int *)(b) = (a))
	#define inl(a) (*(volatile unsigned int *)(a))

	static int ipod_get_keytouch()
	{
	
	    int touch;
	 
	    touch = 0xff;
		int in, st;
		in = inl(0x7000C140);
		st = ((in & 0xff000000) >> 24);
	 
		touch = 0xff;
		if (st == 0xc0)
		    touch = (in & 0x007F0000 ) >> 16;
	 
	    // See http://ipodlinux.org/wiki/Key_Chart
	    // The +6 is for rounding
	    if (touch != 0xff) {
		touch += 6;
		touch /= 12;
		if(touch > 7)
		    touch = 0;
			
		return touch;
		
	    } else {
		    
		return -1;

	    }
	}
	
bool PressingButton = false;

	
bool OSystem_IPOD::pollEvent(Event &event) {
	SDL_Event ev;
	int axis;
	byte b = 0;

	handleKbdMouse();
	
	if(!PressingButton)
	{
		int ipod_velocity = 5;
	
		if(ev.key.keysym.sym != SDLK_RETURN)
		{
		    int input;
		    input = ipod_get_keytouch();
		    switch(input) {
			case TOUCH_NULL:
				event.type = EVENT_MOUSEMOVE;
				_km.x_vel = 0;
				_km.x_down_count = 0;
				_km.y_vel = 0;
				_km.y_down_count = 0;
				fillMouseEvent(event, _km.x, _km.y);
			    break;
			case TOUCH_U:
				event.type = EVENT_MOUSEMOVE;
				_km.x_vel = 0;
				_km.x_down_count = 0;
				_km.y_vel = -ipod_velocity;
				_km.y_down_count = 1;
				fillMouseEvent(event, _km.x, _km.y);
			    // Up
			    break;
			case TOUCH_UR:
				event.type = EVENT_MOUSEMOVE;
				_km.x_vel = ipod_velocity;
				_km.x_down_count = 1;
				_km.y_vel = -ipod_velocity;
				_km.y_down_count = 1;
				fillMouseEvent(event, _km.x, _km.y);
			    // Up right
			    break;
			case TOUCH_R:
				event.type = EVENT_MOUSEMOVE;
				_km.x_vel = ipod_velocity;
				_km.x_down_count = 1;
				_km.y_vel = 0;
				_km.y_down_count = 0;
				fillMouseEvent(event, _km.x, _km.y);
			    // Right
			    break;
			case TOUCH_DR:
				event.type = EVENT_MOUSEMOVE;
				_km.x_vel = ipod_velocity;
				_km.x_down_count = 1;
				_km.y_vel = ipod_velocity;
				_km.y_down_count = 1;
				fillMouseEvent(event, _km.x, _km.y);
			    // Down right
			    break;
			case TOUCH_D:
				event.type = EVENT_MOUSEMOVE;
				_km.x_vel = 0;
				_km.x_down_count = 0;
				_km.y_vel = ipod_velocity;
				_km.y_down_count = 1;
				fillMouseEvent(event, _km.x, _km.y);
			    // Down
			    break;
			case TOUCH_DL:
				event.type = EVENT_MOUSEMOVE;
				_km.x_vel = -ipod_velocity;
				_km.x_down_count = 1;
				_km.y_vel = ipod_velocity;
				_km.y_down_count = 1;
				fillMouseEvent(event, _km.x, _km.y);
			    // Down left
			    break;
			case TOUCH_L:
				event.type = EVENT_MOUSEMOVE;
				_km.x_vel = -ipod_velocity;
				_km.x_down_count = 1;
				_km.y_vel = 0;
				_km.y_down_count = 0;
				fillMouseEvent(event, _km.x, _km.y);
			    // Left
			    break;
			case TOUCH_UL:
				event.type = EVENT_MOUSEMOVE;
				_km.x_vel = -ipod_velocity;
				_km.x_down_count = 1;
				_km.y_vel = -ipod_velocity;
				_km.y_down_count = 1;
				fillMouseEvent(event, _km.x, _km.y);
			    // Up left
			    break;
			default:
			    break;
		    }

		}
	}
	// If the screen mode changed, send an EVENT_SCREEN_CHANGED
	if (_modeChanged) {
		_modeChanged = false;
		event.type = EVENT_SCREEN_CHANGED;
		return true;
	}
	
	

	while(SDL_PollEvent(&ev)) {
		
		switch(ev.type) {
		case SDL_KEYDOWN:{
			b = event.kbd.flags = SDLModToOSystemKeyFlags(SDL_GetModState());

			// Alt-Return and Alt-Enter toggle full screen mode
			if (b == KBD_ALT && (ev.key.keysym.sym == SDLK_RETURN
			                  || ev.key.keysym.sym == SDLK_KP_ENTER)) {
				setFullscreenMode(!_fullscreen);
#ifdef USE_OSD
				if (_fullscreen)
					displayMessageOnOSD("Fullscreen mode");
				else
					displayMessageOnOSD("Windowed mode");
#endif

				break;
			}

			// Alt-S: Create a screenshot
			if (b == KBD_ALT && ev.key.keysym.sym == 's') {
				char filename[20];

				for (int n = 0;; n++) {
					SDL_RWops *file;

					sprintf(filename, "scummvm%05d.bmp", n);
					file = SDL_RWFromFile(filename, "r");
					if (!file)
						break;
					SDL_RWclose(file);
				}
				if (saveScreenshot(filename))
					printf("Saved '%s'\n", filename);
				else
					printf("Could not save screenshot!\n");
				break;
			}

			// Ctrl-m toggles mouse capture
			if (b == KBD_CTRL && ev.key.keysym.sym == 'm') {
				toggleMouseGrab();
				break;
			}

#ifdef MACOSX
			// On Macintosh', Cmd-Q quits
			if ((ev.key.keysym.mod & KMOD_META) && ev.key.keysym.sym == 'q') {
				event.type = EVENT_QUIT;
				return true;
			}
#elif defined(UNIX)
			// On other unices, Control-Q quits
			if ((ev.key.keysym.mod & KMOD_CTRL) && ev.key.keysym.sym == 'q') {
				event.type = EVENT_QUIT;
				return true;
			}
#else
			// Ctrl-z and Alt-X quit
			if ((b == KBD_CTRL && ev.key.keysym.sym == 'z') || (b == KBD_ALT && ev.key.keysym.sym == 'x')) {
				event.type = EVENT_QUIT;
				return true;
			}
#endif

			// Ctrl-Alt-<key> will change the GFX mode
			if ((b & (KBD_CTRL|KBD_ALT)) == (KBD_CTRL|KBD_ALT)) {

				handleScalerHotkeys(ev.key);
				break;
			}
			const bool event_complete = remapKey(ev,event);

			if (event_complete)
				return true;

			event.type = EVENT_KEYDOWN;
			event.kbd.keycode = ev.key.keysym.sym;
			event.kbd.ascii = mapKey(ev.key.keysym.sym, ev.key.keysym.mod, ev.key.keysym.unicode);

			return true;
			}
		case SDL_KEYUP:
			{
			const bool event_complete = remapKey(ev,event);

			if (event_complete)
				return true;

			event.type = EVENT_KEYUP;
			event.kbd.keycode = ev.key.keysym.sym;
			event.kbd.ascii = mapKey(ev.key.keysym.sym, ev.key.keysym.mod, ev.key.keysym.unicode);
			b = event.kbd.flags = SDLModToOSystemKeyFlags(SDL_GetModState());

			// Ctrl-Alt-<key> will change the GFX mode
			if ((b & (KBD_CTRL|KBD_ALT)) == (KBD_CTRL|KBD_ALT)) {
				// Swallow these key up events
				break;
			}

			return true;
			}
		case SDL_MOUSEMOTION:
			event.type = EVENT_MOUSEMOVE;
			fillMouseEvent(event, ev.motion.x, ev.motion.y);

			setMousePos(event.mouse.x, event.mouse.y);
			return true;

		case SDL_MOUSEBUTTONDOWN:
			if (ev.button.button == SDL_BUTTON_LEFT)
				event.type = EVENT_LBUTTONDOWN;
			else if (ev.button.button == SDL_BUTTON_RIGHT)
				event.type = EVENT_RBUTTONDOWN;
#if defined(SDL_BUTTON_WHEELUP) && defined(SDL_BUTTON_WHEELDOWN)
			else if (ev.button.button == SDL_BUTTON_WHEELUP)
				event.type = EVENT_WHEELUP;
			else if (ev.button.button == SDL_BUTTON_WHEELDOWN)
				event.type = EVENT_WHEELDOWN;
#endif
			else
				break;

			fillMouseEvent(event, ev.button.x, ev.button.y);

			return true;

		case SDL_MOUSEBUTTONUP:
			if (ev.button.button == SDL_BUTTON_LEFT)
				event.type = EVENT_LBUTTONUP;
			else if (ev.button.button == SDL_BUTTON_RIGHT)
				event.type = EVENT_RBUTTONUP;
			else
				break;
			fillMouseEvent(event, ev.button.x, ev.button.y);

			return true;

		case SDL_JOYBUTTONDOWN:
			if (ev.jbutton.button == JOY_BUT_LMOUSE) {
				event.type = EVENT_LBUTTONDOWN;
				fillMouseEvent(event, _km.x, _km.y);
			} else if (ev.jbutton.button == JOY_BUT_RMOUSE) {
				event.type = EVENT_RBUTTONDOWN;
				fillMouseEvent(event, _km.x, _km.y);
			} else {
				event.type = EVENT_KEYDOWN;
				switch (ev.jbutton.button) {
					case JOY_BUT_ESCAPE:
						event.kbd.keycode = SDLK_ESCAPE;
						event.kbd.ascii = mapKey(SDLK_ESCAPE, ev.key.keysym.mod, 0);
						break;
					case JOY_BUT_PERIOD:
						event.kbd.keycode = SDLK_PERIOD;
						event.kbd.ascii = mapKey(SDLK_PERIOD, ev.key.keysym.mod, 0);
						break;
					case JOY_BUT_SPACE:
						event.kbd.keycode = SDLK_SPACE;
						event.kbd.ascii = mapKey(SDLK_SPACE, ev.key.keysym.mod, 0);
						break;
					case JOY_BUT_F5:
						event.kbd.keycode = SDLK_F5;
						event.kbd.ascii = mapKey(SDLK_F5, ev.key.keysym.mod, 0);
						break;
				}
			}
			return true;

		case SDL_JOYBUTTONUP:
			if (ev.jbutton.button == JOY_BUT_LMOUSE) {
				event.type = EVENT_LBUTTONUP;
				fillMouseEvent(event, _km.x, _km.y);
			} else if (ev.jbutton.button == JOY_BUT_RMOUSE) {
				event.type = EVENT_RBUTTONUP;
				fillMouseEvent(event, _km.x, _km.y);
			} else {
				event.type = EVENT_KEYUP;
				switch (ev.jbutton.button) {
					case JOY_BUT_ESCAPE:
						event.kbd.keycode = SDLK_ESCAPE;
						event.kbd.ascii = mapKey(SDLK_ESCAPE, ev.key.keysym.mod, 0);
						break;
					case JOY_BUT_PERIOD:
						event.kbd.keycode = SDLK_PERIOD;
						event.kbd.ascii = mapKey(SDLK_PERIOD, ev.key.keysym.mod, 0);
						break;
					case JOY_BUT_SPACE:
						event.kbd.keycode = SDLK_SPACE;
						event.kbd.ascii = mapKey(SDLK_SPACE, ev.key.keysym.mod, 0);
						break;
					case JOY_BUT_F5:
						event.kbd.keycode = SDLK_F5;
						event.kbd.ascii = mapKey(SDLK_F5, ev.key.keysym.mod, 0);
						break;
				}
			}
			return true;

		case SDL_JOYAXISMOTION:
			axis = ev.jaxis.value;
			if ( axis > JOY_DEADZONE) {
				axis -= JOY_DEADZONE;
				event.type = EVENT_MOUSEMOVE;
			} else if ( axis < -JOY_DEADZONE ) {
				axis += JOY_DEADZONE;
				event.type = EVENT_MOUSEMOVE;
			} else
				axis = 0;

			if ( ev.jaxis.axis == JOY_XAXIS) {
#ifdef JOY_ANALOG
				_km.x_vel = axis/2000;
				_km.x_down_count = 0;
#else
				if (axis != 0) {
					_km.x_vel = (axis > 0) ? 1:-1;
					_km.x_down_count = 1;
				} else {
					_km.x_vel = 0;
					_km.x_down_count = 0;
				}
#endif

			} else if (ev.jaxis.axis == JOY_YAXIS) {
#ifndef JOY_INVERT_Y
				axis = -axis;
#endif
#ifdef JOY_ANALOG
				_km.y_vel = -axis / 2000;
				_km.y_down_count = 0;
#else
				if (axis != 0) {
					_km.y_vel = (-axis > 0) ? 1: -1;
					_km.y_down_count = 1;
				} else {
					_km.y_vel = 0;
					_km.y_down_count = 0;
				}
#endif
			}

			fillMouseEvent(event, _km.x, _km.y);

			return true;

		case SDL_VIDEOEXPOSE:
			_forceFull = true;
			break;

		case SDL_QUIT:
			event.type = EVENT_QUIT;
			return true;
		}
	}
	return false;
}

static int rotate_modulation= 0;

bool OSystem_IPOD::remapKey(SDL_Event &ev,Event &event) {


	if (ev.key.keysym.sym == SDLK_RETURN) {

		if (ev.type == SDL_KEYDOWN)
                {
			event.type = EVENT_LBUTTONDOWN;
			PressingButton = true;
		}
                if (ev.type == SDL_KEYUP)
		{
			event.type = EVENT_LBUTTONUP;
			PressingButton = false;
		}
		_km.x_vel = 0;
		_km.x_down_count = 0;
		_km.y_vel = 0;
		_km.y_down_count = 0;
                fillMouseEvent(event, _km.x, _km.y);
                return EVENT_COMPLETED;
	}
	
        if ((ev.key.keysym.sym == SDLK_m)) {
               event.type = EVENT_KEYDOWN;
		event.kbd.keycode = SDLK_F5;
		event.kbd.ascii = mapKey(SDLK_F5, ev.key.keysym.mod, 0);
                return EVENT_COMPLETED;
        }

        if ((ev.key.keysym.sym == SDLK_f)) {

		if (ev.type == SDL_KEYDOWN)
                {
			event.type = EVENT_RBUTTONDOWN;
			PressingButton = true;
		}
                if (ev.type == SDL_KEYUP)
		{
			event.type = EVENT_RBUTTONUP;
			PressingButton = false;
		}
		_km.x_vel = 0;
		_km.x_down_count = 0;
		_km.y_vel = 0;
		_km.y_down_count = 0;
                fillMouseEvent(event, _km.x, _km.y);
                return EVENT_COMPLETED;
        }


        if ((ev.key.keysym.sym == SDLK_w)) {
		if (ev.type == SDL_KEYDOWN)
                {
			event.type = EVENT_RBUTTONDOWN;
			PressingButton = true;

		}
                if (ev.type == SDL_KEYUP)
		{
			event.type = EVENT_RBUTTONUP;
			PressingButton = false;
		}
		_km.x_vel = 0;
		_km.x_down_count = 0;
		_km.y_vel = 0;
		_km.y_down_count = 0;
                fillMouseEvent(event, _km.x, _km.y);
                return EVENT_COMPLETED;
		
        }

        if ((ev.key.keysym.sym == SDLK_d)) {
		if (ev.type == SDL_KEYDOWN)
                {
			event.type = EVENT_KEYDOWN;
			event.kbd.keycode = SDLK_6;
			event.kbd.ascii = mapKey(SDLK_6, ev.key.keysym.mod, 0);
			return EVENT_COMPLETED;
		}
		if (ev.type == SDL_KEYUP)
                {
			event.type = EVENT_KEYUP;
			event.kbd.keycode = SDLK_6;
			event.kbd.ascii = mapKey(SDLK_6, ev.key.keysym.mod, 0);
			return EVENT_COMPLETED;
		}
	}
	
	if ((ev.key.keysym.sym == SDLK_r)) {
			return IGNORE_KEYPRESS;
        }
	
	if ((ev.key.keysym.sym == SDLK_l)) {
                return IGNORE_KEYPRESS;
        }
	
	if (ev.key.keysym.sym == SDLK_h) {

		if (ev.type == SDL_KEYDOWN)
                {
			event.type = EVENT_KEYDOWN;
			event.kbd.keycode = SDLK_ESCAPE;
			event.kbd.ascii = mapKey(SDLK_ESCAPE, ev.key.keysym.mod, 0);
			return EVENT_COMPLETED;
		}
		if (ev.type == SDL_KEYUP)
                {
			event.type = EVENT_KEYUP;
			event.kbd.keycode = SDLK_ESCAPE;
			event.kbd.ascii = mapKey(SDLK_ESCAPE, ev.key.keysym.mod, 0);
			return EVENT_COMPLETED;
		}
        }
	
	
        return NO_REMAP;


}



