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

#ifndef OPTIONS_DIALOG_H
#define OPTIONS_DIALOG_H

#include "gui/dialog.h"
#include "common/str.h"

#ifdef SMALL_SCREEN_DEVICE
#include "gui/KeysDialog.h"
#endif

namespace GUI {

class BrowserDialog;
class CheckboxWidget;
class PopUpWidget;
class SliderWidget;
class StaticTextWidget;

class OptionsDialog : public Dialog {
	typedef Common::String String;
public:
	OptionsDialog(const String &domain, int x, int y, int w, int h);
	OptionsDialog(const String &domain, const String &name);

	void init();

	void open();
	void close();
	void handleCommand(CommandSender *sender, uint32 cmd, uint32 data);

	virtual void handleScreenChanged();

	enum {
		kOKCmd					= 'ok  '
	};

protected:
	/** Config domain this dialog is used to edit. */
	String _domain;

	ButtonWidget *_soundFontButton;
	StaticTextWidget *_soundFont;

	void addGraphicControls(GuiObject *boss, const String &prefix);
	void addAudioControls(GuiObject *boss, const String &prefix);
	void addMIDIControls(GuiObject *boss, const String &prefix);
	void addVolumeControls(GuiObject *boss, const String &prefix);
	// The default value is the launcher's non-scaled talkspeed value. When SCUMM uses the widget,
	// it uses its own scale
	void addSubtitleControls(GuiObject *boss, const String &prefix, int maxSliderVal = 255);

	void setGraphicSettingsState(bool enabled);
	void setAudioSettingsState(bool enabled);
	void setMIDISettingsState(bool enabled);
	void setVolumeSettingsState(bool enabled);
	void setSubtitleSettingsState(bool enabled);

private:
	//
	// Graphics controls
	//
	bool _enableGraphicSettings;
	PopUpWidget *_gfxPopUp;
	CheckboxWidget *_fullscreenCheckbox;
	CheckboxWidget *_aspectCheckbox;
	PopUpWidget *_renderModePopUp;

	//
	// Audio controls
	//
	bool _enableAudioSettings;
	PopUpWidget *_midiPopUp;

	//
	// MIDI controls
	//
	bool _enableMIDISettings;
	CheckboxWidget *_multiMidiCheckbox;
	CheckboxWidget *_mt32Checkbox;
	CheckboxWidget *_enableGSCheckbox;
	StaticTextWidget *_midiGainDesc;
	SliderWidget *_midiGainSlider;
	StaticTextWidget *_midiGainLabel;
	
	//
	// Subtitle controls
	//
	int getSubtitleMode(bool subtitles, bool speech_mute);
	bool _enableSubtitleSettings;
	StaticTextWidget *_subToggleDesc;
	ButtonWidget *_subToggleButton;
	int _subMode;
	static const char *_subModeDesc[];
	StaticTextWidget *_subSpeedDesc;
	SliderWidget *_subSpeedSlider;
	StaticTextWidget *_subSpeedLabel;

	//
	// Volume controls
	//
	bool _enableVolumeSettings;

	StaticTextWidget *_musicVolumeDesc;
	SliderWidget *_musicVolumeSlider;
	StaticTextWidget *_musicVolumeLabel;

	StaticTextWidget *_sfxVolumeDesc;
	SliderWidget *_sfxVolumeSlider;
	StaticTextWidget *_sfxVolumeLabel;

	StaticTextWidget *_speechVolumeDesc;
	SliderWidget *_speechVolumeSlider;
	StaticTextWidget *_speechVolumeLabel;
};


class GlobalOptionsDialog : public OptionsDialog {
	typedef Common::String String;
public:
	GlobalOptionsDialog();
	~GlobalOptionsDialog();

	void open();
	void close();
	void handleCommand(CommandSender *sender, uint32 cmd, uint32 data);

protected:
#ifdef SMALL_SCREEN_DEVICE
	KeysDialog *_keysDialog;
#endif
	StaticTextWidget *_savePath;
	StaticTextWidget *_themePath;
	StaticTextWidget *_extraPath;
};

} // End of namespace GUI

#endif
