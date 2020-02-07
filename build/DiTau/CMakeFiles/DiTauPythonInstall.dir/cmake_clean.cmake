file(REMOVE_RECURSE
  "../x86_64-centos7-gcc62-opt/include/DiTau"
  "../x86_64-centos7-gcc62-opt/python/DiTau/__init__.py"
  "../x86_64-centos7-gcc62-opt/jobOptions/DiTau/DiTauAlgJobOptions.py"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/DiTauPythonInstall.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
