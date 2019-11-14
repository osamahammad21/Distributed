init_message:
	@g++ -std=c++11 base64.h base64.cpp Message.h Message.cpp message_test.cpp -o message
init_server:
	@g++ -std=c++11 -pthread base64.h base64.cpp Message.h Message.cpp UDPSocket.h UDPServerSocket.h Server.h main_server.cpp -o server
makemakinit_client:
	@g++ -std=c++11 -pthread base64.h base64.cpp Message.h Message.cpp UDPSocket.h UDPClientSocket.h Client.h main_client.cpp -o client

