cmake_minimum_required(VERSION 3.31)

set(PROJECT_NAME Hengine)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)

include_directories(third-party/glfw-3.4/include)
include_directories(third-party/glew-2.1.0/include)
include_directories(third-party/glm)

link_directories(third-party/glfw-3.4/lib-mingw-w64)
link_directories(third-party/glew-2.1.0/lib/Release/x64)

add_executable(${PROJECT_NAME} main.cpp
        engine/render/shader/Shader.cpp
        engine/render/shader/Shader.h
        engine/render/BufferBuilder.cpp
        engine/render/BufferBuilder.h
        engine/render/FrameBuffer.cpp
        engine/render/FrameBuffer.h
        engine/Core.cpp
        engine/Core.h
        engine/scene/Scene.cpp
        engine/scene/Scene.h
        engine/scene/SceneManager.cpp
        engine/scene/SceneManager.h
        engine/scene/event/Event.h
        engine/scene/event/EventListener.h
        engine/scene/module/Module.h
        engine/scene/GameObject.h
        engine/scene/event/SceneEvents.h
        engine/scene/module/impl/scene/RenderModule.h
        engine/scene/module/impl/GameObjectModule.h
        engine/scene/module/impl/SceneModule.h
        engine/scene/module/ModuleContainer.h
        engine/Window.cpp
        engine/Window.h
        engine/scene/module/impl/scene/RenderModule.cpp
        engine/scene/Camera.h
        engine/scene/Camera.cpp
        engine/render/shader/VertexSources.h
        engine/render/shader/ShaderPrograms.h
        engine/utils/type_utils.h
        engine/scene/module/impl/scene/post_processing.h
)

target_link_libraries(${PROJECT_NAME} glfw3 glew32 opengl32)