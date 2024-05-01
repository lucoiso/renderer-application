# Author: Lucas Vilas-Boas
# Year: 2024
# Repo: https://github.com/lucoiso/vulkan-renderer

# ----------- Global Definitions -----------
SET(LIBRARY_NAME Application)

# ------------- Library Setup --------------
ADD_EXECUTABLE(${LIBRARY_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/Source/Private/Main.cpp)
SET_TARGET_PROPERTIES(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)

TARGET_LINK_LIBRARIES(${LIBRARY_NAME} PRIVATE
        UserInterface
)

ADD_CUSTOM_COMMAND(TARGET ${LIBRARY_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${LIBRARY_NAME}> $<TARGET_FILE_DIR:${LIBRARY_NAME}>
        COMMAND_EXPAND_LISTS
)