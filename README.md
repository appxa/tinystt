# tinystt — minimal whisper.cpp SRT subtitle generator

CPU-only CLI that transcribes any video/audio to sentence-level `.srt`
using whisper.cpp tiny.en.

## Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4
```

## Usage

```bash
curl -L -o ggml-tiny.en.bin \
  https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-tiny.en.bin

./build/tinystt input.mp4    # → input.srt
TINYSTT_THREADS=2 ./build/tinystt input.mp4   # optional: more cores
```

Default uses 1 CPU thread. Set `TINYSTT_THREADS=N` for more.

Put `ggml-tiny.en.bin` and the video file in the same folder, then run
`tinystt video.mp4` — model picked from cwd (or `TINYSTT_MODEL`).

## Requirements

C++17, cmake ≥ 3.22, ffmpeg.

whisper.cpp fetched at build time via CMake FetchContent — no submodules.

## License

MIT
