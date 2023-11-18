#include <stdio.h>
#include <mysql.h>
#include <string.h>
//#include <DATOS_CONEXION.h>

#define TAM_CADENA 100

#define QUERY_INSERTAR_PROFESOR "INSERT INTO Profesores(ProfesorID,Nombre,Email) VALUES(?,?,?)"
#define QUERY_INSERTAR_CURSO "INSERT INTO Cursos(CursoID,NombreCurso,ProfesorID,Nombre) VALUES(?,?,?,?)"
#define QUERY_INSERTAR_ESTUDIANTE "INSERT INTO Estudiantes(EstudianteID,Nombre,Email) VALUES(?,?,?)"
#define QUERY_INSERTAR_INSCRIPCION "INSERT INTO Inscripciones(InscripcionID,Nombre,EstudianteID,NombreCurso,CursoID) VALUES(?,?,?,?,?)"

#define QUERY_BUSCAR_PROFESOR "SELECT * FROM Profesores WHERE ProfesorID = ?"

#define QUERY_ELIMINAR_PROFESOR "DELETE FROM Profesores WHERE ProfesorID = ?"
#define QUERY_ELIMINAR_ESTUDIANTE "DELETE FROM Estudiantes WHERE EstudianteID = ?"
#define QUERY_ELIMINAR_CURSO "DELETE FROM Cursos WHERE CursoID = ?"
#define QUERY_ELIMINAR_INSCRIPCION "DELETE FROM Inscripciones WHERE InscripcionID = ?"

int conectar(MYSQL **conexion);
int menu();

void EliminarProfesor();
void InsertarProfesor();
void BuscarProfesor(MYSQL *conexion, char *consulta);
void BuscarEstudiante(MYSQL *conexion, char *consulta);
char* BuscarNombreProfesor(MYSQL *conexion, char *consulta);
void BuscarInscripcion(MYSQL *conexion, char *consulta);
void BuscarCurso(MYSQL *conexion, char *consulta);


