init_peer_requester:
	@g++ -pthread base64.h Message.h UDPSocket.h Peer.h main_peer_requester.cpp -o requester
init_peer_owner:
	@g++ -pthread base64.h Message.h UDPSocket.h Peer.h main_peer_owner.cpp -o owner
init_ds:
	@g++ -pthread base64.h Message.h UDPSocket.h directoryServer.h main_ds.cpp -o ds