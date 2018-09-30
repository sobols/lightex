configure_file(lightex.sty lightex.sty COPYONLY)

add_custom_command(
	OUTPUT resource.cc
	COMMAND rescompiler ARGS resource.cc lightex.sty "LIGHTEX_STY"
	DEPENDS lightex.sty
)

add_library(styles STATIC resource.cc)
