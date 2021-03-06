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
#include "common/config-manager.h"
#include "common/util.h"
#include "base/main.h"

#include <stdio.h>
#include <assert.h>

#include <sys/time.h>
#include <unistd.h>

#include <sys/soundcard.h>

#include <sched.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "icons/scummvm.xpm"
#define DEFAULT_CONFIG_FILE ".scummvmrc"

bool AudioInit = true;

int ipod_model = 0;
bool audio_quality = true;
	
	
#if !defined(_WIN32_WCE) && !defined(__MAEMO__)

int main(int argc, char *argv[]) {

	// Create our OSystem instance

	g_system = new OSystem_IPOD();

	assert(g_system);
	
	// Invoke the actual ScummVM main entry point:
	int res = scummvm_main(argc, argv);
	g_system->quit();	// TODO: Consider removing / replacing this!
	return res;
}
#endif 
 
	
void OSystem_IPOD::initBackend() {
	assert(!_inited);
	
	
	ConfMan.setBool("FM_medium_quality", true);
	
	//pod_init_cop();
	//int joystick_num =0;
	uint32 sdlFlags = SDL_INIT_VIDEO /* | SDL_INIT_AUDIO */ | SDL_INIT_TIMER;

	if (ConfMan.hasKey("disable_sdl_parachute"))
		sdlFlags |= SDL_INIT_NOPARACHUTE;


	if (SDL_Init(sdlFlags) == -1) {
		error("Could not initialize SDL: %s", SDL_GetError());
	}

	if (ConfMan.hasKey("rotation_modulation"))
		_rotation_modulation = ConfMan.getInt("rotation_modulation");
	else
		_rotation_modulation = 180;
	
	#ifdef AUDIO_QUALITY
		audio_quality = true;
	#else
		audio_quality = false;
	#endif
	
	_graphicsMutex = createMutex();

	SDL_ShowCursor(SDL_DISABLE);

	// Enable unicode support if possible
	SDL_EnableUNICODE(1);

	_cksumValid = false;
	
	setFeatureState(OSystem::kFeatureAutoComputeDirtyRects, true);

	SDL_Rect **rs = SDL_ListModes(0, SDL_SWSURFACE);
	if(rs == (SDL_Rect **)-1) {

		ipod_model = 0; // unknown
		_mode = GFX_NORMAL;
		_scaleFactor = 1;
		_scalerProc = Normal1x;

	} else {
	       if((*rs)->w < 320 || (*rs)->h < 240) {
			ipod_model = 1; // nano or photo
		       _mode = GFX_HALF;
			_scaleFactor = 0.5;
			_scalerProc = Scaleo5x;
	       } else {
		       ipod_model = 0; // 5G's
			_mode = GFX_NORMAL;
			_scaleFactor = 1.0;
			_scalerProc = Normal1x;
	       }
	}
	
	_modeFlags = 0;
	_fullscreen = true;

	_adjustAspectRatio = false;

	_scalerType = 0;
	_modeFlags = 0;

	_inited = true;
}

OSystem_IPOD::OSystem_IPOD()
	:
#ifdef USE_OSD
	_osdSurface(0), _osdAlpha(SDL_ALPHA_TRANSPARENT), _osdFadeStartTime(0),
#endif
	_hwscreen(0), _screen(0), _screenWidth(0), _screenHeight(0),
	_tmpscreen(0), _overlayWidth(0), _overlayHeight(0),
	_overlayVisible(false),
	_overlayscreen(0), _tmpscreen2(0),
	_samplesPerSec(0),
	_cdrom(0), _scalerProc(0), _modeChanged(false), _dirtyChecksums(0),
	_mouseVisible(false), _mouseDrawn(false), _mouseData(0), _mouseSurface(0),
	_mouseOrigSurface(0), _cursorTargetScale(1), _cursorPaletteDisabled(true),
	_joystick(0),
	_currentShakePos(0), _newShakePos(0),
	_paletteDirtyStart(0), _paletteDirtyEnd(0),
	//ipod_model(0), audio_quality(true), // iPod audio
	_graphicsMutex(0), _transactionMode(kTransactionNone) {

	
	_frameskip = 0;
	// allocate palette storage
	_currentPalette = (SDL_Color *)calloc(sizeof(SDL_Color), 256);
	_cursorPalette = (SDL_Color *)calloc(sizeof(SDL_Color), 256);

	_mouseBackup.x = _mouseBackup.y = _mouseBackup.w = _mouseBackup.h = 0;

	// reset mouse state
	memset(&_km, 0, sizeof(_km));
	memset(&_mouseCurState, 0, sizeof(_mouseCurState));
		
	#ifdef USE_COP
		ipod_init_cop(this);
	#endif
	
	_inited = false;
	
	
}

