# templateModule depends on cyclops
if(TARGET cyclops)
	include_directories(${OSG_INCLUDES})
	
	add_library(templateModule MODULE 
	src/templateModule.cpp
	src/Connection.cpp
	src/Thread.cpp
	src/MatlabGeometry.cpp
	src/DataReader.cpp
	src/GeometryQueue.cpp
	)
	


	# Set the module library dependencies here
	target_link_libraries(templateModule
		omega 
		omegaToolkit 
		cyclops
		)

	declare_native_module(templateModule)
endif()