int main(int argc,char *argv[]){
	int salir = 0;
	do{
		int opc;
		//Variables auxiliares para encontrar campos
		char aux[200];
		char aux2[200];
		
		//Mas variables auxiliares
		char Buscar_Curso[200] = {};
		char Buscar_Curso2[200] = {};
		char ProfesorIDCurso[200] = {};
		char InscripcionCursoID[200] = {};
		
		//BIND para encontrar campos en SQL
		MYSQL_BIND bind_Profesor[200] = {};
		MYSQL_BIND bind_Estudiante[3] = {};
		MYSQL_BIND bind_Curso[3] = {};
		MYSQL_BIND bind_Inscripcion[5] = {};
		
		//Manejo de errores
		int error_conexion = 0;
		char consulta[200] = {},id[100] = {},id2[100] = {};
		unsigned long length[200];
		
		/*
			Variables para guardar campos
		*/
		int ProfesorID = 0;
		char NombreProf[TAM_CADENA] = {},EmailProf[TAM_CADENA] = {};
		
		//CURSOS
		int CursoID = 0;
		char NombreCurso[TAM_CADENA] = {};
		
		//ESTUDIANTE
		int EstudianteID = 0;
		char NombreEstu[TAM_CADENA] = {},EmailEstu[TAM_CADENA] = {};
		
		//INSCRIPCIONES
		int InscripcionID = 0;
		
		//Conexion a la BD
		MYSQL *conexion;
		MYSQL_STMT *stmt;
		MYSQL_RES *res_ptr;
		MYSQL_FIELD *campo;
		
		error_conexion = conectar(&conexion);
		if(!error_conexion){
			stmt = mysql_stmt_init(conexion);
			if(stmt){
				system("cls");
				switch(menu()){
					case 1:	//INSERTAR PROFESOR
						if(!mysql_stmt_prepare(stmt,QUERY_INSERTAR_PROFESOR,strlen(QUERY_INSERTAR_PROFESOR))){
							memset(bind_Profesor, 0, sizeof(bind_Profesor));
							bind_Profesor[0].buffer_type = MYSQL_TYPE_LONG;
							bind_Profesor[0].buffer = (char *) &ProfesorID;
							bind_Profesor[0].is_null = 0;
							bind_Profesor[0].length = 0;
							
							bind_Profesor[1].buffer_type = MYSQL_TYPE_STRING;
							bind_Profesor[1].buffer = (char *) NombreProf;
							bind_Profesor[1].buffer_length = TAM_CADENA;
							bind_Profesor[1].is_null = 0;
							bind_Profesor[1].length = &length[50];
							
							bind_Profesor[2].buffer_type = MYSQL_TYPE_STRING;
							bind_Profesor[2].buffer = (char *) EmailProf;
							bind_Profesor[2].buffer_length = TAM_CADENA;
							bind_Profesor[2].is_null = 0;
							bind_Profesor[2].length = &length[50];
							
							if(!mysql_stmt_bind_param(stmt, bind_Profesor)){
								
								
								printf("Ingrese su nombre: ");fflush(stdin);gets(NombreProf);
								length[50] = strlen(NombreProf);
								printf("Nombre: %s\n",NombreProf);
								
								printf("Ingrese su correo: "); fflush(stdin);gets(EmailProf);
								length[50] = strlen(EmailProf);
								printf("Correo: %s\n",EmailProf);
								
								if(!mysql_stmt_execute(stmt)){
									if(mysql_stmt_affected_rows(stmt) > 0){
										printf("Registro insertado exitosamente\n");
									}else{
										printf("Error al insertar el registro\n");
									}
									mysql_stmt_close(stmt);
								}else{
									printf("ERROR: mysql_stmt_execute(stmt) devuelve TRUE = %s ",mysql_stmt_error(stmt));
								}
							}else{
								printf("Error al asociar los parametros a la sentencia preparada\n");
							}
						}else{
							printf("Error al preparar la sentencia\n");
						}
						break;
					
					case 2: //BUSCAR PROFESOR
						printf("Ingresar id del Profesor: ");fflush(stdin);gets(id);
						sprintf(consulta,"SELECT * FROM Profesores WHERE ProfesorID = %s",id);
						
						BuscarProfesor(conexion,consulta);
						break;
					
					case 3: //ELIMINAR UN PROFESOR
						sprintf(consulta,"SELECT * FROM Profesores");
						BuscarProfesor(conexion,consulta);
						if(!mysql_stmt_prepare(stmt,QUERY_ELIMINAR_PROFESOR, strlen(QUERY_ELIMINAR_PROFESOR))){
							memset(bind_Profesor, 0, sizeof(bind_Profesor));
							bind_Profesor[0].buffer_type = MYSQL_TYPE_LONG;
							bind_Profesor[0].buffer = (char *) &ProfesorID;
							bind_Profesor[0].is_null = 0;
							bind_Profesor[0].length = 0;
											
							if(!mysql_stmt_bind_param(stmt, bind_Profesor)){
								
								printf("Ingrese id: "); scanf("%d",&ProfesorID);
												
								if(!mysql_stmt_execute(stmt)){
									if(mysql_stmt_affected_rows(stmt) > 0){
										printf("Registro eliminado exitosamente\n");
									}else{
										printf("Error al eliminar registro\n");
									}
									mysql_stmt_close(stmt);
								}
							}else{
								printf("Error al asociar los parametros a la sentencia preparada\n");
							}
						}else{
							printf("Eror al preparar la sentencia\n");
						}
						break;
					case 4: //ACTUALIZAR UN PROFESOR
						printf("Que registro desea actualizar?\n");
						printf("	(1) Un Nombre de un profesor\n");
						printf("	(2) Un correo de un profesor\n");
						printf("Opcion:  ");
	        			scanf("%d",&opc);
	        			sprintf(consulta,"SELECT * FROM Profesores");
						BuscarProfesor(conexion,consulta);
	        			switch(opc){
	        				case 1:
	        					if(!mysql_stmt_prepare(stmt, "UPDATE Profesores SET Nombre = ? WHERE ProfesorID = ?", strlen("UPDATE Profesores SET Nombre = ? WHERE ProfesorID = ?"))){
	        						
									memset(bind_Profesor, 0, sizeof(bind_Profesor));
									bind_Profesor[0].buffer_type = MYSQL_TYPE_STRING;
									bind_Profesor[0].buffer = (char *) NombreProf;
									bind_Profesor[0].buffer_length = TAM_CADENA;
									bind_Profesor[0].is_null = 0;
									bind_Profesor[0].length = &length[50];
									
									bind_Profesor[1].buffer_type = MYSQL_TYPE_LONG;
									bind_Profesor[1].buffer = (char *) &ProfesorID;
									bind_Profesor[1].is_null = 0;
									bind_Profesor[1].length = 0;
									
									if(!mysql_stmt_bind_param(stmt, bind_Profesor)){
										printf("Ingrese id del Profesor: "); scanf("%d",&ProfesorID);
										printf("Ingrese el nuevo Nombre: "); fflush(stdin);gets(NombreProf);
										length[50] = strlen(NombreProf);
										
										if(!mysql_stmt_execute(stmt)){
											if(mysql_stmt_affected_rows(stmt) > 0){
												printf("Registro actualizado exitosamente\n");
											}else{
												printf("Error al actualizar registro\n");
											}
											mysql_stmt_close(stmt);
										}
									}else{
										printf("Error al asociar los parametros a la sentencia preparada\n");
									}
								}else{
									printf("Error al preparar la sentencia\n");
								}
	        					break;
	        				case 2:
	        					if(!mysql_stmt_prepare(stmt, "UPDATE Profesores SET Email = ? WHERE ProfesorID = ?", strlen("UPDATE Profesores SET Email = ? WHERE ProfesorID = ?"))){
	        						
									memset(bind_Profesor, 0, sizeof(bind_Profesor));
									bind_Profesor[0].buffer_type = MYSQL_TYPE_STRING;
									bind_Profesor[0].buffer = (char *) NombreProf;
									bind_Profesor[0].buffer_length = TAM_CADENA;
									bind_Profesor[0].is_null = 0;
									bind_Profesor[0].length = &length[50];
									
									bind_Profesor[1].buffer_type = MYSQL_TYPE_LONG;
									bind_Profesor[1].buffer = (char *) &ProfesorID;
									bind_Profesor[1].is_null = 0;
									bind_Profesor[1].length = 0;
									
									if(!mysql_stmt_bind_param(stmt, bind_Profesor)){
										printf("Ingrese id del profesor: "); scanf("%d",&ProfesorID);
										printf("Ingrese el nuevo Email: "); fflush(stdin);gets(NombreProf);
										length[50] = strlen(NombreProf);
										
										if(!mysql_stmt_execute(stmt)){
											if(mysql_stmt_affected_rows(stmt) > 0){
												printf("Registro actualizado exitosamente\n");
											}else{
												printf("Error al actualizar registro\n");
											}
											mysql_stmt_close(stmt);
										}
									}else{
										printf("Error al asociar los parametros a la sentencia preparada\n");
									}
								}else{
									printf("Error al preparar la sentencia\n");
								}
	        					break;
	        				default:
	        					printf("Esa no es una opcion\n");
	        					break;		
						}
						break;
					case 5: // MOSTRAR TODOS LOS PROFESORES
						sprintf(consulta,"SELECT * FROM Profesores");
						BuscarProfesor(conexion,consulta);
						break;
					case 6:// AGREGAR UN ESTUDIANTE
						if(!mysql_stmt_prepare(stmt,QUERY_INSERTAR_ESTUDIANTE,strlen(QUERY_INSERTAR_ESTUDIANTE))){
							memset(bind_Estudiante, 0, sizeof(bind_Estudiante));
							bind_Estudiante[0].buffer_type = MYSQL_TYPE_LONG;
							bind_Estudiante[0].buffer = (char *) &EstudianteID;
							bind_Estudiante[0].is_null = 0;
							bind_Estudiante[0].length = 0;
							
							bind_Estudiante[1].buffer_type = MYSQL_TYPE_STRING;
							bind_Estudiante[1].buffer = (char *) NombreEstu;
							bind_Estudiante[1].buffer_length = TAM_CADENA;
							bind_Estudiante[1].is_null = 0;
							bind_Estudiante[1].length = &length[50];
							
							bind_Estudiante[2].buffer_type = MYSQL_TYPE_STRING;
							bind_Estudiante[2].buffer = (char *) EmailEstu;
							bind_Estudiante[2].buffer_length = TAM_CADENA;
							bind_Estudiante[2].is_null = 0;
							bind_Estudiante[2].length = &length[50];
							
							if(!mysql_stmt_bind_param(stmt, bind_Estudiante)){
								
								printf("Ingrese su nombre: ");fflush(stdin);gets(NombreEstu);
								length[50] = strlen(NombreEstu);
								printf("Nombre: %s\n",NombreEstu);
								
								printf("Ingrese su correo: "); fflush(stdin);gets(EmailEstu);
								length[50] = strlen(EmailEstu);
								printf("Correo: %s\n",EmailEstu);
								
								if(!mysql_stmt_execute(stmt)){
									if(mysql_stmt_affected_rows(stmt) > 0){
										printf("Registro insertado exitosamente\n");
									}else{
										printf("Error al insertar el registro\n");
									}
									mysql_stmt_close(stmt);
								}else{
									printf("ERROR: mysql_stmt_execute(stmt) devuelve TRUE = %s ",mysql_stmt_error(stmt));
								}
							}else{
								printf("Error al asociar los parametros a la sentencia preparada\n");
							}
						}else{
							printf("Error al preparar la sentencia\n");
						}
						break;
					case 7://BUSCAR ESTUDIANTE
						printf("Ingresar id del Estudiante: ");fflush(stdin);gets(id);
						sprintf(consulta,"SELECT * FROM Estudiantes WHERE EstudianteID = %s",id);
						
						BuscarEstudiante(conexion,consulta);
						break;
					case 8: //ELIMINAR ESTUDIANTE
						sprintf(consulta,"SELECT * FROM Estudiantes");
						BuscarEstudiante(conexion,consulta);
						if(!mysql_stmt_prepare(stmt,QUERY_ELIMINAR_ESTUDIANTE, strlen(QUERY_ELIMINAR_ESTUDIANTE))){
							memset(bind_Estudiante, 0, sizeof(bind_Estudiante));
							bind_Estudiante[0].buffer_type = MYSQL_TYPE_LONG;
							bind_Estudiante[0].buffer = (char *) &EstudianteID;
							bind_Estudiante[0].is_null = 0;
							bind_Estudiante[0].length = 0;
											
							if(!mysql_stmt_bind_param(stmt, bind_Estudiante)){
								
								printf("Ingrese id: "); scanf("%d",&EstudianteID);
												
								if(!mysql_stmt_execute(stmt)){
									if(mysql_stmt_affected_rows(stmt) > 0){
										printf("Registro eliminado exitosamente\n");
									}else{
										printf("Error al eliminar registro\n");
									}
									mysql_stmt_close(stmt);
								}else{
									printf("Error en: mysql_stmt_execute(stmt) %s",mysql_stmt_error(stmt));							
								}
							}else{
								printf("Error al asociar los parametros a la sentencia preparada\n");
							}
						}else{
							printf("Eror al preparar la sentencia\n");
						}
						break;
					case 9://ACTUALIZAR UN ESTUDIANTE
						printf("Que registro desea actualizar?\n");
						printf("	(1) Un nombre de un alumno\n");
						printf("	(2) Un correo de un alumno\n");
						printf("Opcion:  ");
	        			scanf("%d",&opc);
	        			sprintf(consulta,"SELECT * FROM Estudiantes");
						BuscarEstudiante(conexion,consulta);
	        			switch(opc){
	        				case 1:
	        					if(!mysql_stmt_prepare(stmt, "UPDATE Estudiantes SET Nombre = ? WHERE EstudianteID = ?", strlen("UPDATE Estudiantes SET Nombre = ? WHERE EstudianteID = ?"))){
	        						
									memset(bind_Estudiante, 0, sizeof(bind_Estudiante));
									bind_Estudiante[0].buffer_type = MYSQL_TYPE_STRING;
									bind_Estudiante[0].buffer = (char *) NombreEstu;
									bind_Estudiante[0].buffer_length = TAM_CADENA;
									bind_Estudiante[0].is_null = 0;
									bind_Estudiante[0].length = &length[50];
									
									bind_Estudiante[1].buffer_type = MYSQL_TYPE_LONG;
									bind_Estudiante[1].buffer = (char *) &EstudianteID;
									bind_Estudiante[1].is_null = 0;
									bind_Estudiante[1].length = 0;
									
									if(!mysql_stmt_bind_param(stmt, bind_Estudiante)){
										printf("Ingrese id del estudiante: "); scanf("%d",&EstudianteID);
										printf("Ingrese el nuevo Nombre: "); fflush(stdin);gets(NombreEstu);
										length[50] = strlen(NombreEstu);
										
										if(!mysql_stmt_execute(stmt)){
											if(mysql_stmt_affected_rows(stmt) > 0){
												printf("Registro actualizado exitosamente\n");
											}else{
												printf("Error al actualizar registro\n");
											}
											mysql_stmt_close(stmt);
										}
									}else{
										printf("Error al asociar los parametros a la sentencia preparada\n");
									}
								}else{
									printf("Error al preparar la sentencia\n");
								}
								break;
							case 2:
	        					if(!mysql_stmt_prepare(stmt, "UPDATE Estudiantes SET Email = ? WHERE EstudianteID = ?", strlen("UPDATE Estudiantes SET Email = ? WHERE EstudianteID = ?"))){
	        						
									memset(bind_Estudiante, 0, sizeof(bind_Estudiante));
									bind_Estudiante[0].buffer_type = MYSQL_TYPE_STRING;
									bind_Estudiante[0].buffer = (char *) EmailEstu;
									bind_Estudiante[0].buffer_length = TAM_CADENA;
									bind_Estudiante[0].is_null = 0;
									bind_Estudiante[0].length = &length[50];
									
									bind_Estudiante[1].buffer_type = MYSQL_TYPE_LONG;
									bind_Estudiante[1].buffer = (char *) &EstudianteID;
									bind_Estudiante[1].is_null = 0;
									bind_Estudiante[1].length = 0;
									
									if(!mysql_stmt_bind_param(stmt, bind_Estudiante)){
										printf("Ingrese id del estudiante: "); scanf("%d",&EstudianteID);
										printf("Ingrese el nuevo Email: "); fflush(stdin);gets(EmailEstu);
										length[50] = strlen(EmailEstu);
										
										if(!mysql_stmt_execute(stmt)){
											if(mysql_stmt_affected_rows(stmt) > 0){
												printf("Registro actualizado exitosamente\n");
											}else{
												printf("Error al actualizar registro\n");
											}
											mysql_stmt_close(stmt);
										}
									}else{
										printf("Error al asociar los parametros a la sentencia preparada\n");
									}
								}else{
									printf("Error al preparar la sentencia\n");
								}
								break;
								default:
									printf("Opcion no valida");
									break;
							
							}
						break;
					case 10://MOSTRAR TODOS LOS ESTUDIANTES
						sprintf(consulta,"SELECT * FROM Estudiantes");
						BuscarEstudiante(conexion,consulta);
						break;
					case 11: //Crear nuevo curso
						if(!mysql_stmt_prepare(stmt,QUERY_INSERTAR_CURSO,strlen(QUERY_INSERTAR_CURSO))){
							memset(bind_Curso, 0, sizeof(bind_Curso));
							bind_Curso[0].buffer_type = MYSQL_TYPE_LONG;
							bind_Curso[0].buffer = (char *) &CursoID;
							bind_Curso[0].is_null = 0;
							bind_Curso[0].length = 0;
							
							bind_Curso[1].buffer_type = MYSQL_TYPE_STRING;
							bind_Curso[1].buffer = (char *) NombreCurso;
							bind_Curso[1].buffer_length = TAM_CADENA;
							bind_Curso[1].is_null = 0;
							bind_Curso[1].length = &length[100];
							
							bind_Curso[2].buffer_type = MYSQL_TYPE_STRING;
							bind_Curso[2].buffer = (char *) id;
							bind_Curso[2].buffer_length = TAM_CADENA;
							bind_Curso[2].is_null = 0;
							bind_Curso[2].length = &length[50];
							
							bind_Curso[3].buffer_type = MYSQL_TYPE_STRING;
							bind_Curso[3].buffer = (char *) aux2;
							bind_Curso[3].buffer_length = TAM_CADENA;
							bind_Curso[3].is_null = 0;
							bind_Curso[3].length = &length[100];
							
							if(!mysql_stmt_bind_param(stmt, bind_Curso)){
								
								printf("Ingrese el nombre del nuevo curso: ");fflush(stdin);gets(NombreCurso);
								length[100] = strlen(NombreCurso);
								printf("Nombre del curso: %s\n\n",NombreCurso);
								
								sprintf(consulta,"SELECT * FROM Profesores");
								BuscarProfesor(conexion,consulta);
								printf("\nIngrese el id del profesor que dara el curso: "); fflush(stdin);gets(id);
								length[50] = strlen(id);
								
								sprintf(aux,"SELECT * FROM Profesores WHERE ProfesorID = %s",id);
								sprintf(aux2, BuscarNombreProfesor(conexion,aux));
								length[100] = strlen(aux2);
								printf("ID : %s\n",id);
								printf("Profesor: %s\n",aux2);
								
								if(!mysql_stmt_execute(stmt)){
									if(mysql_stmt_affected_rows(stmt) > 0){
										printf("Registro insertado exitosamente\n");
									}else{
										printf("Error al insertar el registro\n");
									}
									mysql_stmt_close(stmt);
								}else{
									printf("ERROR: mysql_stmt_execute(stmt) devuelve TRUE = %s ",mysql_stmt_error(stmt));
								}
							}else{
								printf("Error al asociar los parametros a la sentencia preparada\n");
							}
						}else{
							printf("Error al preparar la sentencia\n");
						}
						break;
					case 12: //BUSCAR POR CURSO
						printf("Inserte el ID del curso: ");
						fflush(stdin);
						gets(InscripcionCursoID);
						
						sprintf(consulta,"SELECT * FROM Cursos WHERE CursoID = %s",InscripcionCursoID);
						BuscarCurso(conexion,consulta);
						break;
					case 13: //ELIMINAR UN CURSO
						sprintf(consulta,"SELECT * FROM Cursos");
						BuscarCurso(conexion,consulta);
						if(!mysql_stmt_prepare(stmt,QUERY_ELIMINAR_CURSO, strlen(QUERY_ELIMINAR_CURSO))){
							memset(bind_Curso, 0, sizeof(bind_Curso));
							bind_Curso[0].buffer_type = MYSQL_TYPE_LONG;
							bind_Curso[0].buffer = (char *) &CursoID;
							bind_Curso[0].is_null = 0;
							bind_Curso[0].length = 0;
											
							if(!mysql_stmt_bind_param(stmt, bind_Curso)){
								
								printf("Ingrese id del curso: "); scanf("%d",&CursoID);
												
								if(!mysql_stmt_execute(stmt)){
									if(mysql_stmt_affected_rows(stmt) > 0){
										printf("Registro eliminado exitosamente\n");
									}else{
										printf("Error al eliminar registro\n");
									}
									mysql_stmt_close(stmt);
								}else{
									printf("Error en: mysql_stmt_execute(stmt) %s",mysql_stmt_error(stmt));							
								}
							}else{
								printf("Error al asociar los parametros a la sentencia preparada\n");
							}
						}else{
							printf("Eror al preparar la sentencia\n");
						}
						break;
					case 14://Editar informacion de un curso
						printf("Que registro desea actualizar?\n");
						printf("	(1) Un nombre de un curso\n");
						printf("	(2) El profesor que imparte el curso\n");
						printf("Opcion:  ");
	        			scanf("%d",&opc);
	        			sprintf(consulta,"SELECT * FROM Cursos");
						BuscarInscripcion(conexion,consulta);
	        			switch(opc){
	        				case 1:
	        					if(!mysql_stmt_prepare(stmt, "UPDATE Cursos SET NombreCurso = ? WHERE CursoID = ?", strlen("UPDATE Cursos SET NombreCurso = ? WHERE CursoID = ?"))){
	        						
									memset(bind_Curso, 0, sizeof(bind_Curso));
									bind_Curso[0].buffer_type = MYSQL_TYPE_STRING;
									bind_Curso[0].buffer = (char *) NombreCurso;
									bind_Curso[0].buffer_length = TAM_CADENA;
									bind_Curso[0].is_null = 0;
									bind_Curso[0].length = &length[50];
									
									bind_Curso[1].buffer_type = MYSQL_TYPE_LONG;
									bind_Curso[1].buffer = (char *) &CursoID;
									bind_Curso[1].is_null = 0;
									bind_Curso[1].length = 0;
									
									
									
									if(!mysql_stmt_bind_param(stmt, bind_Curso)){
										printf("Ingrese id del curso: "); fflush(stdin); scanf("%d",&CursoID);
										printf("Ingrese el nuevo Nombre del curso: "); fflush(stdin);gets(NombreCurso);
										length[50] = strlen(NombreCurso);
										
										if(!mysql_stmt_execute(stmt)){
											if(mysql_stmt_affected_rows(stmt) > 0){
												printf("Registro actualizado exitosamente\n");
											}else{
												printf("Error al actualizar registro: %d\n",mysql_stmt_affected_rows(stmt));
											}
											mysql_stmt_close(stmt);
										}else{
											printf("ERROR: mysql_stmt_execute(stmt) devuelve TRUE = %s ",mysql_stmt_error(stmt));
										}
									}else{
										printf("Error al asociar los parametros a la sentencia preparada\n");
									}
								}else{
									printf("Error al preparar la sentencia\n");
								}
								break;
							case 2:
	        					if(!mysql_stmt_prepare(stmt, "UPDATE Cursos SET Nombre = ?,ProfesorID = ?  WHERE CursoID = ?", strlen("UPDATE Cursos SET Nombre = ?,ProfesorID = ?  WHERE CursoID = ?"))){
	        						
									memset(bind_Estudiante, 0, sizeof(bind_Curso));
									bind_Curso[0].buffer_type = MYSQL_TYPE_STRING;
									bind_Curso[0].buffer = (char *) aux2;
									bind_Curso[0].buffer_length = TAM_CADENA;
									bind_Curso[0].is_null = 0;
									bind_Curso[0].length = &length[150];
									
									bind_Curso[1].buffer_type = MYSQL_TYPE_STRING;
									bind_Curso[1].buffer = (char *) ProfesorIDCurso;
									bind_Curso[1].buffer_length = TAM_CADENA;
									bind_Curso[1].is_null = 0;
									bind_Curso[1].length = &length[50];
									
									bind_Curso[2].buffer_type = MYSQL_TYPE_STRING;
									bind_Curso[2].buffer = (char *) id;
									bind_Curso[2].buffer_length = TAM_CADENA;
									bind_Curso[2].is_null = 0;
									bind_Curso[2].length = &length[100];
									
									if(!mysql_stmt_bind_param(stmt, bind_Curso)){
										printf("\nIngrese id del curso: "); fflush(stdin);gets(id);
										
										sprintf(consulta,"SELECT * FROM Profesores");
										BuscarProfesor(conexion,consulta);
										printf("\nIngrese el id del nuevo profesor que impartira el curso: "); fflush(stdin);gets(ProfesorIDCurso);
										
										sprintf(aux,"SELECT * FROM Profesores WHERE ProfesorID = %s",ProfesorIDCurso);
										sprintf(aux2, BuscarNombreProfesor(conexion,aux));
										length[150] = strlen(aux2);
										length[50] = strlen(ProfesorIDCurso);
										length[100] = strlen(id);
										
										
										printf("ID : %s\n",ProfesorIDCurso);
										printf("Profesor: %s\n",aux2);
										
	
										if(!mysql_stmt_execute(stmt)){
											if(mysql_stmt_affected_rows(stmt) > 0){
												printf("Registro actualizado exitosamente\n");
											}else{
												printf("Error al actualizar registro\n");
											}
											mysql_stmt_close(stmt);
										}else{
											printf("ERROR: mysql_stmt_execute(stmt) devuelve TRUE = %s ",mysql_stmt_error(stmt));
										}
									}else{
										printf("Error al asociar los parametros a la sentencia preparada\n");
									}
								}else{
									printf("Error al preparar la sentencia\n");
								}
								break;
								default:
									printf("Opcion no valida");
									break;
							}//FIN SWITCH CASE
						break;// BREAK CASO 14
					
					case 15: //MOSTRAR TODOS LOS CURSOS
						sprintf(consulta,"SELECT * FROM Cursos");
						BuscarCurso(conexion,consulta);
						break;
					case 16: //Crear nueva asignacion
						if(!mysql_stmt_prepare(stmt,QUERY_INSERTAR_INSCRIPCION,strlen(QUERY_INSERTAR_INSCRIPCION))){			
							memset(bind_Inscripcion, 0, sizeof(bind_Inscripcion));
							bind_Inscripcion[0].buffer_type = MYSQL_TYPE_LONG;
							bind_Inscripcion[0].buffer = (char *) &InscripcionID;
							bind_Inscripcion[0].is_null = 0;
							bind_Inscripcion[0].length = 0;
							
							bind_Inscripcion[1].buffer_type = MYSQL_TYPE_STRING;
							bind_Inscripcion[1].buffer = (char *) aux2;
							bind_Inscripcion[1].buffer_length = TAM_CADENA;
							bind_Inscripcion[1].is_null = 0;
							bind_Inscripcion[1].length = &length[150];
							
							bind_Inscripcion[2].buffer_type = MYSQL_TYPE_STRING;
							bind_Inscripcion[2].buffer = (char *) id;
							bind_Inscripcion[2].buffer_length = TAM_CADENA;
							bind_Inscripcion[2].is_null = 0;
							bind_Inscripcion[2].length = &length[50];
							
							bind_Inscripcion[3].buffer_type = MYSQL_TYPE_STRING;
							bind_Inscripcion[3].buffer = (char *) Buscar_Curso2;
							bind_Inscripcion[3].buffer_length = TAM_CADENA;
							bind_Inscripcion[3].is_null = 0;
							bind_Inscripcion[3].length = &length[200];
							
							bind_Inscripcion[4].buffer_type = MYSQL_TYPE_STRING;
							bind_Inscripcion[4].buffer = (char *) InscripcionCursoID;
							bind_Inscripcion[4].buffer_length = TAM_CADENA;
							bind_Inscripcion[4].is_null = 0;
							bind_Inscripcion[4].length = &length[100];
							
							if(!mysql_stmt_bind_param(stmt, bind_Inscripcion)){
								sprintf(consulta,"SELECT * FROM Cursos");
								BuscarCurso(conexion,consulta);
								printf("\nIngrese el id del curso a asignar: ");fflush(stdin);gets(InscripcionCursoID);
								
								sprintf(consulta,"SELECT * FROM Estudiantes");
								BuscarEstudiante(conexion,consulta);
								printf("\nIngrese el id del estudiante a asignar curso: "); fflush(stdin);gets(id);
													
								sprintf(aux,"SELECT * FROM Estudiantes WHERE EstudianteID = %s",id);
								sprintf(aux2, BuscarNombreProfesor(conexion,aux));	
								length[150] = strlen(aux2);
								length[50] = strlen(id);
								
								sprintf(Buscar_Curso,"SELECT * FROM Cursos WHERE CursoID = %s",InscripcionCursoID);
								sprintf(Buscar_Curso2, BuscarNombreProfesor(conexion,Buscar_Curso));
								length[200] = strlen(Buscar_Curso2);
								length[100] = strlen(InscripcionCursoID);
								
								printf("\nID Estudiante: %s\n",id);
								printf("Nombre Estudiante: %s\n",aux2);
								printf("Curso asignado: %s\n",Buscar_Curso2);
								printf("ID del curso: %s\n\n",InscripcionCursoID);
								
								if(!mysql_stmt_execute(stmt)){
									if(mysql_stmt_affected_rows(stmt) > 0){
										printf("Registro insertado exitosamente\n");
									}else{
										printf("Error al insertar el registro\n");
									}
									mysql_stmt_close(stmt);
								}else{
									printf("ERROR: mysql_stmt_execute(stmt) devuelve TRUE = %s ",mysql_stmt_error(stmt));
								}
							}else{
								printf("Error al asociar los parametros a la sentencia preparada\n");
							}
						}else{
							printf("Error al preparar la sentencia\n");
						}
						break;
					case 17: //Buscar inscripcion
						printf("Inserte el ID  de la inscripcion: ");
						fflush(stdin);
						gets(InscripcionCursoID);
						
						sprintf(consulta,"SELECT * FROM Inscripciones WHERE InscripcionID = %s",InscripcionCursoID);
						BuscarInscripcion(conexion,consulta);
						break;
					case 18:
						sprintf(consulta,"SELECT * FROM Cursos");
						BuscarCurso(conexion,consulta);
						printf("\n\nInserte el ID  del curso: ");
						fflush(stdin);
						gets(InscripcionCursoID);
						
						sprintf(consulta,"SELECT * FROM Inscripciones WHERE CursoID = %s",InscripcionCursoID);
						BuscarInscripcion(conexion,consulta);
						break;
					case 19://ELIMINAR INSCRIPCION
						sprintf(consulta,"SELECT * FROM Inscripciones");
						BuscarInscripcion(conexion,consulta);
						if(!mysql_stmt_prepare(stmt,QUERY_ELIMINAR_INSCRIPCION, strlen(QUERY_ELIMINAR_INSCRIPCION))){
							memset(bind_Inscripcion, 0, sizeof(bind_Inscripcion));
							bind_Inscripcion[0].buffer_type = MYSQL_TYPE_LONG;
							bind_Inscripcion[0].buffer = (char *) &InscripcionID;
							bind_Inscripcion[0].is_null = 0;
							bind_Inscripcion[0].length = 0;
											
							if(!mysql_stmt_bind_param(stmt, bind_Inscripcion)){
								
								printf("Ingrese id de la inscripcion que quiera eliminar: "); scanf("%d",&InscripcionID);
												
								if(!mysql_stmt_execute(stmt)){
									if(mysql_stmt_affected_rows(stmt) > 0){
										printf("Registro eliminado exitosamente\n");
									}else{
										printf("Error al eliminar registro\n");
									}
									mysql_stmt_close(stmt);
								}else{
									printf("Error en: mysql_stmt_execute(stmt) %s",mysql_stmt_error(stmt));							
								}
							}else{
								printf("Error al asociar los parametros a la sentencia preparada\n");
							}
						}else{
							printf("Eror al preparar la sentencia\n");
						}
						break;
					case 20://MOSTRAR TODAS LAS INSCRIPCIONES
						sprintf(consulta,"SELECT * FROM Inscripciones");
						BuscarInscripcion(conexion,consulta);
						break;
					case 21: //SALIR
						salir = 1;
						mysql_close(conexion);
						break;	
					default:
						printf("Opcion incorrecta\n");
						break;
				}
			}else{
				printf("Error la inicializar la sentencia preparada\n");
			}	
		}
		system("pause");
	}while(salir == 0);
	
	return 0;
		
}

