# EUI-NEO on OpenHarmony

This Stage-model application links EUI-NEO and the OHOS-enabled SDL3 tree into
`libentry.so`. The ArkTS entry ability is provided by `@ohos-rs/ability`, which
owns the XComponent and forwards its lifecycle and platform services to SDL.

The development layout expects `EUI-NEO`, `SDL`, and `openharmony-ability` to
be sibling directories. Check out the ability repository's
`feat/native-platform-services` branch, then override
`SDL3_SOURCE_DIR` in the native build arguments and update the local HAR path
when using another layout.

The SDL3 runner keeps the shared desktop Gallery and applies the same mobile
policy as the Android runner: it reads SDL's display content scale, caps the
example UI scale at 2x, and translates standard finger events into taps,
vertical scrolling and long-press context input. Synthetic touch-to-mouse
events are disabled to avoid duplicate delivery. The OpenGL backend restores
the complete OHOS backbuffer from EUI's render cache before each present,
because the EGL swapchain does not guarantee preserved backbuffer contents.

Build from this directory:

```sh
ohpm install
hvigorw --mode module -p product=default -p module=entry@default -p buildMode=debug assembleHap
```
