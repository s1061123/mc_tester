
all: mc_client mc_server

mc_client: mc_client.c

mc_server: mc_server.c

clean:
	rm -f mc_server mc_client
