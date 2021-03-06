init_peer_requester:
	@g++ -std=c++11 -pthread base64.h base64.cpp Image.h Image.cpp Message.h Message.cpp UDPSocket.h UDPSocket.cpp Peer.h Peer.cpp main_peer_requester.cpp -o requester
init_peer_owner:
	@g++ -std=c++11 -pthread base64.h base64.cpp Image.h Image.cpp Message.h Message.cpp UDPSocket.h UDPSocket.cpp Peer.h Peer.cpp -o owner
init_ds:
	@g++ -std=c++11 -pthread base64.h base64.cpp Message.h Message.cpp UDPSocket.h UDPSocket.cpp directoryServer.h directoryServer.cpp main_ds.cpp -o ds
init_ImageTest:
	@g++ -std=c++11 -pthread base64.h base64.cpp Image.h Image.cpp imagetest.cpp -o image
socksender:
	@g++ -std=c++11 -pthread base64.h base64.cpp Message.h Message.cpp UDPSocket.h UDPSocket.cpp  image_send_test.cpp -o socksender
sockreceiver:
	@g++ -std=c++11 -pthread base64.h base64.cpp Message.h Message.cpp UDPSocket.h UDPSocket.cpp  image_receive_test.cpp -o sockreceiver


