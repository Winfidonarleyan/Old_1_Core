# Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  Custom/Unell_VIP_System.cpp
  Custom/Unell_Announcer_System.cpp
  Custom/Unell_Login_System.cpp
  Custom/Unell_Chat_System.cpp
  Custom/Unell_Custom.cpp
  Custom/Unell_Custom_NPC.cpp
  Custom/Unell_Event.cpp
  Custom/Unell_Area_System.cpp
  Custom/Unell_PvP_System.cpp
  Custom/Unell_Referer_System.cpp
  Custom/Unell_Script.cpp
  Custom/Unell_Script.h
  Custom/Unell_Guild_System.cpp
  Custom/Unell_Top_System.cpp
)

message("  -> Подготовленно: Кастом скрипты")
