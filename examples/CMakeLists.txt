set(base_examples
    satellite-terminal-demo
    orientation-helper-demo
)

foreach(
  example
  ${base_examples}
)
  build_lib_example(
    NAME ${example}
    SOURCE_FILES ${example}.cc
    LIBRARIES_TO_LINK
    ${libmlxsat}
    ${libsatellite}
    ${libapplications}
    ${libpoint-to-point}
    ${libinternet}
    ${libnetwork}
  )
endforeach()
