function(
  nanobind_pyarrow_enable_sanitizers
  project_name
  ENABLE_SANITIZER_ADDRESS
  ENABLE_SANITIZER_LEAK
  ENABLE_SANITIZER_UNDEFINED_BEHAVIOR
  ENABLE_SANITIZER_THREAD
  ENABLE_SANITIZER_MEMORY)

  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    set(SANITIZERS "")

    if(${ENABLE_SANITIZER_ADDRESS})
      list(APPEND SANITIZERS "address")
      if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
        execute_process(
          COMMAND ${CMAKE_C_COMPILER} -print-file-name=libclang_rt.asan.so
          OUTPUT_VARIABLE LIBASAN_PATH
          OUTPUT_STRIP_TRAILING_WHITESPACE
          RESULT_VARIABLE STATUS)
        if(STATUS
           AND NOT
               STATUS
               EQUAL
               0)
          message(FATAL_ERROR "Could not find location for libclang_rt.asan.so: ${STATUS}")
        else()
          set(LIBASAN_LOCATION ${LIBASAN_PATH})
        endif()
      else()
        set(__LOCATE_LIBASAN_CODE "int main() {return 0;}")
        try_compile(
          STATUS SOURCE_FROM_VAR
          locateLibAsan.cpp __LOCATE_LIBASAN_CODE
          OUTPUT_VARIABLE __CMAKE_C_COMPILER_OUTPUT
          LINK_OPTIONS "-fsanitize=address;-fPIC;-shared"
          COPY_FILE ${CMAKE_CURRENT_BINARY_DIR}/locate_libasan NO_LOG)
        if(NOT STATUS)
          message(FATAL_ERROR "Could not find location for libasan.so: ${__CMAKE_C_COMPILER_OUTPUT}")
        else()
          execute_process(
            COMMAND env -u LD_PRELOAD ldd ${CMAKE_CURRENT_BINARY_DIR}/locate_libasan
            OUTPUT_VARIABLE LDD_OUTPUT
            OUTPUT_STRIP_TRAILING_WHITESPACE
            RESULT_VARIABLE STATUS)
          if(STATUS
             AND NOT
                 STATUS
                 EQUAL
                 0)
            message(FATAL_ERROR "Could not find location for libasan.so: ${STATUS}")
          else()
            string(
              REGEX
              REPLACE ".*libasan\.so(\.[0-9]+) => ([a-zA-Z0-9\/\._-]+).*"
                      "\\2"
                      LIBASAN_LOCATION
                      ${LDD_OUTPUT})
          endif()
        endif()
      endif()
      message(STATUS "Found libasan.so: ${LIBASAN_LOCATION}")
      set(LIBASAN_LOCATION
          "${LIBASAN_LOCATION}"
          PARENT_SCOPE)
      set(ADDIOTIONAL_ENVIRONMENT_SETTINGS
          "LD_PRELOAD=${LIBASAN_LOCATION}"
          PARENT_SCOPE)
    endif()

    if(${ENABLE_SANITIZER_LEAK})
      list(APPEND SANITIZERS "leak")
    endif()

    if(${ENABLE_SANITIZER_UNDEFINED_BEHAVIOR})
      list(APPEND SANITIZERS "undefined")
    endif()

    if(${ENABLE_SANITIZER_THREAD})
      if("address" IN_LIST SANITIZERS OR "leak" IN_LIST SANITIZERS)
        message(WARNING "Thread sanitizer does not work with Address and Leak sanitizer enabled")
      else()
        list(APPEND SANITIZERS "thread")
      endif()
    endif()

    if(${ENABLE_SANITIZER_MEMORY} AND CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
      message(
        WARNING
          "Memory sanitizer requires all the code (including libc++) to be MSan-instrumented otherwise it reports false positives"
      )
      if("address" IN_LIST SANITIZERS
         OR "thread" IN_LIST SANITIZERS
         OR "leak" IN_LIST SANITIZERS)
        message(WARNING "Memory sanitizer does not work with Address, Thread or Leak sanitizer enabled")
      else()
        list(APPEND SANITIZERS "memory")
      endif()
    endif()
  elseif(MSVC)
    if(${ENABLE_SANITIZER_ADDRESS})
      list(APPEND SANITIZERS "address")
    endif()
    if(${ENABLE_SANITIZER_LEAK}
       OR ${ENABLE_SANITIZER_UNDEFINED_BEHAVIOR}
       OR ${ENABLE_SANITIZER_THREAD}
       OR ${ENABLE_SANITIZER_MEMORY})
      message(WARNING "MSVC only supports address sanitizer")
    endif()
  endif()

  list(
    JOIN
    SANITIZERS
    ","
    LIST_OF_SANITIZERS)

  if(LIST_OF_SANITIZERS)
    if(NOT
       "${LIST_OF_SANITIZERS}"
       STREQUAL
       "")
      message(STATUS "Enabling sanitizers: ${LIST_OF_SANITIZERS}")
      if(NOT MSVC)
        target_compile_options(${project_name} INTERFACE -fsanitize=${LIST_OF_SANITIZERS})
        target_link_options(${project_name} INTERFACE -fsanitize=${LIST_OF_SANITIZERS})
      else()
        string(FIND "$ENV{PATH}" "$ENV{VSINSTALLDIR}" index_of_vs_install_dir)
        if("${index_of_vs_install_dir}" STREQUAL "-1")
          message(
            SEND_ERROR
              "Using MSVC sanitizers requires setting the MSVC environment before building the project. Please manually open the MSVC command prompt and rebuild the project."
          )
        endif()
        target_compile_options(${project_name} INTERFACE /fsanitize=${LIST_OF_SANITIZERS} /Zi /INCREMENTAL:NO)
        target_compile_definitions(${project_name} INTERFACE _DISABLE_VECTOR_ANNOTATION _DISABLE_STRING_ANNOTATION)
        target_link_options(${project_name} INTERFACE /INCREMENTAL:NO)
      endif()
    endif()
  endif()

endfunction()
