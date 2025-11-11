# CEF FFmpeg with Proprietary Codecs for YouTube Live Streams

## Quick Start

**To enable YouTube live streams:**

1. **Download**: Get CEF Standard Distribution from https://cef-builds.spotifycdn.com/index.html
2. **Extract**: Find `libffmpeg.dll` or `chrome_elf.dll` in the `Release/` folder (1-3 MB file)
3. **Place**: Copy the DLL to `vendor/cef3/cef/Release/` in your MTA:SA source
4. **Build**: Rebuild MTA:SA - the DLL will be automatically copied to output
5. **Done**: YouTube live streams should now work

**Quick Download Link**: https://cef-builds.spotifycdn.com/index.html (Select "Standard Distribution" for Windows x86)

---

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

### Option 1: Download Pre-built (Recommended - Easiest)

#### **Recommended Source: CEF Builds with Proprietary Codecs**
Download pre-built CEF distributions that include FFmpeg with proprietary codecs:

1. **Spotify CEF Builds** (Recommended):
   - URL: https://cef-builds.spotifycdn.com/index.html
   - Look for builds with "standard" distribution (includes proprietary codecs)
   - Download the CEF build matching your current CEF version
   - Extract and locate `libffmpeg.dll` or `chrome_elf.dll` from the Release folder
   - File size: ~1-3 MB

2. **CEF Automated Builds**:
   - URL: https://cef-builds.spotifycdn.com/index.html
   - Select your platform (Windows 32-bit or 64-bit)
   - Choose "Standard Distribution" (not minimal)
   - Download and extract to find FFmpeg DLL

3. **GitHub Community Builds**:
   - Search for "CEF proprietary codecs" on GitHub
   - Look for repositories providing pre-built FFmpeg DLLs
   - Example: https://github.com/topics/cef-binary (community builds)

#### **Alternative: Pre-built FFmpeg from Other Sources**
If you need just the FFmpeg DLL:
- **Zeranoe FFmpeg Builds**: https://github.com/BtbN/FFmpeg-Builds/releases
  - Download "ffmpeg-master-latest-win32-gpl-shared.zip" or win64 version
  - Extract and locate `avcodec-XX.dll`, `avformat-XX.dll`, `avutil-XX.dll`
  - Note: This is full FFmpeg (larger), but includes all codecs

### Option 2: Build from Source (For minimal size)
Build Chromium's FFmpeg with only H.264/AAC codecs:
```bash
# In Chromium source tree
gn gen out/Release --args='proprietary_codecs=true ffmpeg_branding="Chrome" is_component_ffmpeg=true'
ninja -C out/Release ffmpeg
```

This produces a minimal ffmpeg library (~1-3 MB) with only proprietary codecs.

**Build Instructions**:
1. Follow Chromium build setup: https://chromium.googlesource.com/chromium/src/+/main/docs/windows_build_instructions.md
2. Set up depot_tools
3. Use the GN args above to enable proprietary codecs
4. Build only the FFmpeg component

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
