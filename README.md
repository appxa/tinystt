# tinystt — minimal whisper.cpp SRT subtitle generator

CPU-only CLI that transcribes any video/audio to sentence-level `.srt`
using whisper.cpp tiny.en.

## Download (ready builds)

Get the latest zip/tar from:
https://github.com/appxa/tinystt/releases

| File | OS |
|------|-----|
| `tinystt-linux-x64.tar.gz` | Linux |
| `tinystt-windows-x64.zip` | Windows |
| `tinystt-macos-x64.tar.gz` | macOS |

Each archive is only the binary. You still need **ffmpeg** and the **model**.

### Linux / macOS

```bash
# 1) unpack binary onto PATH
tar -xzf tinystt-linux-x64.tar.gz   # or tinystt-macos-x64.tar.gz
mkdir -p ~/.local/bin
mv tinystt ~/.local/bin/
chmod +x ~/.local/bin/tinystt
echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.bashrc   # mac: ~/.zshrc
source ~/.bashrc

# 2) ffmpeg
# Debian/Ubuntu: sudo apt install ffmpeg
# Fedora:        sudo dnf install ffmpeg
# macOS:         brew install ffmpeg

# 3) model (once; put next to video, or set TINYSTT_MODEL)
curl -L -o ggml-tiny.en.bin \
  https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-tiny.en.bin

# 4) run
tinystt video.mp4    # → video.srt
```

### Windows

1. Install [ffmpeg](https://ffmpeg.org/download.html) and put it on PATH.
2. Unzip `tinystt-windows-x64.zip` → you get `tinystt.exe`.
3. Put `tinystt.exe` somewhere on PATH, or keep it next to your video.
4. Download the model once:

```powershell
curl.exe -L -o ggml-tiny.en.bin https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-tiny.en.bin
```

5. Run (cmd / PowerShell):

```powershell
tinystt.exe video.mp4
# or: .\tinystt.exe video.mp4
```

Optional env vars (all OS):

```bash
TINYSTT_THREADS=2 tinystt video.mp4
TINYSTT_MODEL=/path/to/ggml-tiny.en.bin tinystt video.mp4
```

Default: 1 CPU thread. Model from cwd or `TINYSTT_MODEL`.

## Build from source

Needs: C++17, cmake ≥ 3.22, g++/MSVC/clang, ffmpeg, git.

```bash
git clone https://github.com/appxa/tinystt.git
cd tinystt
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
install -m 755 build/tinystt ~/.local/bin/tinystt   # Windows: copy build\Release\tinystt.exe
```

whisper.cpp is fetched at build time via CMake FetchContent — no submodules.

Maintainers: tag `v0.1.0` (or any `v*`) and push to publish release zips via CI.

## License

MIT
