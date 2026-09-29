# C Photo Compressor — Roadmap

A minimal, educational lossy image compressor in C for personal learning.

## Project goal

Build a CLI tool (later a C library) that compresses RGB photographs using a simplified JPEG-like pipeline:

- RGB → YCbCr 4:2:0
- 8×8 DCT / IDCT
- Quantization
- Zig-zag scan
- Huffman entropy coding
- Proprietary binary format

## What this is NOT

- A production replacement for JPEG, WebP, AVIF, or libjpeg-turbo.
- A container for multiple images (out of initial scope).
- A lossless compressor.
- Security-critical software.

## Architecture decisions

- **Language:** C (C11 or C99).
- **Build system:** `make`.
- **Image loader:** `stb_image.h` and `stb_image_write.h`.
- **DCT/IDCT:** naive double-loop implementation first, optimized later.
- **Default quality:** 90.
- **Color channels:** RGB24 now; RGBA reserved for a future header field.
- **Concurrency:** single-threaded first.
- **Comparison baseline:** libjpeg-turbo quality 90.

## Folder structure (proposed)

```text
c-photo-compressor/
├── README.md
├── ROADMAP.md
├── Makefile
├── .gitignore
├── src/
│   ├── main.c
│   ├── codec.h
│   ├── codec.c
│   ├── colorspace.c
│   ├── dct.c
│   ├── quantize.c
│   ├── huffman.c
│   └── bitstream.c
├── include/
│   └── cphotocompress.h
├── tests/
│   ├── test_roundtrip.c
│   └── test_benchmark.c
├── tools/
│   └── generate_test_dataset.sh
└── dataset/
    └── README.md
```

## Milestones

### M0 — Scaffold

- [ ] Repo setup, Makefile, .gitignore, folder structure.
- [ ] Add `stb_image.h` and `stb_image_write.h` as vendored deps.
- [ ] Create a small real-photo test dataset.

### M1 — Image loading

- [ ] Load any supported image into raw RGB24 buffer.
- [ ] Save RGB buffer back to PNG for verification.

### M2 — Colorspace & subsampling

- [ ] RGB ↔ YCbCr conversion.
- [ ] 4:2:0 chroma subsampling and upsampling.

### M3 — DCT/IDCT

- [ ] Naive 8×8 forward DCT.
- [ ] Naive 8×8 inverse IDCT.
- [ ] Round-trip test with acceptable error.

### M4 — Quantization & zig-zag

- [ ] Quantization tables (default quality 90).
- [ ] Zig-zag reordering.
- [ ] Dequantization and inverse zig-zag.

### M5 — Huffman coding

- [ ] Build symbol frequency tables from DCT coefficients.
- [ ] Build canonical Huffman tables.
- [ ] Bitstream writer/reader.
- [ ] Encode/decode a block stream.

### M6 — File format

- [ ] Design proprietary binary header (magic, version, width, height, channels, quality, tables).
- [ ] Write encoder output to file.
- [ ] Validate magic and version on read.

### M7 — Round-trip decoder

- [ ] Full decode pipeline.
- [ ] Bit-exact pixel comparison is not expected; verify PSNR ≥ threshold.

### M8 — CLI

- [ ] `cphotoc compress input output.cph`
- [ ] `cphotoc decompress input.cph output.png`
- [ ] `cphotoc benchmark input_dir`

### M9 — Benchmarks

- [ ] Compression ratio.
- [ ] Compression/decompression throughput (MB/s).
- [ ] PSNR vs original.
- [ ] Memory usage.
- [ ] Comparison table vs libjpeg-turbo `-quality 90`.

### M10 — Library API

- [ ] Stable C API in `include/cphotocompress.h`.
- [ ] Separate encoder/decoder from CLI.

### M11 — Optimizations (future)

- [ ] AAN fast DCT.
- [ ] SIMD (SSE/NEON).
- [ ] Multi-threaded batch processing.
- [ ] Adaptive quantization / trellis quantization.

### M12 — Container format (future)

- [ ] Multi-image archive with index.
- [ ] Optional parser to re-emit PNG/JPEG.

## Coding standards

- Compile with `-Wall -Wextra -Wpedantic` and aim for zero warnings.
- Use AddressSanitizer and UndefinedBehaviorSanitizer during development (`-fsanitize=address,undefined`).
- Zero tolerance for memory leaks under normal operation.
- Explicit error handling on every syscall and allocation.
- No `goto` for normal flow; acceptable for centralized cleanup if needed.

## Metrics

| Metric | Why it matters |
|---|---|
| Compression ratio | Storage saved. |
| Throughput (MB/s) | How fast it processes photos. |
| PSNR | Objective quality vs original. |
| Peak RAM | Memory footprint. |
| Time with I/O | Real-world batch performance. |

## Risks

- Lossy compression on already-JPEG photos will not beat the original JPEG size.
- Naive DCT will be slow; acceptable for learning.
- Huffman implementation is error-prone; round-trip tests are essential.

## Resources

- JPEG standard (ITU-T T.81 / ISO/IEC 10918-1).
- libjpeg-turbo source for reference.
- `stb_image.h` by Sean Barrett.
