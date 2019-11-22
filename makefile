init_peer_requester:
	@g++ -pthread base64.h Image.h Message.h UDPSocket.h Peer.h main_peer_requester.cpp -o requester
init_peer_owner:
	@g++ -pthread base64.h Image.h Message.h UDPSocket.h Peer.h main_peer_owner.cpp -o owner
init_ds:
	@g++ -pthread base64.h base64.cpp Message.h Message.cpp UDPSocket.h UDPSocket.cpp rapidcsv.h directoryServer.h directoryServer.cpp main_ds.cpp -o ds
init_ImageTest:
	@g++ -pthread base64.h Image.h imagetest.cpp -o image