OSystem_IPOD::~OSystem_IPOD() {
	free(_dirtyChecksums);
	free(_currentPalette);
	free(_cursorPalette);
	free(_mouseData);
}

uint32 OSystem_IPOD::getMillis() {
	return SDL_GetTicks();
}

void OSystem_IPOD::delayMillis(uint msecs) {
	SDL_Delay(msecs);
}

void OSystem_IPOD::setTimerCallback(TimerProc callback, int timer) {
	SDL_SetTimer(timer, (SDL_TimerCallback) callback);
}

void OSystem_IPOD::setWindowCaption(const char *caption) {
	SDL_WM_SetCaption(caption, caption);
}

bool OSystem_IPOD::hasFeature(Feature f) {
	return
		(f == kFeatureFullscreenMode) ||
		(f == kFeatureAspectRatioCorrection) ||
		(f == kFeatureAutoComputeDirtyRects) ||
		(f == kFeatureCursorHasPalette);
}

void OSystem_IPOD::setFeatureState(Feature f, bool enable) {
	switch (f) {
	case kFeatureFullscreenMode:
		setFullscreenMode(enable);
		break;
	case kFeatureAspectRatioCorrection:
		setAspectRatioCorrection(enable);
		break;
	case kFeatureAutoComputeDirtyRects:
		if (enable)
			_modeFlags |= DF_WANT_RECT_OPTIM;
		else
			_modeFlags &= ~DF_WANT_RECT_OPTIM;
		break;
	default:
		break;
	}
}

bool OSystem_IPOD::getFeatureState(Feature f) {
	assert (_transactionMode == kTransactionNone);

	switch (f) {
	case kFeatureFullscreenMode:
		return _fullscreen;
	case kFeatureAspectRatioCorrection:
		return _adjustAspectRatio;
	case kFeatureAutoComputeDirtyRects:
		return _modeFlags & DF_WANT_RECT_OPTIM;
	default:
		return false;
	}
}

void OSystem_IPOD::quit() {
	/*
	if (_cdrom) {
		SDL_CDStop(_cdrom);
		SDL_CDClose(_cdrom);
	}
	*/
	unloadGFXMode();
	deleteMutex(_graphicsMutex);
	
	/*
	if (_joystick)
		SDL_JoystickClose(_joystick);
	*/
	SDL_ShowCursor(SDL_ENABLE);
	SDL_Quit();

	exit(0);
}

void OSystem_IPOD::setupIcon() {
	int w, h, ncols, nbytes, i;
	unsigned int rgba[256], icon[32 * 32];
	unsigned char mask[32][4];

	sscanf(scummvm_icon[0], "%d %d %d %d", &w, &h, &ncols, &nbytes);
	if ((w != 32) || (h != 32) || (ncols > 255) || (nbytes > 1)) {
		warning("Could not load the icon (%d %d %d %d)", w, h, ncols, nbytes);
		return;
	}
	for (i = 0; i < ncols; i++) {
		unsigned char code;
		char color[32];
		unsigned int col;
		sscanf(scummvm_icon[1 + i], "%c c %s", &code, color);
		if (!strcmp(color, "None"))
			col = 0x00000000;
		else if (!strcmp(color, "black"))
			col = 0xFF000000;
		else if (color[0] == '#') {
			sscanf(color + 1, "%06x", &col);
			col |= 0xFF000000;
		} else {
			warning("Could not load the icon (%d %s - %s) ", code, color, scummvm_icon[1 + i]);
			return;
		}

		rgba[code] = col;
	}
	memset(mask, 0, sizeof(mask));
	for (h = 0; h < 32; h++) {
		const char *line = scummvm_icon[1 + ncols + h];
		for (w = 0; w < 32; w++) {
			icon[w + 32 * h] = rgba[(int)line[w]];
			if (rgba[(int)line[w]] & 0xFF000000) {
				mask[h][w >> 3] |= 1 << (7 - (w & 0x07));
			}
		}
	}

	SDL_Surface *sdl_surf = SDL_CreateRGBSurfaceFrom(icon, 32, 32, 32, 32 * 4, 0xFF0000, 0x00FF00, 0x0000FF, 0xFF000000);
	SDL_WM_SetIcon(sdl_surf, (unsigned char *) mask);
	SDL_FreeSurface(sdl_surf);
}

OSystem::MutexRef OSystem_IPOD::createMutex(void) {
	return (MutexRef) SDL_CreateMutex();
}

void OSystem_IPOD::lockMutex(MutexRef mutex) {
	SDL_mutexP((SDL_mutex *) mutex);
}

