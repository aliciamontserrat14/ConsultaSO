#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

MYSQL *conn;

void conectarBD() {
	conn = mysql_init(NULL);
	if(conn==NULL) {
		printf("Error al crear la conexi￳n: %u %s\n",
		mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
	
	conn = mysql_real_connect (conn, "localhost", "root", "mysql", NULL, 0, NULL, 0);
	if(conn==NULL) {
		printf("Error al crear la conexi￳n: %u %s\n",
			   mysql_errno(conn), mysql_error(conn));
		exit(1);
	}
}

void ejecutarConsulta(char *sql) {
	if(mysql_query(conn, sql) !=0) {
		fprintf(stderr, "error en la consulta %s\n", mysql_error(conn));
		mysql_close(conn);
		exit(EXIT_FAILURE);
				
	}
	
	
}

int main() {
	conectarBD();
	ejecutarConsulta("DROP DATABASE IF EXISTS JUEGO");
	ejecutarConsulta("CREATE DATABASE JUEGO");
	ejecutarConsulta("USE JUEGO");
	ejecutarConsulta("CREATE TABLE jugadores(ID integer primary key not null, NOMBRE varchar(20) not null, CONTRASE￑A varchar(20) not null, PUNTOS integer, VICTORIAS integer)");
	ejecutarConsulta("CREATE TABLE partidas (ID integer primary key not null, FECHA varchar(20), TIEMPO time, JUGADORES integer, GANADOR varchar(20) not null)");
	ejecutarConsulta("CREATE TABLE relacionjp (ID integer primary key not null, ID_J integer not null, foreign key (ID_J) references jugadores(ID), ID_P integer not null, foreign key (ID_P) references partidas(ID))");
	
	ejecutarConsulta("INSERT INTO jugadores (ID, NOMBRE, CONTRASE￑A, PUNTOS, VICTORIAS) VALUES (1, 'Jugador1', 'Contrase￱a1', 100, 10), (2, 'Jugador2', 'Contrase￱a2', 200, 15), (3, 'Jugador3', 'Contrase￱a3', 150, 12)");
	ejecutarConsulta("INSERT INTO partidas (ID, FECHA, TIEMPO, JUGADORES, GANADOR) VALUES (1, '03-01', '11:00:00', 3, 'Jugador1'), (2, '03-02', '16:34:00', 2, 'Jugador2'), (3, '03-03', '17:30:00', 1, 'Jugador3')");
	ejecutarConsulta("INSERT INTO relacionjp (ID, ID_J, ID_P) VALUES (1, 1, 1), (2, 2, 1)");
	
	ejecutarConsulta("SELECT jugadores.NOMBRE FROM (jugadores, partidas, relacionjp) ORDER BY jugadores.PUNTOS DESC LIMIT 1");
	
	mysql_close(conn);
	return 0;
}