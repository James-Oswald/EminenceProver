
#Allows us to use #include<vampire/...> from our sources
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#[[

#TODO Have yet to get building from source to work

message("[Eminence Prover] Configuring Z3")
add_subdirectory(vampire/Z3)

message("[Eminence Prover] Configuring Vampire")
find_package(Z3 CONFIG REQUIRED) #This is a hack to foce Vampire to find Z3
add_subdirectory(vampire)
]]

message("Downloading Vampire")
file(DOWNLOAD ${CMAKE_CURRENT_BINARY_DIR}/vampire4.7.zip)
message("Downloading Vampire")

