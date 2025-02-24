#include <iostream> // Bibliothèque standard pour les flux d'entrée/sortie
#include <cstdint> // Assure la portabilité des types d'entiers avec une largeur et un signe spécifiques sur différents systèmes
#include <fstream> // Classe de flux d'entrée/sortie pour opérer sur des fichiers
#include <sstream> // Classes de modèles pour les flux de chaînes de caractères

// using namespace std;

// fonction : si l'instruction est "SET 5", on veut return "SET"
std::string parse_opcode(const std::string& instr){
    /*  
    pseudo code:
        > trouver ou est l'espace dans l'instruction
        > retourner tout ce qui est avant l'espace
            >> si il n'y a pas d'espace (ex: PRINT), return toute l'instruction
    */
        
        size_t espace = instr.find(' '); // Trouve la position du premier espace
    
        if(espace != std::string::npos) { // Si un espace est trouvé, extraire la partie avant l’espace
            return instr.substr(0, espace);
        }
    
        return instr; // Sinon, retourner toute l’instruction
}



// fonction : si l'instruction est "SET 5", on veut return 5, 0 si pas d'operande
std::uint16_t parse_operand(const std::string& instr){
    /*  
    pseudo code :
        > IF il y un espace 
            >> prendre tout ce qui est apres l'espace
            >> convertir en nombre
            >> retourner le nombre
        > ELSE cas de PRINT et de IFNZ, pas d'operande
        > return 0 ou gerer l'erreur
    */
        
        size_t espace = instr.find(' '); // Trouve la position du premier espace
    
        // Si un espace est trouvé, on tente d'extraire l'opérande
        if (espace != std::string::npos) {
            std::string operand_str = instr.substr(espace + 1);
        
            // Vérifier si l'opérande contient uniquement des chiffres
            for (char c : operand_str) {
                if (!std::isdigit(c)) {
                    std::cerr << "| ERROR | operande invalide! : [ " << instr << " ] " << std::endl;
                    return 0;
                }
            }
    
            // Convertir en uint16_t après vérification
            return static_cast<uint16_t>(stoul(operand_str));
        }
        
        return 0; // si pas d'operande
}


//  fonction : effectue les operations a la volée
void exec(const std::string& program_path){
    /*  
    pseudo code :
        > Initialiser un registre 16 bits à 0
        > Ouvrir le fichier programme
        > IF le fichier ne peut pas être ouvert
            >> Afficher un message d'erreur
            >> Retourner
        > WHILE lire chaque ligne du fichier
            >> IF la ligne est vide
                >>> Continuer à la prochaine ligne
            >> Extraire l'opcode de la ligne
            >> Extraire l'opérande de la ligne
            >> IF l'opcode est "SET"
                >>> Mettre à jour le registre avec l'opérande
            >> ELSE IF l'opcode est "ADD"
                >>> Ajouter l'opérande au registre
                >>> Limiter le registre à 65535 si nécessaire
            >> ELSE IF l'opcode est "SUB"
                >>> Soustraire l'opérande du registre
                >>> Limiter le registre à 0 si nécessaire
            >> ELSE IF l'opcode est "PRINT"
                >>> Afficher la valeur du registre
            >> ELSE IF l'opcode est "IFNZ"
                >>> IF le registre est 0
                    >>> Sauter la ligne suivante
            >> ELSE
                >>> Afficher un message d'erreur pour une instruction inconnue
        > Fermer le fichier
    */
        std::uint16_t registre = 0; // Registre 16 bits initialisé à 0
        std::string ligne;
    
        std::ifstream fichier(program_path); // Lire à partir du fichier
    
        if (!fichier) {
            std::cerr << "| ERROR | Impossible d'ouvrir le fichier : " << program_path << std::endl;
            return;
        }
    
        while (getline(fichier, ligne)){
            if (ligne.empty()) {
                continue; // Sauter les lignes vides
            }
    
            std::string opcode = parse_opcode(ligne);
            uint16_t operand = parse_operand(ligne);
    
            if (opcode == "SET") {
                uint16_t valeur = parse_operand(ligne);
                
                if(valeur != 0 || ligne == "SET 0"){ // Vérifier si parse_operand retourne une erreur
                    registre = valeur;
                }
            } 

            else if (opcode == "ADD") {
                uint32_t nouveau_registre = registre + operand;
                registre = (nouveau_registre > 65535) ? 65535 : nouveau_registre;
            } 
            
            else if (opcode == "SUB") {
                int nouveau_registre = registre - operand;
                registre = (nouveau_registre < 0) ? 0 : nouveau_registre;
            } 

            else if (opcode == "PRINT") {
                std::cout << registre << std::endl;
            } 

            else if (opcode == "IFNZ") {

                if (registre == 0) {
                    getline(fichier, ligne); // Sauter la ligne suivante
                }
            } 

            else {
                std::cerr << "| ERROR | Instruction inconnue!! -> [ " << opcode << " ]" << std::endl;
            }
        }
            
        fichier.close();
}



int main(int argc, char* argv[]){
    /*  
    pseudo code :
        > IF le nombre d'arguments est inférieur à 2
            >> Afficher un message d'erreur indiquant l'utilisation correcte du programme
            >> Retourner 1 pour indiquer une erreur
        > Stocker le chemin du fichier programme passé en argument
        > Appeler la fonction exec avec le chemin du fichier programme
        > Retourner 0 pour indiquer que le programme s'est terminé avec succès
    */
        // Vérifie si le nombre d'arguments passés au programme est inférieur à 2
        if (argc < 2) {
            // Affiche un message d'erreur indiquant l'utilisation correcte du programme
            std::cerr << "| ERROR | Usage: " << argv[0] << " <program_file>" << std::endl;
            // Retourne 1 pour indiquer une erreur
            return 1;
        }
    
        // Stocke le chemin du fichier programme passé en argument
        std::string program_path = argv[1];
        // Appelle la fonction exec avec le chemin du fichier programme
        exec(program_path);
        // Retourne 0 pour indiquer que le programme s'est terminé avec succès
        return 0;
}