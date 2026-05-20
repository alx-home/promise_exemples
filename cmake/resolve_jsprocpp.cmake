function(resolve_jsprocpp_dependency)
   message(STATUS "[promise_examples] Resolving dependency: alx-home::jsprocpp")

   # Standard flow: try package manager first, then fallback to FetchContent.
   find_package(jsprocpp CONFIG QUIET)

   if(TARGET alx-home::jsprocpp)
      message(STATUS "[promise_examples] Found alx-home::jsprocpp via package manager")
   endif()

   # If the package isn't available, fall back to fetching from GitHub.
   if(NOT TARGET alx-home::jsprocpp)
      include(FetchContent)
      set(FETCHCONTENT_QUIET OFF)

      set(JSPROCPP_GIT_REPOSITORY "https://github.com/alx-home/JSProCpp.git" CACHE STRING "alx-home/JSProCpp Git repository")
      set(JSPROCPP_GIT_TAG "release" CACHE STRING "alx-home/JSProCpp Git tag, branch, or SHA")

      message(STATUS "[promise_examples] alx-home::jsprocpp package not found; using FetchContent")
      message(STATUS "[promise_examples] Fetching ${JSPROCPP_GIT_REPOSITORY} @ ${JSPROCPP_GIT_TAG}")

      FetchContent_Declare(jsprocpp
            GIT_REPOSITORY "${JSPROCPP_GIT_REPOSITORY}"
            GIT_TAG "${JSPROCPP_GIT_TAG}"
            GIT_SHALLOW TRUE
      )
      FetchContent_MakeAvailable(jsprocpp)

      if(NOT TARGET alx-home::jsprocpp)
         message(FATAL_ERROR "FetchContent downloaded alx-home/jsprocpp but no compatible CMake target was exported. Expected alx-home::jsprocpp, JSProCpp, or jsprocpp.")
      else()
         message(STATUS "[promise_examples] Using alx-home::jsprocpp from FetchContent")
      endif()
   endif()

   if(NOT TARGET alx-home::jsprocpp)
      message(FATAL_ERROR "alx-home::jsprocpp target not found after package and FetchContent resolution.")
   endif()
endfunction()
