# ===========================================================================
#  Copyright © 2014  CS -  All rights reserved.
# ===========================================================================

NAME   := dashboard-demo
CXX    := g++
CFLAGS := -std=gnu++11 -O2 -Wall -Wextra -g -I ../lib
# EC LIBS   := -lEGL -lGLESv1_CM -lX11 -lpng -lm -lpthread
LIBS   := -lEGL -lGL -lX11 -lpng -lm -lpthread

all:
	$(CXX) $(CFLAGS) ./*.cpp ../lib/*.cpp -o $(NAME).elf $(LIBS)

clean:
	@ rm -fv ./$(NAME).elf
