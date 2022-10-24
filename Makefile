HEADERS = Menu.h

sfml-app: main.o Menu.o
	g++ main.o Menu.o -o sfml-app -L"C:\SFML-2.5.1\lib" -lsfml-graphics -lsfml-window -lsfml-system
	./sfml-app
	
main.o : Menu.cpp $(HEADERS)
	gcc -c main.cpp Menu.cpp -I"C:\SFML-2.5.1\include"

clean:
	rm -rf main.o Menu.o
	rm -rf sfml-app.exe



	
	