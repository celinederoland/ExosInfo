/* 
 * File:   main.cpp
 * Author: celine
 *
 * Created on 24 novembre 2014, 08:26
 */

#include <cstdlib>
#include <iostream>


using namespace std;

/////////////////////////////////////////////////////////////////
//Syntaxe
/////////////////////////////////////////////////////////////////

class B {
};

//relation est un

class A : public B {
};

//héritage d'implémentation (usage interne d'une autre structure de données)

class C : private B {
};
/////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////
//Exemple : différence entre héritage privé et délégation
/////////////////////////////////////////////////////////////////

class Vector {
public:

    void push_back(int val) {

    };
};

class PileHeritagePrive : private Vector {

    void empiler(int val) {
        push_back(val);
    }
};

class PileDelegation {
    Vector v;

    void empiler(int val) {
        v.push_back(val);
    }
};
/////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////
//Exemple
/////////////////////////////////////////////////////////////////

struct Animal {
    string mySpecies;
    string myName;

    Animal() {}
    Animal(const string& espece, const string& nom)
    : mySpecies(espece), myName(nom) {
    }
};

struct Insecte : public Animal {

    Insecte(const string& nom)
    : Animal("Insecte", nom) {
    }
};

struct Mammifere : public Animal {

    Mammifere(const string& nom)
    : Animal("Mammifère", nom) {
    }
};

struct Lion : public Mammifere {
};

void affiche(Animal a) { //Constructeur par copie d'animal -> pas de polymorphisme
    cout << "Je suis un " << a.mySpecies
            << " et je m'appelle " << a.myName
            << " mon type est Animal"
            << endl;

}

void afficheDyn(const Animal &a) { //On manipule bien un insecte lors de affiche(morpion)
    cout << "Je suis un " << a.mySpecies
            << " et je m'appelle " << a.myName
            << " mon type est celui de l'argument qu'on m'a passé"
            << endl;
}

void affichePtr(const Animal* a) { //On manipule bien un insecte lors de affiche(morpion)
    cout << "Je suis un " << a -> mySpecies
            << " et je m'appelle " << a -> myName
            << " mon type est celui de l'argument qu'on m'a passé"
            << endl;
}
/////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////
// Polymorphisme
/////////////////////////////////////////////////////////////////

struct M {
    string toStringStatique() const { return "M"; }
    virtual string toStringDynamique() const { return "M";}
};

struct F : public M {
    string toStringStatique() const { return "F"; }
    virtual string toStringDynamique() const { return "F"; }
};

/////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////
// Classe abstraite
/////////////////////////////////////////////////////////////////

struct AbAnimal {
    virtual void cri() const = 0;
};

struct CLion : public AbAnimal {
    virtual void cri() const {
        cout << "Roooarr" << endl;
    }
};
/////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////
// Classe abstraite
/////////////////////////////////////////////////////////////////
struct Point{
    double x,y;
    Point(double x, double y) : x(x), y(y) {};
};

struct Boite {
    Point bg, hd;
    Boite(const Boite& autre) : bg(autre.bg), hd(autre.hd) {}
    Boite(const Point& bg, const Point& hd) : bg(bg), hd(hd) {}
    Boite(double xg, double yh, double xd, double yb) :
    bg(xg,yh), hd(xd,yb) {}
};

ostream& operator<<(ostream& out, const Point& p) {
    out << "(" << p.x << ";" << p.y << ")";
}

ostream& operator<<(ostream& out, const Boite&b) {
    out << "boite[" << b.bg << " ; " << b.hd << "]";
}
/////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////
// Transtypage
/////////////////////////////////////////////////////////////////
class Mere {
    virtual void mamethode() {}
};

class Fille : public Mere {
};
/////////////////////////////////////////////////////////////////
/*
 * 
 */
int main(int argc, char** argv) {

    Insecte morpion("CorBack"); //Instance d'insecte et aussi d'animal
    Mammifere chien("Toulouse"); //Instance de mammifère et aussi d'animal
    Mammifere lion("Simba"); //Instance de mammifère et aussi d'animal
    
    affiche(morpion);
    afficheDyn(morpion);
    affichePtr(&morpion);
    cout << endl;
    
    //5 instances d'animal initialisées avec le constructeur par défaut
    Animal* T[3]; 
    T[0] = &morpion;
    T[1] = &chien;
    T[2] = &lion;
    
    for (int i = 0; i < 3; ++i) {
        affiche(*T[i]); //instances d'Animal
        affichePtr(T[i]); //instances de Insecte ou Mammifere
    }
    
    cout << endl;
    
    //Allocation dynamique
    Animal* TDyn[3];
    //Création d'une instance d'Insecte et retourne son adresse dans le tas
    TDyn[0] = new Insecte("Corback");
    TDyn[1] = new Mammifere("Toulouse");
    TDyn[2] = new Mammifere("Simba");
    
    for (int i = 0; i < 3; ++i) {
        affiche(*TDyn[i]); //instances d'Animal
        affichePtr(TDyn[i]); //instances de Insecte ou Mammifere
    }
    
    //La désallocation n'est pas automatique
    for (int i = 0; i < 3; ++i) {
        delete TDyn[i];
    }
    
    cout << endl;
    
    M a;
    cout << a.toStringStatique() << " " << a.toStringDynamique() << endl;
    F b;
    cout << b.toStringStatique() << " " << b.toStringDynamique() << endl;
    
    M& ref_b = b; //Je vois l'instance de F comme un M
    cout << ref_b.toStringStatique() << " " << ref_b.toStringDynamique() << endl;
    
    M* ptr_b = &b;
    cout << ptr_b -> toStringStatique() << " " << ptr_b -> toStringDynamique() << endl;

    cout << endl;
    
    //Classe abstraite
    CLion mufasa; //OK
    //AbAnimal dahu; //Erreur
    AbAnimal& ref = mufasa; 
    ref.cri();
    
    cout << endl;
    
    //Surcharge
    Boite boite(3.0,6.0,7.2,1.5);
    cout << boite << endl;
    return 0;
    
    //Transtypage
    Mere* m1 = new Mere;
    Mere* m2 = new Fille;
    Fille* f;
    //Le transtypage de m2 est légal car le type concret de m2 est Fille
    f = dynamic_cast<Fille*>(m2); 
    if (f == 0) { cout << "transtypage vers m2 raté" << endl; }
    //Le transtypage de m1 échoue car le type concret de m1 est Mere
    f = dynamic_cast<Fille*>(m1); 
    if (f == 0) { cout << "transtypage vers m1 raté" << endl; }
    
}

