CXXFLAGS = `pkg-config --cflags webkit2gtk-4.0 gtk+-3.0`
LIBS = `pkg-config --libs webkit2gtk-4.0 gtk+-3.0`

all:
    g++ $(CXXFLAGS) main.cpp -o MyWebBrowser $(LIBS)
