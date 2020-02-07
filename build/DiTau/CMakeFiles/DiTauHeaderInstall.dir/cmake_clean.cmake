file(REMOVE_RECURSE
  "../x86_64-centos7-gcc62-opt/include/DiTau"
  "../x86_64-centos7-gcc62-opt/python/DiTau/__init__.py"
  "../x86_64-centos7-gcc62-opt/jobOptions/DiTau/DiTauAlgJobOptions.py"
  "../x86_64-centos7-gcc62-opt/include/DiTau"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/DiTauHeaderInstall.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
