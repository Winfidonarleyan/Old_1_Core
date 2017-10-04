# output generic information about the core and buildtype chosen

message("")
message("* UnellCore версия            : ${rev_id_str} (${rev_hash_str})")
if( UNIX )
  message("* Выбрана конфигурация        : ${CMAKE_BUILD_TYPE}")
endif()
message("")

# output information about installation-directories and locations

message("* Установка ядра в            : ${CMAKE_INSTALL_PREFIX}")
if( UNIX )
  message("* Установка библиотек в       : ${LIBSDIR}")
  message("* Установка конфигов в        : ${CONF_DIR}")
endif()
message("")

# Show infomation about the options selected during configuration

if( SERVERS )
  message("* Построение world/auth       : Да (стандартно)")
else()
  message("* Построение world/authserver : Нет")
endif()

if( SCRIPTS )
  message("* Построение with scripts     : Да (стандартно)")
  add_definitions(-DSCRIPTS)
else()
  message("* Построение with scripts     : Нет")
  set(USE_SCRIPTPCH 0)
endif()

if( TOOLS )
  message("* Построение map/vmap tools   : Да")
else()
  message("* Построение map/vmap tools   : Нет  (стандартно)")
endif()

if( USE_COREPCH )
  message("* Построение core w/PCH       : Да (стандартно)")
else()
  message("* Построение core w/PCH       : Нет")
endif()

if( USE_SCRIPTPCH )
  message("* Построение scripts w/PCH    : Да (стандартно)")
else()
  message("* Построение scripts w/PCH    : Нет")
endif()

if( WITH_WARNINGS )
  message("* Show all warnings      : Да")
else()
  message("* Show compile-warnings       : Нет  (стандартно)")
endif()

if( WITH_COREDEBUG )
  message("* Использование coreside debug     : Да")
  add_definitions(-DTRINITY_DEBUG)
else()
  message("* Использование дебага        : Нет  (стандартно)")
endif()

if( WIN32 )
  if( USE_MYSQL_SOURCES )
    message("* Использование MySQL sourcetree   : Да (стандартно)")
  else()
    message("* Использование MySQL sourcetree   : Нет")
  endif()
endif( WIN32 )

if ( NOJEM )
  message("")
  message("*** WARNING: jemalloc linking has been disabled!")
  message("*** Please note that this is for DEBUGGING WITH VALGRIND only!")
  message("*** DO NOT DISABLE IT UNLESS YOU KNOW WHAT YOU'RE DOING!")
endif()
message("")