char *BuscarNombreProfesor(MYSQL *conexion, char *consulta){
	
	int error, filas,columnas,i,j;
	char *aux;
	
	MYSQL_FIELD *campo;
	MYSQL_RES *res_ptr;
	MYSQL_ROW res_fila;
	
	error = mysql_query(conexion,consulta);
	if(!error){
		res_ptr = mysql_store_result(conexion);
		
		if(res_ptr){
			filas = mysql_num_rows(res_ptr);
			columnas = mysql_num_fields(res_ptr);
		
			
			//printf("\n");
			
			for(i = 1;i<=filas;i++){
				res_fila = mysql_fetch_row(res_ptr);
				for(j = 0;j<columnas;j++){
					if(j == 1){
						//printf("Nombreeeee:   %30s\t",res_fila[j]);	
						aux = res_fila[j];
						return res_fila[j];
					}
					
				}
				//printf("\n");
			}
		}else{
		printf("Error al obtener resultado\n");
		}	
	}else{
		printf("Ocurrio un error al ejecutar la consulta SQL\n");
	}
}

void BuscarProfesor(MYSQL *conexion, char *consulta){
	
	int error, filas,columnas,i,j;
	
	MYSQL_FIELD *campo;
	MYSQL_RES *res_ptr;
	MYSQL_ROW res_fila;
	
	error = mysql_query(conexion,consulta);
	if(!error){
		res_ptr = mysql_store_result(conexion);
		
		if(res_ptr){
			filas = mysql_num_rows(res_ptr);
			columnas = mysql_num_fields(res_ptr);
		
			while(campo = mysql_fetch_field(res_ptr)){
				printf("%20s\t",campo->name);
			}
			printf("\n");
			
			for(i = 1;i<=filas;i++){
				res_fila = mysql_fetch_row(res_ptr);
				for(j = 0;j<columnas;j++){
					if(j == 0){
						printf("%15s\t",res_fila[j]);
					}
					if(j == 1){
						printf("%30s\t",res_fila[j]);	
					}
					if(j == 2){
						printf("%30s\t",res_fila[j]);
					}
					
				}
				printf("\n");
			}
		}else{
		printf("Error al obtener resultado\n");
		}	
	}else{
		printf("Ocurrio un error al ejecutar la consulta SQL\n");
	}
}

