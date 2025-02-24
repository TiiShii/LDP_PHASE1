# Nom de l'exe
EXEC = projet_phase_1

# Fichiers source
SRC = phase1.cpp
OBJ = $(SRC:.cpp=.o)

# Options de compilation
CXX = g++

# compile le programme
$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJ)

# Compilation des fichiers .cpp en .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage des fichiers objets et exe
clean:
	rm -f $(OBJ) $(EXEC)

# Exécuter le programme après compilation
run: $(EXEC)
