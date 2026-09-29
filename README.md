# c-photo-compressor

A minimal, educational lossy image compressor written in C.

> This is a learning project. It is not intended to compete with libjpeg-turbo, WebP, or AVIF.

## Goal

Build a CLI tool (and later a reusable C library) that compresses RGB photographs using a simplified JPEG-like pipeline:

- RGB → YCbCr 4:2:0
- 8×8 DCT / IDCT
- Quantization
- Zig-zag scan
- Huffman entropy coding
- Proprietary binary format

## Roadmap

See [ROADMAP.md](ROADMAP.md).

## License

MIT (or choose your own — to be decided by the author).