void BuscarInscripcion(MYSQL *conexion, char *consulta){
	
	int error, filas,columnas,i,j;
	
	MYSQL_FIELD *campo;
	MYSQL_RES *res_ptr;
	MYSQL_ROW res_fila;
	
	error = mysql_query(conexion,consulta);
	if(!error){
		res_ptr = mysql_store_result(conexion);
		
		if(res_ptr){
			filas = mysql_num_rows(res_ptr);
			columnas = mysql_num_fields(res_ptr);
		
			while(campo = mysql_fetch_field(res_ptr)){
				printf("%20s\t",campo->name);
			}
			printf("\n");
			
			for(i = 1;i<=filas;i++){
				res_fila = mysql_fetch_row(res_ptr);
				for(j = 0;j<columnas;j++){
					if(j == 0){
						printf("%15s\t",res_fila[j]);
					}
					if(j == 1){
						printf("%30s\t",res_fila[j]);	
					}
					if(j == 2){
						printf("%15s\t",res_fila[j]);
					}
					if(j == 3){
						printf("%27s\t",res_fila[j]);
					}
					if(j == 4){
						printf("%17s\t",res_fila[j]);
					}
					
				}
				printf("\n");
			}
		}else{
		printf("Error al obtener resultado\n");
		}	
	}else{
		printf("Ocurrio un error al ejecutar la consulta SQL\n");
	}
}

