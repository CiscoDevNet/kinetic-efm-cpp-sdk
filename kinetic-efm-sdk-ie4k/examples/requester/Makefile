CFLAGS = -std=c++11 -Wall -Wextra -I ../../include -g -O2 -D_FORTIFY_SOURCE=2 -fPIE -fstack-protector
LDFLAGS = -L ../../lib -pie -Wl,-z,now
LIBS = -lboost_log -lboost_date_time -lboost_program_options -lboost_system -lboost_thread -lboost_filesystem -lboost_regex -lssl -lcrypto -ldl -pthread

.PHONY: all clean
all: requester requester-shared

DEPS = requester_link.h
OBJ = main.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

requester: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS) -ldslink-sdk-cpp-static $(LIBS)

requester-shared: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS) -ldslink-sdk-cpp $(LIBS)

run: requester
	./requester

run-shared: requester-shared create-libcrypto-symlink
	LD_LIBRARY_PATH=../../lib ./requester-shared

clean:
	$(RM) requester requester-shared $(OBJ)
