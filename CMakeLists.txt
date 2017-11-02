#--------------------------------------------------------
# The CMakeLists.txt for:                       pMux
# Author(s):                              
#--------------------------------------------------------

SET(SRC
  Mux.cpp
  Mux_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pMux ${SRC})

TARGET_LINK_LIBRARIES(pMux
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

