#include <iostream>
#include <cstdint>
#include <fstream>
#include <sstream>

using namespace std;

// fonction : si l'instruction est "SET 5", on veut return "SET"
string parse_opcode(const std::string& instr){
/*  
pseudo code:
    > trouver ou est l'espace dans l'instruction
    > retourner tout ce qui est avant l'espace
        >>  si il n'y a pas d'espace (ex: PRINT), return toute l'instruction
*/
    
    size_t espace = instr.find(' '); // Trouve la position du premier espace

    if(espace != string::npos) { //Si un espace est trouvé, extraire la partie avant l’espace
        return instr.substr(0, espace);
    }

    

    return instr; // Sinon, retourner toute l’instruction
}



// fonction : si l'instruction est "SET 5", on veut return 5, 0 si pas d'operande
uint16_t parse_operand(const std::string& instr){
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
    if (espace != string::npos) {
        std::string operand_str = instr.substr(espace + 1);
    
        // Vérifier si l'opérande contient uniquement des chiffres
        for (char c : operand_str) {
            if (!isdigit(c)) {
                cerr << "| ERROR | operande invalide! : [ " << instr << " ] " << endl;
                return 0;
            }
        }

        // Convertir en uint16_t après vérification
        return static_cast<uint16_t>(stoul(operand_str));
    }
    
    return 0; // si pas d'operande
    
}


//  fonction
void exec(const std::string& program_path){
    uint16_t registre = 0; // Registre 16 bits initialisé à 0
    string ligne;

    ifstream fichier(program_path); // Read from the file

    if (!fichier) {
        cerr << "| ERROR | Impossible d'ouvrir le fichier : " << program_path << endl;
        return;
    }

    while (getline(fichier, ligne)){
        if (ligne.empty()) {
            continue; // Skip empty lines
        }

        string opcode = parse_opcode(ligne);
        uint16_t operand = parse_operand(ligne);

        if (opcode == "SET") {
            uint16_t valeur = parse_operand(ligne);
            if(valeur != 0 or ligne == "SET 0"){ // verifier si parse operand retourne une erreur
                registre = valeur;
            }
        }

        else
         if (opcode == "ADD") {
            uint32_t nouveau_registre = registre + operand;
            registre = (nouveau_registre > 65535) ? 65535 : nouveau_registre;
        }


        else if (opcode == "SUB") {
            int nouveau_registre = registre - operand;
            registre = (nouveau_registre < 0) ? 0 : nouveau_registre;
        }

        else if (opcode == "PRINT") {
            cout << registre << endl;
        }
        
        else if (opcode == "IFNZ") {
            if (registre == 0) {
                getline(fichier, ligne); // Sauter la ligne suivante
            }
        }

        else {
            cerr << "| ERROR | Instruction inconnue!! -> [ " << opcode << " ]" << endl;
        }
    }
        
    fichier.close();
}



int main(){
    // tests output
    /*cout << " SET 5 utilisant |parse_opcode| : " << parse_opcode("SET 5") << endl; // afficher "SET"
    cout << " ADD 10 utilisant |parse_opcode| : " << parse_opcode("ADD 10") << endl; // afficher "ADD"
    cout << " PRINT utilisant |parse_opcode| : " << parse_opcode("PRINT") << endl; // afficher "PRINT"

    cout << " SET 5 utilisant |parse_operand| : " << parse_operand("SET 5") << endl; // afficher "5"
    cout << " ADD 10 utilisant |parse_operand| : " << parse_operand("ADD 10") << endl; // afficher "10"
    cout << " PRINT utilisant |parse_operand| : " << parse_operand("PRINT") << endl; // afficher "0"
    */
    exec("test.txt");
    //exec("program.txt");
    return 0;
}