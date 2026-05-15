function(resolve_alx_promise_dependency)
   message(STATUS "[promise_examples] Resolving dependency: alx-home::promise")

   # Standard flow: try package manager first, then fallback to FetchContent.
   find_package(alx-promise CONFIG QUIET)

   if(TARGET alx-home::promise)
      message(STATUS "[promise_examples] Found alx-home::promise via package manager")
   endif()

   # If the package isn't available, fall back to fetching from GitHub.
   if(NOT TARGET alx-home::promise)
      include(FetchContent)
      set(FETCHCONTENT_QUIET OFF)

      set(ALX_PROMISE_GIT_REPOSITORY "https://github.com/alx-home/promise.git" CACHE STRING "alx-home/promise Git repository")
      set(ALX_PROMISE_GIT_TAG "release" CACHE STRING "alx-home/promise Git tag, branch, or SHA")

      message(STATUS "[promise_examples] alx-promise package not found; using FetchContent")
      message(STATUS "[promise_examples] Fetching ${ALX_PROMISE_GIT_REPOSITORY} @ ${ALX_PROMISE_GIT_TAG}")

      FetchContent_Declare(alx_promise
            GIT_REPOSITORY "${ALX_PROMISE_GIT_REPOSITORY}"
            GIT_TAG "${ALX_PROMISE_GIT_TAG}"
            GIT_SHALLOW TRUE
      )
      FetchContent_MakeAvailable(alx_promise)

      if(NOT TARGET alx-home::promise)
         message(FATAL_ERROR "FetchContent downloaded alx-home/promise but no compatible CMake target was exported. Expected alx-home::promise, promise, or alx_promise.")
      else()
         message(STATUS "[promise_examples] Using alx-home::promise from FetchContent")
      endif()
   endif()

   if(NOT TARGET alx-home::promise)
      message(FATAL_ERROR "alx-home::promise target not found after package and FetchContent resolution.")
   endif()
endfunction()
