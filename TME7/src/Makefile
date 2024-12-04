exo1 : pipe.cpp 
	g++ -o pipe pipe.cpp -I . -g -Wall -std=c++1y
exo2 : prod_cons.cpp Stack.h
	g++ -o prod_cons prod_cons.cpp Stack.h -I . -g -Wall -std=c++1y -pthread
serveur : 
	g++ -o serveur chat_serveur.cpp chat_common.h -I . -g -Wall -std=c++1y -pthread
client  : 
	g++ -o client chat_client.cpp chat_common.h -I . -g -Wall -std=c++1y -pthread
clean:
	rm -f pipe prod_cons serveur client
