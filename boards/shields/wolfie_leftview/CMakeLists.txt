if(CONFIG_ZMK_DISPLAY AND CONFIG_SHIELD_WOLFIE_LEFTVIEW)
  zephyr_library_sources(custom_status_screen.c)
endif()