void BuscarEstudiante(MYSQL *conexion, char *consulta){
	
	int error, filas,columnas,i,j;
	
	MYSQL_FIELD *campo;
	MYSQL_RES *res_ptr;
	MYSQL_ROW res_fila;
	
	error = mysql_query(conexion,consulta);
	if(!error){
		res_ptr = mysql_store_result(conexion);
		
		if(res_ptr){
			filas = mysql_num_rows(res_ptr);
			columnas = mysql_num_fields(res_ptr);
		
			while(campo = mysql_fetch_field(res_ptr)){
				printf("%20s\t",campo->name);
			}
			printf("\n");
			
			for(i = 1;i<=filas;i++){
				res_fila = mysql_fetch_row(res_ptr);
				for(j = 0;j<columnas;j++){
					if(j == 0){
						printf("%15s\t",res_fila[j]);
					}
					if(j == 1){
						printf("%30s\t",res_fila[j]);	
					}
					if(j == 2){
						printf("%30s\t",res_fila[j]);
					}
					if(j == 3){
						printf("%30s\t",res_fila[j]);
					}
					if(j == 4){
						printf("%30s\t",res_fila[j]);
					}
					
				}
				printf("\n");
			}
		}else{
		printf("Error al obtener resultado\n");
		}	
	}else{
		printf("Ocurrio un error al ejecutar la consulta SQL\n");
	}
}

