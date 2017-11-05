# ===========================================================================
#  Copyright © 2014  CS  -  All rights reserved.
# ===========================================================================

CXX    := g++
CFLAGS := -std=gnu++11 -O2 -Wall -Wextra -g -I ../lib
# EC LIBS   := -lEGL -lGLESv1_CM -lX11 -lpng -lm
LIBS   := -lEGL -lGL -lX11 -lpng -lm

all:
	$(CXX) $(CFLAGS) ./*.cpp ../lib/*.cpp -o dashboard.elf $(LIBS)

clean:
	rm -v ./dashboard.elf
