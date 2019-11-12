# init_server:
# 	@g++ -pthread UDPSocket.h UDPServerSocket.h Server.h main_server.cpp -o server
# init_client:
# 	@g++ -pthread UDPSocket.h UDPClientSocket.h Client.h main_client.cpp -o client
init_message:
	@g++ Message.h Message.cpp message_test.cpp