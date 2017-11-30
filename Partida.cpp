#include "Partida.h"
#include "InteraccionConJugador.h"
#include "constantes.h"
#include "TableroBMP.h"
#include "PuntajesBMP.h"
#include "EstadoDePartida.h"
#include "Posicion.h"
using namespace std;
Partida::Partida(uint cantidadFilas, uint cantidadColumnas, char nivelDificultad){

	tablero = new Tablero(cantidadFilas, cantidadColumnas);

	dificultad = new Dificultad(cantidadFilas, cantidadColumnas, nivelDificultad);

	listaDeJugadores = new Lista<Jugador*>;

	listaDeJugadores->iniciarCursor();

	jugadorEnTurno = NULL;

	tablero->inicializar(dificultad->obtenerCantidadDeBombas());

	turno = 0;
}

void Partida::agregarJugador(std::string nombre, uint numeroJugador){

	Jugador* nuevoJugador = new Jugador(nombre, numeroJugador);
	listaDeJugadores->agregar(nuevoJugador);

}

void Partida::agregarJugadores(uint cantJugadores){
	InteraccionConJugador interactuar;
	for (uint i = 0; i < cantJugadores; i++){

		string nombre = interactuar.pedirNombreJugador(i);
		this->agregarJugador(nombre, i+1);

	}

}

void Partida::avanzarTurno(){

	listaDeJugadores->avanzarCursor();
	jugadorEnTurno = listaDeJugadores->obtenerCursor();
	if (jugadorEnTurno->getSigueJugando()){
		turno++;
	}
}

Jugador* Partida::obtenerJugadorEnTurno(){

	return this->jugadorEnTurno;

}

int Partida::calcularPuntajeDeJugada(Jugada* jugada){

	bool hayBomba = (this->tablero->obtenerValorCasillero(jugada->obtenerFila(),jugada->obtenerColumna()) == BOMBA);
	bool estabaMarcado = (this->tablero->obtenerEstadoCasillero(jugada->obtenerFila(),jugada->obtenerColumna()) == MARCADO);
	char accion = jugada->obtenerAccion();

	if (hayBomba && estabaMarcado && (accion == MARCAR || accion == ABRIR)){
		return -2;
	}
	else if (!hayBomba && estabaMarcado && (accion == MARCAR || accion == ABRIR)){
		return 2;
	}
	else if (accion == MARCAR && hayBomba && !estabaMarcado){
		return 1;
	}
	else if (accion == MARCAR && !hayBomba && !estabaMarcado){
		return -1;
	}
	return 0;
}

int Partida::actualizarTablero(Jugada* jugada,EstadoDePartida* estado){

	int puntaje;

	puntaje = calcularPuntajeDeJugada(jugada);

	if(jugada->obtenerAccion() == ABRIR){
		tablero->descubrirCasillero(jugada->obtenerFila(), jugada->obtenerColumna(),estado);

		if(tablero->obtenerValorCasillero(jugada->obtenerFila(), jugada->obtenerColumna()) == BOMBA){
			jugadorEnTurno->dejarFueraDeJuego();
			estado->agregarJugadorEliminado(jugadorEnTurno);
			cout << jugadorEnTurno->getNombre() << " esta fuera de juego\n" << endl;
		}
	}
	else if(jugada->obtenerAccion() == MARCAR){
		tablero->marcarCasillero(jugada->obtenerFila(), jugada->obtenerColumna());
		estado->agregarPosicionInteractuada(jugada->obtenerFila(), jugada->obtenerColumna(),MARCADO);

	}

	return puntaje;
}

int Partida::actualizarTableroDeshaciendoJugada(EstadoDePartida* estado){
	int costeDeshacer = -3;
	Posicion* posicionesADeshacer=estado->obtenerPosicionesDescubiertas();
	uint tope=estado->obtenerTope();
	for (uint i=0; i<tope; i++){
		tablero->cambiarEstadoCasillero(posicionesADeshacer[i].fila, posicionesADeshacer[i].columna,posicionesADeshacer[i].estadoDePosicion);
	}
	if (estado->obtenerJugadorEliminado()){
		Jugador* jugadorARevivir = estado->obtenerJugadorEliminado();
		jugadorARevivir->revivirJugador();
	}

	//Coste en puntos de deshacer una Jugada
	return costeDeshacer;
}


int Partida::actualizarTableroRehaciendoJugada(EstadoDePartida* estado){
	int costeRehacer = -3;
	Posicion* posicionesARehacer=estado->obtenerPosicionesDescubiertas();
	uint tope=estado->obtenerTope();
	for (uint i=0; i<tope; i++){
		tablero->cambiarEstadoCasillero(posicionesARehacer[i].fila, posicionesARehacer[i].columna,posicionesADeshacer[i].estadoDePosicion);
	}
	if (estado->obtenerJugadorEliminado()){
		Jugador* jugadorAEliminar = estado->obtenerJugadorEliminado();
		jugadorAEliminar->dejarFueraDeJuego();
		cout << jugadorAEliminar->getNombre() << " esta fuera de juego\n" << endl;
	}
	return costeRehacer;
}

void Partida::actualizarPuntaje(int puntaje){

	this->jugadorEnTurno->modificarPuntaje(puntaje);

}

void Partida::imprimirTablero(uint turno){

	tablero->imprimir(turno);

}

Lista<Jugador*>* Partida::obtenerListaDeJugadores(){
	return this->listaDeJugadores;
}

uint Partida::cantidadJugadoresActivos() {
	Jugador* jugadorActual;
	uint jugadoresActivos = 0;
	for (uint i=1; i<=listaDeJugadores->contarElementos();i++){
		jugadorActual = listaDeJugadores->obtener(i);
		if (jugadorActual->getSigueJugando()){
			jugadoresActivos++;
		}
	}
	return jugadoresActivos;
}

bool Partida::continuarPartida(){
	return ((this->tablero->casillerosRestantesPorDescubrir() > 0)&&(this->cantidadJugadoresActivos() > 0));
}

int Partida::obtenerTurnoActual(){
	return this->turno;
}

Tablero* Partida::getTablero(){

	return this->tablero;

}

Partida::~Partida() {
	listaDeJugadores->iniciarCursor();
	for(uint i=1;i<=listaDeJugadores->contarElementos();i++){
		listaDeJugadores->avanzarCursor();
		Jugador* jugadorActual = listaDeJugadores->obtenerCursor();
		delete jugadorActual;
	}
	delete tablero;
	delete dificultad;
	delete listaDeJugadores;
}
