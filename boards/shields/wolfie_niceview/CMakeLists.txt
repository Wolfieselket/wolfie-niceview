
if(CONFIG_ZMK_DISPLAY AND CONFIG_SHIELD_WOLFIE_NICEVIEW)
  zephyr_library_sources(custom_status_screen.c)
endif()
