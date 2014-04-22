/*
Copyright (c) 2013 Steven Arnow
'sound_sdl.c' - This file is part of libdarnit_tpw

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.
*/

#include "../main.h"

void tpw_sound_callback(void (*callback)(void *data, void *mixdata, int bytes)) {
	tpw.sound.callback = callback;

	return;
}


void tpw_sound_do_callback(void *data, void *mixdata, int bytes) {
	(tpw.sound.callback)(data, mixdata, bytes);

	return;
}


int tpw_sound_open(TPW_SOUND_SETTINGS settings) {
	SDL_AudioSpec fmt;

	tpw.sound = settings;

	fmt.freq = settings.sample_rate;
	switch (settings.format) {
		case TPW_SAMPLE_FORMAT_S16LE:
			fmt.format = AUDIO_S16;
			break;
		case TPW_SAMPLE_FORMAT_S8:
			fmt.format = AUDIO_S8;
			break;
	}
	fmt.channels = settings.channels;
	fmt.samples = settings.samples;
	fmt.callback = (void *) tpw_sound_do_callback;
	fmt.userdata = settings.userdata;

	return SDL_OpenAudio(&fmt, NULL);
}


void tpw_sound_control(TPW_SOUND_COMMAND command) {
	switch(command) {
		case TPW_SOUND_COMMAND_PAUSE:
			SDL_PauseAudio(1);
			break;
		case TPW_SOUND_COMMAND_PLAY:
			SDL_PauseAudio(0);
			break;
	}
	
	return;
}


#ifdef __ALSA_CAPTURE__

#include <alsa/asoundlib.h>

struct {
	void			(*callback)(int channels, void *sample, int samples, void *user_data);
	void			*user_data;
	snd_pcm_t		*alsa_handle;
	int			channels;
} tpw_mic_capture;


void tpw_sound_capture_handler(snd_async_handler_t *pcm_callback) {
	char buff[1024];
	snd_pcm_t *pcm_handle = snd_async_handler_get_pcm(pcm_callback);
	snd_pcm_sframes_t avail;
	int this;

	avail = snd_pcm_avail_update(pcm_handle);
	for (; avail; ) {
		snd_pcm_readi(pcm_handle, buff, 256);
		this = (avail < 1024) ? avail : 1024;
		this /= tpw_mic_capture.channels;
		this /= 2;

		if (tpw_mic_capture.callback)
			tpw_mic_capture.callback(tpw_mic_capture.channels, buff, this, tpw_mic_capture.user_data);
	}
	return;
}


void tpw_sound_capture_init() {
	tpw_mic_capture.alsa_handle = NULL;
	tpw_mic_capture.user_data = NULL;
	tpw_mic_capture.callback = NULL;

	return;
}

int tpw_sound_capture_attempt_init(const char *device, int channels) {
	int err;
	unsigned int rate;
	snd_pcm_hw_params_t *hw_params;
	snd_async_handler_t *pcm_callback;
	char *err_str;
	rate = 44100;

	hw_params = NULL;

	if ((err = snd_pcm_open(&tpw_mic_capture.alsa_handle, device, SND_PCM_STREAM_CAPTURE, SND_PCM_NONBLOCK)) < 0) {
		err_str = "Unable to open audio device";
		goto error;
	}

	if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
		err_str = "Unable to alloc audio hardware parameter structure";
		goto error;
	}

	if ((err = snd_pcm_hw_params_any(tpw_mic_capture.alsa_handle, hw_params)) < 0) {
		err_str = "Unable to get default hardware parameters";
		goto error;
	}

	if ((err = snd_pcm_hw_params_set_access(tpw_mic_capture.alsa_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		err_str = "Unable to set capture access type";
		goto error;
	}

	if ((err = snd_pcm_hw_params_set_format(tpw_mic_capture.alsa_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
		err_str = "Unable to set capture sample format";
		goto error;
	}

	if ((err = snd_pcm_hw_params_set_rate_near(tpw_mic_capture.alsa_handle, hw_params, &rate, 0)) < 0) {
		err_str = "Unable to set capture sample rate";
		goto error;
	}

	if ((err = snd_pcm_hw_params_set_channels(tpw_mic_capture.alsa_handle, hw_params, channels)) < 0) {
		err_str = "Unable to set capture sample channels";
		goto error;
	}

	if ((err = snd_pcm_hw_params(tpw_mic_capture.alsa_handle, hw_params)) < 0) {
		err_str = "Unable to apply capture hardware parameters";
		goto error;
	}

	snd_pcm_hw_params_free(hw_params);
	hw_params = NULL;

	if ((err = snd_pcm_prepare(tpw_mic_capture.alsa_handle)) < 0) {
		err_str = "Unable to prepare capture hardware for use";
		goto error;
	}
	
	snd_async_add_pcm_handler(&pcm_callback, tpw_mic_capture.alsa_handle, tpw_sound_capture_handler, NULL);
	snd_pcm_start(tpw_mic_capture.alsa_handle);
	
	return 1;
	
	error:
		fprintf(stderr, "Capture hardware failed to open: %s: %s\n", err_str, snd_strerror(err));
		if (hw_params)
			free(hw_params);
		if (tpw_mic_capture.alsa_handle)
			snd_pcm_close(tpw_mic_capture.alsa_handle);
		tpw_mic_capture.alsa_handle = NULL;
			
	return 0;
}


int tpw_sound_capture_register(void (*callback)(int, void *, int, void *), void *user_data) {

	tpw_mic_capture.callback = callback;
	tpw_mic_capture.user_data = user_data;

	if (tpw_mic_capture.alsa_handle)
		return 1;
	if (tpw_sound_capture_attempt_init("default:0", 2))
		return 1;
	if (tpw_sound_capture_attempt_init("default:0", 1))
		return 1;
	if (tpw_sound_capture_attempt_init("hw:0,0", 2))
		return 1;
	if (tpw_sound_capture_attempt_init("hw:0,0", 1))
		return 1;
	

	return 0;
}


void tpw_sound_capture_unregister() {
}


#else

int tpw_sound_capture_register() {
	/* Not implemented, fail */
	return 0;
}

void tpw_sound_capture_unregister() {
	/* Not supported, nothing to unregister */
	return;
}


void twp_sound_capture_init() {
	/* Not implemented */
	return;
}

#endif
