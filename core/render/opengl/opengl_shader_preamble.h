#pragma once

#if defined(__OHOS__)
#define EUI_OPENGL_VERTEX_PREAMBLE "#version 300 es\n"
#define EUI_OPENGL_FRAGMENT_PREAMBLE \
    "#version 300 es\n"               \
    "precision highp float;\n"        \
    "precision highp int;\n"
#else
#define EUI_OPENGL_VERTEX_PREAMBLE "#version 330 core\n"
#define EUI_OPENGL_FRAGMENT_PREAMBLE "#version 330 core\n"
#endif
