# CEF FFmpeg with Proprietary Codecs for YouTube Live Streams

## Quick Start

**To enable YouTube live streams:**

### Option A: Use CEF with Built-in Proprietary Codecs (Recommended)
Recent CEF versions (90+) include FFmpeg directly in `libcef.dll` - you need the **proprietary codecs** variant:

1. **Check your CEF version**: Look at your current `vendor/cef3/cef/` structure
2. **If using older CEF (pre-90)**: You need a separate `ffmpeg.dll` (see Option B below)
3. **If using newer CEF (90+)**: FFmpeg is in `libcef.dll` - you need to replace your entire CEF build with one that has proprietary codecs enabled

### Option B: Add Separate FFmpeg DLL (For Older CEF or Minimal Builds)

**Working Download Sources:**

1. **GitHub - DSpeichert/cef-minimal-binary** (Direct FFmpeg DLLs):
   - URL: https://github.com/dspeichert/cef-minimal-binary/releases
   - Look for releases with "proprietary" in the name
   - Download and extract `libffmpeg.dll` or `ffmpeg.dll`
   - Size: ~1-2 MB

2. **GitHub Community Builds**:
   - Search: https://github.com/search?q=libffmpeg.dll+proprietary+codecs&type=repositories
   - Look for CEF wrapper projects that provide proprietary codec builds
   
3. **Alternative: Extract from Chrome**:
   - Download Google Chrome installer
   - Extract it (use 7-Zip or similar)
   - Find `chrome_elf.dll` or FFmpeg-related DLLs
   - Copy to `vendor/cef3/cef/Release/`

**Installation:**
1. Download `libffmpeg.dll` or `ffmpeg.dll` from sources above
2. Place it in `vendor/cef3/cef/Release/`
3. Rebuild MTA:SA - the build system will automatically copy it
4. YouTube live streams should now work

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

### Option 1: Download Pre-built libffmpeg.dll (Easiest)

**Note**: Modern CEF builds (90+) from Spotify often integrate FFmpeg into `libcef.dll` itself, so a separate `libffmpeg.dll` may not be included. Use these alternative sources instead:

#### **Working Sources for libffmpeg.dll:**

1. **GitHub - Moonlight/CEF Builds**:
   - URL: https://github.com/topics/cef-binary
   - Search for repositories providing CEF with proprietary codecs
   - Look for "proprietary" or "ffmpeg" in release names
   
2. **RaMMicHaeL's CEF Builds** (Community maintained):
   - Search GitHub for "cef proprietary codecs windows"
   - Many community members provide pre-built FFmpeg DLLs
   
3. **Extract from Chromium/Chrome**:
   - Download Chromium or Google Chrome standalone installer
   - Use 7-Zip or similar to extract without installing
   - Locate FFmpeg-related DLL (may be in different locations depending on version)
   - Copy to `vendor/cef3/cef/Release/`

4. **CEF Sharp or CefGlue Projects**:
   - These .NET wrapper projects often bundle CEF with proprietary codecs
   - Check their releases for pre-built binaries
   - Example: https://github.com/cefsharp/CefSharp/releases
   - Extract `libffmpeg.dll` from their packages

#### **Important Notes:**
- **CEF 90+** (newer): FFmpeg is often integrated into `libcef.dll`. You may need to replace the entire CEF build with a proprietary-codecs variant rather than adding a separate DLL.
- **CEF 75-89**: Usually has separate `libffmpeg.dll` in the Release folder
- **CEF 74 and older**: Uses `ffmpeg.dll`
- **Version Matching**: Ensure the FFmpeg DLL version matches your CEF version for compatibility

### Option 2: Build from Source (For Minimal Size)
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

## Troubleshooting

### "libffmpeg.dll not found in CEF download"
- **Newer CEF versions (90+)**: FFmpeg is integrated into `libcef.dll`. The separate DLL may not exist.
- **Solution**: Use one of these approaches:
  1. Find a community build that provides a separate `libffmpeg.dll`
  2. Replace your entire CEF installation with one built with proprietary codecs
  3. Extract FFmpeg DLL from Chrome/Chromium browser installation

### "YouTube still shows 'your browser can't play this video'"
- Ensure the FFmpeg DLL version matches your CEF version
- Check that the DLL is actually being copied to the output directory
- Verify the DLL is ~1-3 MB (if much smaller, it may not include proprietary codecs)
- Try different sources for the FFmpeg DLL

### "Which file do I need?"
- **CEF 90+**: `libffmpeg.dll` (or FFmpeg is in `libcef.dll`)
- **CEF 75-89**: `libffmpeg.dll`
- **CEF 74 and older**: `ffmpeg.dll`
- When in doubt, try placing any of these files and rebuild

## Notes
- The FFmpeg library with proprietary codecs cannot be included in this repository due to licensing and size considerations
- File size should be kept under 5 MB to avoid bloat
- Ensure the FFmpeg version matches your CEF version for compatibility
