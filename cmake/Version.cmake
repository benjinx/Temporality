
INCLUDE(GetGitRevisionDescription)

# Get Git revision
GET_GIT_HEAD_REVISION(GIT_REFSPEC PROJECT_VERSION_GIT_HASH)
STRING(SUBSTRING "${PROJECT_VERSION_GIT_HASH}" 0 8 PROJECT_VERSION_GIT_HASH)

# Parse version from git tags
GIT_DESCRIBE(VERSION --tags)
IF(VERSION)
    STRING(REGEX REPLACE 
        "^v([0-9]+)\\..*" "\\1" 
        PROJECT_VERSION_MAJOR 
        "${VERSION}"
    )
    STRING(REGEX REPLACE 
        "^v[0-9]+\\.([0-9]+).*" "\\1" 
        PROJECT_VERSION_MINOR 
        "${VERSION}"
    )
    STRING(REGEX REPLACE 
        "^v[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" 
        PROJECT_VERSION_PATCH 
        "${VERSION}"
    )
ELSE()
    SET(PROJECT_VERSION_MAJOR 0)
    SET(PROJECT_VERSION_MINOR 0)
    SET(PROJECT_VERSION_PATCH 0)
    
    MESSAGE(WARNING "Failed to parse version from `git describe --tags`")
ENDIF()

SET(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
MESSAGE(STATUS "Temporality Version ${PROJECT_VERSION}")
