cmake_minimum_required(VERSION 3.15) # Actualizado por el uso de list(POP_FRONT)
project(GPU C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-pedantic -Wall -Wextra)

# SDL3
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3 REQUIRED sdl3)

# Importar glad.o (objeto precompilado)
set(GLAD_OBJECT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/glad.o")
add_library(glad OBJECT IMPORTED
        src/Camera/camera.h
        src/Camera/camera.c
        src/App/App.h)
set_target_properties(glad PROPERTIES IMPORTED_OBJECTS "${GLAD_OBJECT_PATH}")
target_include_directories(glad INTERFACE external/glad/include)

# Buscar TODOS los .c recursivamente, excluyendo los que están en build/, external/ o CMakeFiles/
file(GLOB_RECURSE ALL_C_FILES "*.c")
list(FILTER ALL_C_FILES EXCLUDE REGEX ".*/(build|external|CMakeFiles)/.*")
add_executable(GPU ${ALL_C_FILES}
        src/App/App.c
        src/App/App.h
        src/App/WinCfg.h
        src/App/WinCfg.c
        src/Shader/Shader.h
        src/Shader/Shader.c)

# --- Incluir TODOS los subdirectorios recursivamente (excepto los excluidos) ---
set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})  # La raíz siempre incluida

# Lista de directorios a procesar (empezamos con la raíz)
set(dirs_to_process ${CMAKE_CURRENT_SOURCE_DIR})
# Mientras haya directorios por procesar
while(dirs_to_process)
    # Tomar el primer directorio de la lista
    list(POP_FRONT dirs_to_process current_dir)
    # Buscar sus subdirectorios
    file(GLOB subdirs LIST_DIRECTORIES true RELATIVE ${current_dir} ${current_dir}/*)
    foreach(subdir ${subdirs})
        set(full_path ${current_dir}/${subdir})
        # Si es un directorio y no está en la lista de exclusión
        if(IS_DIRECTORY ${full_path} AND NOT subdir MATCHES "^(build|external|cmake-build-debug|out|bin|lib|CMakeFiles)$")
            list(APPEND INCLUDE_DIRS ${full_path})
            list(APPEND dirs_to_process ${full_path})  # Seguir explorando
        endif()
    endforeach()
endwhile()

# --- Configuración explícita de Includes Externos ---
list(APPEND INCLUDE_DIRS ${SDL3_INCLUDE_DIRS})

# Aseguramos de que el compilador encuentre <cglm/cglm.h> sin importar cómo esté estructurada la carpeta
list(APPEND INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/external")
# Aplicar las rutas de inclusión al target
target_include_directories(GPU PRIVATE ${INCLUDE_DIRS})

# Enlazar bibliotecas
target_link_libraries(GPU PRIVATE
    glad
    ${SDL3_LIBRARIES}
    -lGL
    -lm
    -ldl
)