// tinystt — CPU-only whisper.cpp tiny.en -> sentence-level .srt
// usage: tinystt input.mp4  ->  input.srt

#include "whisper.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

static std::string model_path() {
    if (const char *e = getenv("TINYSTT_MODEL")) return e;
    return "ggml-tiny.en.bin";
}

static bool decode_audio(const std::string &in, std::vector<float> &pcm) {
    char cmd[4096];
    snprintf(cmd, sizeof(cmd),
        "ffmpeg -nostdin -loglevel error -i \"%s\" -f f32le -acodec pcm_f32le -ac 1 -ar 16000 - 2>/dev/null",
        in.c_str());
    FILE *p = popen(cmd, "r");
    if (!p) return false;
    float buf[4096];
    size_t n;
    while ((n = fread(buf, sizeof(float), 4096, p)) > 0)
        pcm.insert(pcm.end(), buf, buf + n);
    return pclose(p) == 0 && !pcm.empty();
}

static std::string srt_time(int64_t t) {
    int64_t ms = t * 10;
    int h = int(ms / 3600000); ms %= 3600000;
    int m = int(ms / 60000);   ms %= 60000;
    int s = int(ms / 1000);    ms %= 1000;
    char out[16];
    snprintf(out, sizeof(out), "%02d:%02d:%02d,%03d", h, m, s, int(ms));
    return out;
}

int main(int argc, char **argv) {
    if (argc != 2) { fprintf(stderr, "usage: %s input.mp4\n", argv[0]); return 1; }
    std::string in = argv[1], mp = model_path();

    std::vector<float> pcm;
    if (!decode_audio(in, pcm)) { fprintf(stderr, "tinystt: ffmpeg failed\n"); return 2; }

    auto cparams = whisper_context_default_params();
    cparams.use_gpu = false;
    whisper_context *ctx = whisper_init_from_file_with_params(mp.c_str(), cparams);
    if (!ctx) { fprintf(stderr, "tinystt: model not found at %s\n", mp.c_str()); return 3; }

    auto wp = whisper_full_default_params(WHISPER_SAMPLING_GREEDY);
    wp.language = "en";
    // default 1 core; override with TINYSTT_THREADS=N
    if (const char *t = getenv("TINYSTT_THREADS")) {
        int n = atoi(t);
        wp.n_threads = n > 0 ? n : 1;
    } else {
        wp.n_threads = 1;
    }
    wp.print_progress = true;

    if (whisper_full(ctx, wp, pcm.data(), int(pcm.size()))) { whisper_free(ctx); return 4; }

    std::string out = in;
    size_t dot = out.find_last_of('.'), sl = out.find_last_of('/');
    if (dot != std::string::npos && (sl == std::string::npos || dot > sl))
        out = out.substr(0, dot);
    out += ".srt";

    FILE *f = fopen(out.c_str(), "w");
    if (!f) { whisper_free(ctx); return 5; }

    int n_seg = whisper_full_n_segments(ctx), cue = 0;
    for (int i = 0; i < n_seg; ++i) {
        const char *txt = whisper_full_get_segment_text(ctx, i);
        if (!txt || !*txt) continue;
        // skip leading space whisper sometimes adds
        while (*txt == ' ') ++txt;
        if (!*txt) continue;
        fprintf(f, "%d\n%s --> %s\n%s\n\n", ++cue,
                srt_time(whisper_full_get_segment_t0(ctx, i)).c_str(),
                srt_time(whisper_full_get_segment_t1(ctx, i)).c_str(),
                txt);
    }
    fclose(f);
    whisper_free(ctx);
    fprintf(stderr, "tinystt: wrote %s (%d cues)\n", out.c_str(), cue);
    return 0;
}