void BuscarCurso(MYSQL *conexion, char *consulta){
	
	int error, filas,columnas,i,j;
	
	MYSQL_FIELD *campo;
	MYSQL_RES *res_ptr;
	MYSQL_ROW res_fila;
	
	error = mysql_query(conexion,consulta);
	if(!error){
		res_ptr = mysql_store_result(conexion);
		
		if(res_ptr){
			filas = mysql_num_rows(res_ptr);
			columnas = mysql_num_fields(res_ptr);
		
			while(campo = mysql_fetch_field(res_ptr)){
				printf("%20s\t",campo->name);
			}
			printf("\n");
			
			for(i = 1;i<=filas;i++){
				res_fila = mysql_fetch_row(res_ptr);
				for(j = 0;j<columnas;j++){
					if(j == 0){
						printf("%18s\t",res_fila[j]);
					}
					if(j == 1){
						printf("%22s\t",res_fila[j]);	
					}
					if(j == 2){
						printf("%16s\t",res_fila[j]);
					}
					if(j == 3){
						printf("%23s\t",res_fila[j]);
					}
					
				}
				printf("\n");
			}
		}else{
		printf("Error al obtener resultado\n");
		}	
	}else{
		printf("Ocurrio un error al ejecutar la consulta SQL\n");
	}
}

