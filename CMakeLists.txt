set(FLUFFYTW
	${CMAKE_CURRENT_SOURCE_DIR}/f_helper.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/f_helper.h
	${CMAKE_CURRENT_SOURCE_DIR}/f_component.h
	${CMAKE_CURRENT_SOURCE_DIR}/f_config.h

	${CMAKE_CURRENT_SOURCE_DIR}/f_bots.h
	${CMAKE_CURRENT_SOURCE_DIR}/f_bots.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/aimbot/aimbot.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/aimbot/aimbot_scans.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/aimbot/aimbot.h

	${CMAKE_CURRENT_SOURCE_DIR}/f_visuals.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/f_visuals-base.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/f_visuals.h
)

set(GAME_CLIENT
	${GAME_CLIENT}
	${FLUFFYTW}
	PARENT_SCOPE
)