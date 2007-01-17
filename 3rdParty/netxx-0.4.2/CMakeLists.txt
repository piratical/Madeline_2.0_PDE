
SET( SOURCES
	
	address.cpp 
	osutil.cpp 
	peer.cpp 
	probe.cpp 
	probe_select.cpp 
	resolve_getaddrinfo.cpp 
	sockaddr.cpp 
	socket.cpp 
	sockopt.cpp 
	streambase.cpp 
	stream.cpp 
	tls_certificate.cpp 
	tls_context.cpp 
	tls_stream.cpp 
	tls_tls_pimpl.cpp
	
)
ADD_LIBRARY(netxx STATIC ${SOURCES} )

