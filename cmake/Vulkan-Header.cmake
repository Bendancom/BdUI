include(FetchContent)
FetchContent_Declear(
	vulkan-header
	GIT_REPOSITORY  ${VULKAN_HEADER_GIT_URL}
	GIT_TAG         ${VULKAN_HEADER_GIT_TAG}
	SOURCE_DIR		${PROJECT_SOURCE_DIR}/vulkan/vulkan-header
)
FetchContent_MakeAvailable(vulkan-header)