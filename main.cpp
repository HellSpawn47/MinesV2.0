#include <iostream>
#include "Jugador.h"
#include "ListaCircular.h"
#include "Tablero.h"
#include "Randomizador.h"
using namespace std;

int main(){

	/*Lista<Jugador*> ListaDeJugadores;
	string nombre;
	Jugador* nuevoJugador;
	int i = 0;

	while (i < 3){
		cout << "Ingrese el nombre del nuevo Jugador: ";
		cin >> nombre ;
		nuevoJugador = new Jugador(nombre);
		ListaDeJugadores.agregar(nuevoJugador);
		i++;
	}

	ListaDeJugadores.iniciarCursor();

	while(ListaDeJugadores.avanzarCursor()){
		Jugador* jugadorActual = ListaDeJugadores.obtenerCursor();
		cout << jugadorActual->getNombre() << endl;
	}*/

	Tablero tablero(10,10);
	tablero.inicializar(15);
	tablero.imprimir();
	tablero.descubrirCasillero(0,0);
	tablero.imprimir();

	/*Randomizador randomizador;
	randomizador.setearSemillaRandom();
	cout << randomizador.obtenerNumeroAleatorioEntreRango(0, 5) << endl;
	cout << randomizador.obtenerNumeroAleatorioEntreRango(0, 5) << endl;
	cout << randomizador.obtenerNumeroAleatorioEntreRango(0, 5) << endl;
	cout << randomizador.obtenerNumeroAleatorioEntreRango(0, 5) << endl;
	*/

	return 0;
}
