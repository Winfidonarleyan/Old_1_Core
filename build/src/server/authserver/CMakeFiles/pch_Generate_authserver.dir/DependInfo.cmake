# The set of languages for which implicit dependencies are needed:
SET(CMAKE_DEPENDS_LANGUAGES
  )
# The set of files for implicit dependencies of each language:

# Preprocessor definitions for this target.
SET(CMAKE_TARGET_DEFINITIONS
  "SCRIPTS"
  )

# Targets to which this target links.
SET(CMAKE_TARGET_LINKED_INFO_FILES
  )

# The include file search paths:
SET(CMAKE_C_TARGET_INCLUDE_PATH
  "src/server/authserver"
  "."
  "../src/server/shared"
  "../src/server/shared/Database"
  "../src/server/shared/Debugging"
  "../src/server/shared/Packets"
  "../src/server/shared/Cryptography"
  "../src/server/shared/Cryptography/Authentication"
  "../src/server/shared/Logging"
  "../src/server/shared/Threading"
  "../src/server/shared/Utilities"
  "../src/server/authserver"
  "../src/server/authserver/Authentication"
  "../src/server/authserver/Realms"
  "../src/server/authserver/Server"
  "/usr/include/mysql"
  )
SET(CMAKE_CXX_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
SET(CMAKE_Fortran_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
SET(CMAKE_ASM_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
