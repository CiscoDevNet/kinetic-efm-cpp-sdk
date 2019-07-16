CFLAGS = -std=c++11 -Wall -Wextra -I ../../include -g -O2 -D_FORTIFY_SOURCE=2 -fPIE -fstack-protector
LDFLAGS = -L ../../lib -pie -Wl,-z,now
# -latomic is only required on some platforms
LIBS = -lboost_log -lboost_date_time -lboost_program_options -lboost_system -lboost_thread -lboost_filesystem -lboost_regex -lssl -lcrypto -ldl -lpthread -latomic

LINK_NAME=simple-responder
VERSION=1.2.3
DESCRIPTION="Simple Responder Link Example"

.PHONY: all clean
all: simple-responder simple-responder-shared

DEPS = error_code.h
OBJ = error_code.o main.o

%.o: %.cpp $(DEPS)
	$(CXX) -DLINK_VERSION="\"$(VERSION)\"" -c -o $@ $< $(CFLAGS)

simple-responder: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS) -ldslink-sdk-cpp-static $(LIBS)

simple-responder-shared: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS) -ldslink-sdk-cpp $(LIBS)

run: simple-responder
	./simple-responder

run-shared: simple-responder-shared
	LD_LIBRARY_PATH=../../lib ./simple-responder-shared

package: simple-responder
	../../packaging/package_link.sh $(LINK_NAME) ./simple-responder $(VERSION) $(DESCRIPTION)

package-shared: simple-responder-shared
	../../packaging/package_link.sh $(LINK_NAME) ./simple-responder-shared $(VERSION) $(DESCRIPTION) ../../lib/*.so.*

clean:
	$(RM) simple-responder simple-responder-shared $(OBJ) *.zip

