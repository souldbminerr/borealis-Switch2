if (NOT DEFINED SWITCH_ROMFS_DIR)
    message(FATAL_ERROR "SWITCH_ROMFS_DIR is required")
endif ()

file(GLOB _switch2_keymap_fonts "${SWITCH_ROMFS_DIR}/font/keymap_*.ttf")
file(GLOB _switch2_fallback_fonts "${SWITCH_ROMFS_DIR}/font/*_fallback.ttf")

set(_switch2_fonts_to_remove ${_switch2_keymap_fonts} ${_switch2_fallback_fonts})
if (_switch2_fonts_to_remove)
    file(REMOVE ${_switch2_fonts_to_remove})
endif ()