int conectar(MYSQL **conexion){
	int error;
	
	*conexion = mysql_init(NULL);
	
	if(mysql_real_connect(*conexion,"localhost","root","123456","BDescuela",3306,NULL,0) != NULL){
		printf("Se establecio la conexion con la base de datos\n");
		error = 0;
	}else{
		printf("Error al conectar la base de datos\n");
		error = 1;
	}
	return error;
}
//PRUEBA DE EDICION 
int menu(){
	int opc = 0;
        printf(" __________________________________________________________________________________________________________________________________________________________________\n");
        printf("|                                                            Bienvenido, escoja una opcion a ejecutar                                                              |\n");
        printf(" __________________________________________________________________________________________________________________________________________________________________\n");
        printf("|            PROFESORES                 |           ESTUDIANTES                   |                  CURSOS                 |        Inscripciones                 |\n");
        printf(" _______________________________________|_________________________________________|_________________________________________|______________________________________|\n");
		printf("| (1) Crear nuevo profesor              | (6) Crear nuevo estudiante              | (11) Crear nuevo curso                  | (16) Crear nueva inscripcion         |\n");
        printf("| (2) Buscar profesor                   | (7) Buscar estudiante                   | (12) Buscar por curso                   | (17) Buscar una inscripcion          |\n");
        printf("| (3) Eliminar profesor                 | (8) Eliminar estudiante                 | (13) Eliminar un curso                  | (18) Buscar inscripcion por curso    |\n");
        printf("| (4) Editar informacion de un profesor | (9) Editar informacion de un estudiante | (14) Editar informacion de un curso     | (19) Eliminar una inscripcion        |\n");
		printf("| (5) Mostrar todos los profesores      | (10) Mostrar todos los estudiantes      | (15) Mostrar todos los cursos           | (20) Mostrar todas las inscripciones |\n"); 
		printf("|                                       |                                         |                                         |                                      |\n");
		printf("|                                       |                                         |                                         |                                      |\n");
        printf("|                                       |                                     (21) Salir                                    |                                      |\n");
        printf(" -------------------------------------------------------------------------------------------------------------------------------------------------------------------\n\n");
        printf("Opcion:  ");
        scanf("%d",&opc);
	
	return opc;
}


