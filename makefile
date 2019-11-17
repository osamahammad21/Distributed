init_message:
	@g++ -std=c++11 base64.h base64.cpp Message.h Message.cpp message_test.cpp -o message
init_server:
	@g++ -std=c++11 -pthread base64.h base64.cpp Message.h UDPSocket.h UDPServerSocket.h Server.h main_server.cpp -o server
init_client:
	@g++ -std=c++11 -pthread base64.h base64.cpp Message.h UDPSocket.h UDPClientSocket.h Client.h message_test.cpp -o client
init_UDPSocket:
	@g++ -std=c++11 -pthread base64.h base64.cpp Message.h UDPSocket.h  UDPSocket_test.cpp -o socktest

