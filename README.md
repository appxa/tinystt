# tinysst — minimal whisper.cpp SRT subtitle generator

CPU-only CLI that transcribes any video/audio to `.srt` subtitles using
whisper.cpp tiny.en with word-level timestamps.

## Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4
```

## Usage

```bash
curl -L -o ggml-tiny.en.bin \
  https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-tiny.en.bin

./build/tinysst input.mp4    # → input.srt
```
#point
Put `ggml-tiny.en.bin` and the video file in the same folder, then run
`tinysst video.mp4` — it auto-picks the model from the current directory.

## Requirements

C++17, cmake ≥ 3.22, ffmpeg.

whisper.cpp fetched at build time via CMake FetchContent — no submodules.

## License

MIT
