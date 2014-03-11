#include "snd/inc/audio.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <mutex>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include "hacc/inc/documents.h"
#include "hacc/inc/haccable_standard.h"

namespace snd {
    using namespace util;

    void Audio::finish () {
        FILE* f = fopen(filename.c_str(), "r");
        if (!f) {
            throw hacc::X::Logic_Error("Could not open " + filename + " for reading: " + strerror(errno));
        }
        fseek(f, 0, SEEK_END);
        auto bytes = ftell(f);
        if (bytes % 4) {
            fclose(f);
            throw hacc::X::Logic_Error("Length of " + filename + " not divisible by 4");
        }
        length = bytes / sizeof(Sample);
        fseek(f, 0, SEEK_SET);
        samples = new Sample [length];
        if (fread(samples, sizeof(Sample), length, f) != length) {
            fclose(f);
            throw hacc::X::Logic_Error("Could not read from " + filename + ": " + strerror(errno));
        }
        if (fclose(f) != 0) {
            throw hacc::X::Logic_Error("Could not close " + filename + ": " + strerror(errno));
        }
    }

    static SDL_AudioDeviceID dev = 0;

    static Links<Voice> voices;
    static Links<Voice> new_voices;
    std::mutex new_voices_mutex;

    void mix_voices (void* _, uint8* buf, int len) {
        auto samples = (Sample*)buf;
        len /= sizeof(Sample);  // Assuming len is divisible by sizeof(Sample)
        new_voices_mutex.lock();
        while (!new_voices.empty()) {
            new_voices.begin()->link(voices);
        }
        new_voices_mutex.unlock();
        for (int i = 0; i < len; i++) {
            int32 l = 0;
            int32 r = 0;
            Voice* destroy = NULL;
            for (auto& v : voices) {
                if (destroy) {
                    destroy->auto_delete_from->destroy(destroy);
                    destroy = NULL;
                }
                if (v.audio && !v.paused) {
                    v.pos %= v.audio->length;
                    l += v.audio->samples[v.pos].l;
                    r += v.audio->samples[v.pos].r;
                    v.pos += 1;
                    if (v.pos >= v.audio->length && v.auto_delete_from) {
                        destroy = &v;
                    }
                }
            }
            if (destroy) {
                destroy->auto_delete_from->destroy(destroy);
                destroy = NULL;
            }
            samples[i].l = l > 32767 ? 32767 : l < -32768 ? -32768 : l;
            samples[i].r = r > 32767 ? 32767 : r < -32768 ? -32768 : r;
        }
    }

    void init () {
        static bool initted = false;
        if (!initted) {
            initted = true;
            if (SDL_Init(SDL_INIT_AUDIO) < 0) {
                printf("SDL_Init failed: %s\n", SDL_GetError());
            }

            SDL_AudioSpec spec;
            spec.freq = 44100;
            spec.format = AUDIO_S16;
            spec.channels = 2;
            spec.samples = 4096;
            spec.callback = mix_voices;
            spec.userdata = NULL;

            dev = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
            SDL_PauseAudioDevice(dev, 0);
        }
    }

    void Voice::finish () {
        init();
        new_voices_mutex.lock();
        link(new_voices);
        new_voices_mutex.unlock();
    }

    Voice::Voice (Audio* audio) : audio(audio) { finish(); }

    Voice::~Voice () {
        new_voices_mutex.lock();
        unlink();
        new_voices_mutex.unlock();
    }

} using namespace snd;

HACCABLE(Audio) {
    name("snd::Audio");
    attr("filename", member(&Audio::filename));
    finish(&Audio::finish);
}

HACCABLE(Voice) {
    name("snd::Voice");
    attr("audio", member(&Voice::audio).optional());
    attr("pos", member(&Voice::pos).optional());
    attr("paused", member(&Voice::paused).optional());
    attr("auto_delete_from", member(&Voice::auto_delete_from).optional());
    finish(&Voice::finish);
}
