# -------------- Directories ---------------
OPTION(VULKAN_RENDERER_ENABLE_IMGUI "Enable ImGui linkage, including support for custom viewport" ON)

ADD_SUBDIRECTORY(Application)
ADD_SUBDIRECTORY(vulkan-renderer)