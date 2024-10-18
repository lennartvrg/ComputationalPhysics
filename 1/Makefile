# Name der ausführbaren Datei
TARGET = main

# Alle C++-Dateien in den Variablen SRCS speichern
SRCS = main.cpp src/GenerateRandom.cpp #src/PlotHistogram.cpp #src/subclass2.cpp

# Kompilierungsregel
$(TARGET): $(SRCS)
	g++	-o	$(TARGET)	$(SRCS)
	@echo "Compiled! Run with: ./$(TARGET)"
    
# Clean Regel
clean:
	rm	-f	$(TARGET)
