## automatically generated CMakeLists.txt file

# Declare the package
atlas_subdir( DiTau )

# Declare external dependencies ... default here is to include ROOT
find_package( ROOT COMPONENTS MathCore RIO Core Tree Hist )

# Declare package as a library
# Note the convention that library names get "Lib" suffix
# Any package you depend on you should add
# to LINK_LIBRARIES line below (see the example)
atlas_add_library( DiTauLib src/*.cxx
                   PUBLIC_HEADERS DiTau
                   INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
                   LINK_LIBRARIES ${ROOT_LIBRARIES}
                                    AthAnalysisBaseCompsLib AsgTools xAODEventInfo DiTauMassTools AssociationUtils xAODTau xAODEgamma xAODMuon xAODJet xAODMissingET METInterface AthContainers PATInterfaces TauAnalysisTools 
)

# if you add athena components (tools, algorithms) to this package
# these lines are needed so you can configure them in joboptions
atlas_add_component( DiTau src/components/*.cxx
                      NOCLIDDB
                      LINK_LIBRARIES DiTauLib 
)

# if you add an application (exe) to this package
# declare it like this (note convention that apps go in the util dir)
# atlas_add_executable( MyApp util/myApp.cxx
#                       LINK_LIBRARIES DiTauLib
# )

# Install python modules, joboptions, and share content
atlas_install_python_modules( python/*.py )
atlas_install_joboptions( share/*.py )
atlas_install_data( data/* )
# You can access your data from code using path resolver, e.g.
# PathResolverFindCalibFile("DiTau/file.txt")



