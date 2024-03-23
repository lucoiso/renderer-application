# -------------- Directories ---------------
OPTION(VULKAN_RENDERER_ENABLE_IMGUI "Enable ImGui linkage, including support for custom viewport" OFF)

ADD_SUBDIRECTORY(Application)
ADD_SUBDIRECTORY(VulkanRenderer)