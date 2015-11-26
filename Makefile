objs = out/server.cpp.o out/httprequest.cpp.o out/framework.cpp.o out/main.cpp.o out/module.cpp.o out/httpresponse.cpp.o out/defaultmodule.cpp.o 

CXX = g++
AR = ar
CXXFLAGS = -Wall -Wno-unused-local-typedefs -O2 -I./ -std=c++11

minix.out: out $(objs)
	$(CXX) -o minix.out $(objs) -likkcpr -lpthread -lpcre

out:
	mkdir out

out/server.cpp.o: minix/server.cpp minix/server.h minix/module.h minix/framework.h minix/httprequest.h
	$(CXX) $(CXXFLAGS) -c minix/server.cpp -o out/server.cpp.o

out/httprequest.cpp.o: minix/httprequest.cpp minix/httprequest.h
	$(CXX) $(CXXFLAGS) -c minix/httprequest.cpp -o out/httprequest.cpp.o

out/framework.cpp.o: minix/framework.cpp minix/framework.h minix/server.h
	$(CXX) $(CXXFLAGS) -c minix/framework.cpp -o out/framework.cpp.o

out/main.cpp.o: minix/main.cpp minix/framework.h minix/server.h
	$(CXX) $(CXXFLAGS) -c minix/main.cpp -o out/main.cpp.o

out/module.cpp.o: minix/module.cpp minix/defaultmodule.h minix/module.h minix/framework.h minix/server.h
	$(CXX) $(CXXFLAGS) -c minix/module.cpp -o out/module.cpp.o

out/httpresponse.cpp.o: minix/httpresponse.cpp minix/httpresponse.h
	$(CXX) $(CXXFLAGS) -c minix/httpresponse.cpp -o out/httpresponse.cpp.o

out/defaultmodule.cpp.o: minix/defaultmodule.cpp minix/defaultmodule.h minix/httpresponse.h minix/httprequest.h minix/module.h
	$(CXX) $(CXXFLAGS) -c minix/defaultmodule.cpp -o out/defaultmodule.cpp.o

