all:server.cpp client.cpp
	g++ util.cpp server.cpp Epoll.cpp InetAddress.cpp Socket.cpp -g -o server && \
	g++ util.cpp client.cpp -g -o client

clean:
	rm server && rm client