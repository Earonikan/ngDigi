CXX = g++
TARGET = ngDigi
CXXFLAGS = -Wall -O2 -march=native -std=c++17 -Iinclude #-fsanitize=address

SRCDIR   = src
HEADDIR  = include
OBJDIR   = obj
BUILD	 = build

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(HEADDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS =  $(shell root-config --libs)
ROOTLIBS += -lRHTTP
ROOTLIBS += -lSpectrum
# ROOTLIBS += -lCAENDigitizer
# DEPLIBS	=  -lCAENDigitizer


# $(BUILD)/$(TARGET): $(OBJECTS)
$(TARGET): $(OBJECTS)
	@$(CXX) $(CXXFLAGS) $(ROOTFLAGS) -o $@ $(OBJECTS) $(ROOTLIBS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CXX) $(CXXFLAGS) $(ROOTFLAGS) -c $< -o $@ $(ROOTLIBS)
	@echo "Compiled "$<" successfully!"

clean:
	rm -rf $(OBJDIR)/*.o $(TARGET)

debug: CXXFLAGS += -g

debug: $(TARGET)
