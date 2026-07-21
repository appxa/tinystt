# tinystt — minimal whisper.cpp SRT subtitle generator

CPU-only CLI that transcribes any video/audio to sentence-level `.srt`
using whisper.cpp tiny.en.

## Requirements

C++17, cmake ≥ 3.22, g++, ffmpeg, git.

## Install (build + PATH)

```bash
git clone https://github.com/appxa/tinystt.git
cd tinystt
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
install -m 755 build/tinystt ~/.local/bin/tinystt
```

Ensure `~/.local/bin` is on PATH (logout/login if `tinystt` not found):

```bash
echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc
```

Download the model once (cwd, or set `TINYSTT_MODEL`):

```bash
curl -L -o ggml-tiny.en.bin \
  https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-tiny.en.bin
```

## Usage

```bash
tinystt input.mp4                 # → input.srt
TINYSTT_THREADS=2 tinystt input.mp4
TINYSTT_MODEL=/path/to/ggml-tiny.en.bin tinystt input.mp4
```

Default: 1 CPU thread. Model from cwd or `TINYSTT_MODEL`.

whisper.cpp is fetched at build time via CMake FetchContent — no submodules.

## License

MIT
