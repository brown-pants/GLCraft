set(ThirdPartyFolder "3rdparty")

if(NOT TARGET glad)
	add_subdirectory(glad)
	set_target_properties(glad PROPERTIES FOLDER ${ThirdPartyFolder}/glad)
endif()

if(NOT TARGET glfw)
    option(GLFW_BUILD_EXAMPLES "" OFF)
    option(GLFW_BUILD_TESTS "" OFF)
    option(GLFW_BUILD_DOCS "" OFF)
    option(GLFW_INSTALL "" OFF)
    add_subdirectory(glfw)
    set_target_properties(glfw PROPERTIES FOLDER ${ThirdPartyFolder}/glfw)
    set_target_properties(update_mappings PROPERTIES FOLDER ${ThirdPartyFolder}/glfw)
endif()


