# templateModule depends on cyclops
if(TARGET cyclops)
	include_directories(${OSG_INCLUDES})
	
	add_library(templateModule MODULE 
	templateModule.cpp
	Connection.cpp
	Thread.cpp
	MatlabGeometry.cpp
	DataReader.cpp
	GeometryQueue.cpp
	)
	


	# Set the module library dependencies here
	target_link_libraries(templateModule
		omega 
		omegaToolkit 
		cyclops
		)

	declare_native_module(templateModule)
endif()