void OSystem_IPOD::unlockMutex(MutexRef mutex) {
	SDL_mutexV((SDL_mutex *) mutex);
}

void OSystem_IPOD::deleteMutex(MutexRef mutex) {
	SDL_DestroyMutex((SDL_mutex *) mutex);
}


#undef CAPTURE_SOUND

#define FRAG_SIZE 512
	
int sound_fd, ipod_mixer, paramz, frag_size;

uint8 sound_buffer[FRAG_SIZE];
audio_buf_info info;

static void *sound_and_music_thread(void *params) {
	/* Init sound */
	
	OSystem::SoundProc sound_proc = ((THREAD_PARAM *)params)->sound_proc;
	void *proc_param = ((THREAD_PARAM *)params)->param;


	if(AudioInit)
	{
		sound_fd = open("/dev/dsp", O_WRONLY);
		
		if (sound_fd < 0) {
			warning("Error opening sound device!\n");
			return NULL;
		}
	
		paramz = AFMT_S16_NE;
		if (ioctl(sound_fd, SNDCTL_DSP_SETFMT, &paramz) == -1) {
			warning("Error in the SNDCTL_DSP_SETFMT ioctl!\n");
			return NULL;;
		}
		
		if ( (audio_quality) )
		{
			paramz = 1;
			if (ioctl(sound_fd, SNDCTL_DSP_CHANNELS, &paramz) == -1) {
				warning("Error in the SNDCTL_DSP_CHANNELS ioctl!\n");
				return NULL;
			}
		}
		else
		{
			paramz = 2;
			if (ioctl(sound_fd, SNDCTL_DSP_CHANNELS, &paramz) == -1) {
				warning("Error in the SNDCTL_DSP_CHANNELS ioctl!\n");
				return NULL;
			}
		}

		paramz = SAMPLES_PER_SEC;
		if (ioctl(sound_fd, SNDCTL_DSP_SPEED, &paramz) == -1) {
			warning("Error in the SNDCTL_DSP_SPEED ioctl!\n");
			return NULL;
		}
		if (paramz != SAMPLES_PER_SEC) {
			warning("%d kHz not supported!\n", SAMPLES_PER_SEC);
			return NULL;
		}
		ipod_mixer = open("/dev/mixer", O_RDWR);
		if (ipod_mixer  < 0) {
			warning("Error opening MIXER device!\n");
			return NULL;
		}
	
		int ipod_volume = 50;
		ioctl(ipod_mixer, SOUND_MIXER_WRITE_PCM, &ipod_volume);
		
		AudioInit = false;
	}

	sched_yield();
	while (1) {
		uint8 *buf = (uint8 *)sound_buffer;
		int size, written;

		sound_proc(proc_param, (byte *)sound_buffer, FRAG_SIZE);

		size = FRAG_SIZE;
		while (size > 0) {
			written = write(sound_fd, buf, size);
			buf += written;
			size -= written;
		}
	}

	return NULL;
}

#pragma mark -
#pragma mark --- Audio ---
#pragma mark -

bool OSystem_IPOD::setSoundCallback(SoundProc proc, void *param) {
	
	static THREAD_PARAM thread_param;

	if (ConfMan.hasKey("output_rate"))
		_samplesPerSec = ConfMan.getInt("output_rate");

	if ( (audio_quality)  && (ipod_model!= 1) )
	{
		_samplesPerSec = 8000;
	}
	else
	{
		_samplesPerSec = SAMPLES_PER_SEC;
	}
	
	/* And finally start the music thread */
	thread_param.param = param;
	thread_param.sound_proc = proc;

	pthread_create(&_sound_thread, NULL, sound_and_music_thread, (void *)&thread_param);

	return true;
}

void OSystem_IPOD::clearSoundCallback() {
	close(sound_fd);
	close(ipod_mixer);
}

int OSystem_IPOD::getOutputSampleRate() const {
	return _samplesPerSec;
}

#pragma mark -
#pragma mark --- CD Audio ---
#pragma mark -

bool OSystem_IPOD::openCD(int drive) {

	return (_cdrom != NULL);
}

void OSystem_IPOD::stopCD() {	/* Stop CD Audio in 1/10th of a second */
	_cdStopTime = SDL_GetTicks() + 100;
	_cdNumLoops = 0;
}

void OSystem_IPOD::playCD(int track, int num_loops, int start_frame, int duration) {
}

bool OSystem_IPOD::pollCD() {
	if (!_cdrom)
		return false;

	//return (_cdNumLoops != 0 && (SDL_GetTicks() < _cdEndTime || SDL_CDStatus(_cdrom) != CD_STOPPED));
}

void OSystem_IPOD::updateCD() {
	
	if (!_cdrom)
		return;
}
