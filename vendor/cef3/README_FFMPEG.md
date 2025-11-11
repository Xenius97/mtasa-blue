# CEF FFmpeg with Proprietary Codecs for YouTube Live Streams

## Problem
YouTube live streams require H.264 video codec and AAC audio codec support, which are not included in CEF "minimal" builds.

## Solution
To enable YouTube live streams and other H.264/AAC content, you need to provide an FFmpeg library with proprietary codec support.

## Required File
Place one of the following files in `vendor/cef3/cef/Release/`:

### For CEF 90+ (newer versions):
- **File**: `chrome_elf.dll` or `libffmpeg.dll` (depends on CEF version)
- **Size**: Typically 1-3 MB (minimal build with H.264/AAC only)
- **Source**: Build from Chromium source with `proprietary_codecs=true` and `ffmpeg_branding="Chrome"`

### For older CEF versions:
- **File**: `ffmpeg.dll`
- **Size**: Typically 1-3 MB
- **Source**: Build from FFmpeg source with H.264 and AAC codec support enabled

## How to Obtain

### Option 1: Build from Source (Recommended for minimal size)
Build Chromium's FFmpeg with only H.264/AAC codecs:
```bash
# In Chromium source tree
gn gen out/Release --args='proprietary_codecs=true ffmpeg_branding="Chrome" is_component_ffmpeg=true'
ninja -C out/Release ffmpeg
```

This produces a minimal ffmpeg library (~1-3 MB) with only proprietary codecs.

### Option 2: Download Pre-built (Easier but larger)
Some third parties provide pre-built FFmpeg libraries with proprietary codecs:
- Check CEF forums or community builds
- Ensure the version matches your CEF version
- Verify it includes H.264 and AAC codec support

## Installation
1. Obtain the appropriate FFmpeg DLL (see above)
2. Place it in `vendor/cef3/cef/Release/`
3. The build system will automatically copy it to the output directory
4. Rebuild MTA:SA

## Verification
After installation, YouTube live streams and H.264 videos should play without "your browser can't play this video" errors.

## Notes
- The FFmpeg library with proprietary codecs cannot be included in this repository due to licensing and size considerations
- File size should be kept under 5 MB to avoid bloat
- Ensure the FFmpeg version matches your CEF version for compatibility
