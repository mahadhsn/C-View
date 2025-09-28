# C-View

A tiny, fast **command‑line image processor** for uncompressed **24‑bit BMP** files. C‑View focuses on the essentials—grayscale, reflection, rotation, blur, and edge detection—so you can batch quick edits without heavyweight GUIs.

> **Version:** 2.2.0  
> **Language:** C  
> **Platforms:** Linux, macOS (Intel/Apple Silicon)

---

## Resume Highlights

- • **Developed a C‑based utility** to apply filters like grayscale, reflection, rotation, edge detection, and blur
- • **Designed to process images up to ~30% faster** than comparable tools with reduced memory use

---

## Features

- **Grayscale** — weighted RGB luminance
- **Reflection (horizontal)** — mirror each scanline in‑place (padding‑safe)
- **Rotate 90° left** — updates BMP header and reflows pixel data
- **Blur (5×5 box kernel)** — averages a neighborhood per pixel
- **Edge detection (Sobel)** — adaptive threshold from average gradient magnitude

---

## Supported Formats & Limitations

- **Input:** uncompressed **24‑bit BMP** (`.bmp`) with a standard 54‑byte header  
- **Output:** 24‑bit BMP (row padding preserved/adjusted)  
- Not tested on compressed BMP variants, paletted (8‑bit) BMPs, or color‑mask variants (BI_BITFIELDS)

---

## Build

**Prerequisites:** `gcc` (or clang), `make`

```bash
# Release binary
make c-view

# Test binary with coverage flags
make c-view-test
```

Artifacts:
- `./c-view` – production CLI
- `./c-view-test` – coverage‑enabled build for tests/CI

---

## Usage

```text
./c-view [OPTION] <input.bmp> <output.bmp>
```

**Options**
```
--help   Show help
-gs      Grayscale
-rs      Reflect horizontally
-rot     Rotate 90° left
-bl      Blur (5×5 box)
-ed      Edge detection (Sobel + adaptive threshold)
```

**Examples**
```bash
./c-view --help

./c-view -gs ./images/source.bmp ./out/grayscale.bmp
./c-view -rs ./images/source.bmp ./out/reflected.bmp
./c-view -rot ./images/source.bmp ./out/rotated.bmp
./c-view -bl ./images/source.bmp ./out/blur.bmp
./c-view -ed ./images/source.bmp ./out/edges.bmp
```

**Exit codes**
- `0` success
- `1` invalid args or I/O error

---

## Project Directory Structure

```
Directory structure:
└── mahadhsn-c-view/
    ├── README.md
    ├── c-view
    ├── Makefile
    ├── src/
    │   ├── blur.c
    │   ├── blur.h
    │   ├── C-View.c
    │   ├── edgeDetect.c
    │   ├── edgeDetect.h
    │   ├── grayscale.c
    │   ├── grayscale.h
    │   ├── reflection.c
    │   ├── reflection.h
    │   ├── rotation.c
    │   └── rotation.h
    ├── test/
    │   ├── help.txt
    │   ├── testscript.sh
    │   └── usage.txt
    └── .github/
        └── workflows/
            └── test.yml

```

---

## Performance Notes

- Operates in memory with per‑pass buffers; suitable for small–moderate BMPs.
- BMP row padding is preserved per spec (typical formula in code):  
  `row_padded = (width * 3 + 3) & ~3`  
  or equivalently `padding = (4 - (width * 3) % 4) % 4`.
- Blur and edge detection are **O(W×H×K²)** per pass (K = kernel size). Consider downsampling for very large images.
- Edge threshold adapts from the image’s average gradient magnitude to reduce noise.

---

## Testing & Continuous Integration

**Local**
```bash
make c-view-test
./test/testscript.sh | tee ./test/test_results.txt

# Generate gcov coverage reports (one per module)
gcov c-view-test-C-View.c \
     c-view-test-blur.c \
     c-view-test-reflection.c \
     c-view-test-rotation.c \
     c-view-test-grayscale.c \
     c-view-test-edgeDetect.c
```

**GitHub Actions**
- Builds both binaries
- Runs `test/testscript.sh`
- Uploads `test/test_results.txt` and `*.gcov` artifacts  
Workflow file: `.github/workflows/test.yml`

---

## Troubleshooting

- **“Usage: ./c-view [OPTIONS]”** → Check argument order and count  
- **Return code 1** → Missing/invalid input file or output path not writable  
- **Corrupted output** → Ensure input is uncompressed 24‑bit BMP  
- **Slow on huge images** → Downsample first; heavy filters are quadratic in kernel size

---

## Contributing

1. Fork and create a feature branch
2. Keep functions pure (operate on `FILE*` streams; avoid globals)
3. Preserve BMP header semantics and padding
4. Add a golden‑image test when adding a new filter/flag
5. Run the test script and ensure all tests pass

**Ideas / TODO**
- Vertical reflection, 180°/right rotation
- Parameterized blur (Gaussian) and kernel size
- Support additional BMP variants and/or PNG via a minimal decoder
- SIMD optimizations for hot loops

---

## License

MIT
