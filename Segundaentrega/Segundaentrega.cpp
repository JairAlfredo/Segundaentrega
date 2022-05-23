//bibliotecas
#include <stdio.h>     
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include "resource.h"
using namespace std;

//estructuras
struct Productos {
	char nombre[255];
	int cantidad;
	char foto[255];
	char foto2[255];
	int codigoproducto;
	char marca[255];
	char descripcion[255];
	float monto;
	Productos* anterior;
	Productos* siguiente;
}*origenProductos;
struct Envios {
	char direccion[255];
	char fechadeenvio[255];
	char estatusdelenvio[255];
	Envios* anterior;
	Envios* siguiente;
}*origenEnvios;
struct Usuarios {
	char nombreusuario[255]; //(no se debe repetir nombre y solo debe contener letras)
	char contraseña[255]; //debe aparecer oculta
	char nombrevendedor[255];
	char aliasempresa[255];
	char fotovendedor[255]; //debe aparecer en todas las pantallas excepto la de salir, iniciar sesion y registrar
	Usuarios* anterior;
	Usuarios* siguiente;
}*origenUsuarios;
Usuarios usuarioLoggeado{};//para comparar usuario
//
//variables globales
HINSTANCE hInstanceGlobal = 0;
HWND hHome = 0;
Usuarios* usuario = new Usuarios;
//declaracion de funciones
Productos* nuevoProducto(Productos*producto);
Envios* nuevoEnvio(string direccion, string fechadeenvio, string estatusdelenvio);
Usuarios*nuevoUsuario(Usuarios* usuario);
bool buscarUsuario(string nombreusuario,string contraseña, string nombrecondicion,string contraseñacondicion);
bool buscarProducto(string nombre);
void agregarproductoalinicio(Productos* producto);
void agregarproductoalmedio(Productos* producto, string nombre);
/*void agregarproductoalfinal(Productos* producto);*/
void borrarproductoalmedio(string nombre);
void borrarproductoalfinal();
void modificarproducto(string nombre, int codigoproducto, string marca, string descripcion, float monto, string nombrecondicion);
void guardararchivoproductos();
void leerarchivoproductos();
/*void imprime(Productos* lista);*/
bool soloLetras(string);
bool fechaMenorAHoy(SYSTEMTIME hoy, SYSTEMTIME fecha);
//callbacks
BOOL CALLBACK callback3(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		Productos* indice = origenProductos;
		while (indice != NULL) {
			SendDlgItemMessage(hWnd, LB_PRODUCTOSDADOSDEALTA, LB_ADDSTRING, 0, (LPARAM)indice->nombre);
			indice = indice->siguiente;
		}
		break; 
	}
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		//Ventana informacion del vendedor
		case ID_INFORMACIONVENDEDOR: {
			EndDialog(hWnd, 0);
			HWND InformacionVendedor = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_INFORMACIONVENDEDOR), NULL, callback3);
			UpdateWindow(InformacionVendedor);
			ShowWindow(InformacionVendedor, SW_SHOW);
			break;
		}
		case BTN_CARGARIMAGEN: {
			OPENFILENAME ofn; // common dialog box structure
			char szFile[260]; // buffer for file name
			HANDLE hf; // file handle
			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			// Set lpstrFile[0] to 0' so that GetOpenFileName does not
			// use the contents of szFile to initialize itself
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "Bitmap\0 * .bmp";
			ofn.nFilterIndex = 2;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&ofn) == true) {
				SetWindowText(GetDlgItem(hWnd, TXT_RUTAVENDEDOR), ofn.lpstrFile);
			}
			else {
				MessageBox(0, "No eligio archivo ", "Error", MB_OK | MB_ICONINFORMATION);
			}
			GetDlgItemText(hWnd, TXT_RUTAVENDEDOR, usuario->fotovendedor, 255);
			static HBITMAP bmp = (HBITMAP)LoadImage(NULL, usuario->fotovendedor, IMAGE_BITMAP, 150, 200, LR_LOADFROMFILE);
			SendDlgItemMessage(hWnd, PC_INFORMACIONVENDEDOR, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
			break;
		}	
		case BTN_ALTAVENDEDOR: {
			GetDlgItemText(hWnd, TXT_NOMBREDELVENDEDOR, usuario->nombrevendedor, 255);
			GetDlgItemText(hWnd, TXT_ALIASDELAEMPRESA, usuario->aliasempresa, 255);
			if (string(usuario->nombrevendedor) == ("") && string(usuario->aliasempresa) == ("")) {
				MessageBox(NULL, "Nombre del vendedor y alias de la empresa vacios", "Error", MB_OK | MB_ICONINFORMATION);
			}
			else if (string(usuario->nombrevendedor) == ("")) {
				MessageBox(NULL, "Nombre del vendedor vacio", "Error", MB_OK | MB_ICONINFORMATION);
			}
			else if (string(usuario->aliasempresa) == ("")) {
				MessageBox(NULL, "Alias de la empresa vacio", "Error", MB_OK | MB_ICONINFORMATION);
			}
			else {
				bool esLetras = soloLetras(usuario->nombrevendedor);
				if (soloLetras(usuario->nombrevendedor) == true) {
					nuevoUsuario(usuario); //Se guardan los datos del usuario
					MessageBox(NULL, "Ya puedes empezar a comprar o vender", "Aviso", MB_OK);
				}
				else {
					MessageBox(NULL, "El nombre del vendedor solo debe contener letras", "Error", MB_OK | MB_ICONINFORMATION);
				}
			}
			break;
		}
		 //Ventana nuevo producto
		case ID_PRODUCTOS_NUEVOPRODUCTO: {
			EndDialog(hWnd, 0);
			HWND NuevoProducto = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_NUEVOPRODUCTO), NULL, callback3);
			UpdateWindow(NuevoProducto);
			ShowWindow(NuevoProducto, SW_SHOW);
			break;
		}
		case BTN_CARGARIMAGEN2: {
			OPENFILENAME ofn; // common dialog box structur e
			char szFile[260]; // buffer for file name
			HANDLE hf; // file handle
			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			// Set lpstrFile[0] to 0' so that GetOpenFileName does not
			// use the contents of szFile to initialize itself
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "Bitmap\0 * .bmp";
			ofn.nFilterIndex = 2;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&ofn) == true) {
				SetWindowText(GetDlgItem(hWnd, TXT_RUTAPRODUCTO), ofn.lpstrFile);
			}
			else {
				MessageBox(0, "No eligio archivo", "Error", MB_OK | MB_ICONINFORMATION);
			}
			Productos* producto = new Productos;
			GetDlgItemText(hWnd, TXT_RUTAPRODUCTO, producto->foto, 255);
			static HBITMAP bmp = (HBITMAP)LoadImage(NULL, producto->foto, IMAGE_BITMAP, 150, 200, LR_LOADFROMFILE);
			SendDlgItemMessage(hWnd, PC_PRODUCTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
			break;
		}
		case BTN_CARGARIMAGEN3: {
			OPENFILENAME ofn; // common dialog box structur e
			char szFile[260]; // buffer for file name
			HANDLE hf; // file handle
			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			// Set lpstrFile[0] to 0' so that GetOpenFileName does not
			// use the contents of szFile to initialize itself
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "Bitmap\0 * .bmp";
			ofn.nFilterIndex = 2;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if (GetOpenFileName(&ofn) == true) {
				SetWindowText(GetDlgItem(hWnd, TXT_RUTAPRODUCTO2), ofn.lpstrFile);
			}
			else {
				MessageBox(0, "No eligio archivo", "Error", MB_OK | MB_ICONINFORMATION);
			}
			Productos* producto = new Productos;
			GetDlgItemText(hWnd, TXT_RUTAPRODUCTO2, producto->foto2, 255);
			static HBITMAP bmp = (HBITMAP)LoadImage(NULL, producto->foto2, IMAGE_BITMAP, 150, 200, LR_LOADFROMFILE);
			SendDlgItemMessage(hWnd, PC_PRODUCTO2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
			break;
		}
		case BTN_AGREGARPRODUCTO: {
		Productos* producto= new Productos;
		int num;
		int num2;
		float num3;
			char cantidad[255], codigoproducto[255], monto[255];
			GetDlgItemText(hWnd, TXT_NOMBREDELPRODUCTO, producto->nombre, 255);
			GetDlgItemText(hWnd, TXT_CANTIDADENINVENTARIO, cantidad, 255);
			GetDlgItemText(hWnd, TXT_RUTAPRODUCTO, producto->foto, 255);
			GetDlgItemText(hWnd, TXT_RUTAPRODUCTO2, producto->foto2, 255);
			GetDlgItemText(hWnd, TXT_CODIGODELPRODUCTO, codigoproducto, 255);
			GetDlgItemText(hWnd, TXT_MARCA, producto->marca, 255);
			GetDlgItemText(hWnd, TXT_DESCRIPCION, producto->descripcion, 255);
			GetDlgItemText(hWnd, TXT_MONTO, monto, 255);
			num= atoi(cantidad);
			num2 = atoi(codigoproducto);
			num3= atof(monto);
			producto->cantidad = num;
			producto->codigoproducto = num2;
			producto->monto = num3;
			nuevoProducto(producto);
			MessageBox(NULL, "Producto agregado", "Aviso", MB_OK);
			break;
		}
		//Ventana eliminar producto
		case ID_PRODUCTOS_ELIMINARPRODUCTO: {
			EndDialog(hWnd, 0);
			HWND EliminarProducto = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_ELIMINARPRODUCTO), NULL, callback3);
			UpdateWindow(EliminarProducto);
			ShowWindow(EliminarProducto, SW_SHOW);
			break;
		}
		case BTN_ELIMINARPRODUCTO: {
			char nombre[255];
			GetDlgItemText(hWnd, TXT_BUSCARNOMBRE, nombre, 255);
			borrarproductoalmedio(nombre);
			break;
		}
		case BTN_MOSTRAR: {
			char nombre[255];
			GetDlgItemText(hWnd, TXT_BUSCARNOMBRE, nombre, 255);
			bool encontrarProducto = buscarProducto(nombre);
			if (buscarProducto(nombre) ==true) {
				Productos* indice = origenProductos;
				while (indice != NULL) {
					char cantidadeditada[255], codigoeditado[255], montoeditado[255];
					 _itoa_s(indice->cantidad, cantidadeditada,2);
					 _itoa_s(indice->codigoproducto, codigoeditado,2 );
					_gcvt_s(montoeditado,255,indice->monto, 12);
					SendDlgItemMessage(hWnd, LB_CARACTERISTICAS, LB_ADDSTRING, 0, (LPARAM)indice->nombre); 
					  SendDlgItemMessage(hWnd, LB_CARACTERISTICAS, LB_ADDSTRING, 0, (LPARAM)cantidadeditada);
					  SendDlgItemMessage(hWnd, LB_CARACTERISTICAS, LB_ADDSTRING, 0, (LPARAM)codigoeditado);
					  SendDlgItemMessage(hWnd, LB_CARACTERISTICAS, LB_ADDSTRING, 0, (LPARAM)indice->marca);
					  SendDlgItemMessage(hWnd, LB_CARACTERISTICAS, LB_ADDSTRING, 0, (LPARAM)indice->descripcion);
					  SendDlgItemMessage(hWnd, LB_CARACTERISTICAS, LB_ADDSTRING, 0, (LPARAM)montoeditado);
					indice = indice->siguiente;
				}
			}
			else {
				MessageBox(0, "No se encuentra el producto ", "Error", MB_OK | MB_ICONINFORMATION);
			}
			break;
		}
		//Ventana  editar producto
		case ID_PRODUCTOS_EDITARPRODUCTO: {
			EndDialog(hWnd, 0);
			HWND EditarProducto = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_EDITARPRODUCTO), NULL, callback3);
			UpdateWindow(EditarProducto);
			ShowWindow(EditarProducto, SW_SHOW);
			break;
		}
		case BTN_CONFIRMAREDICION: {
			char nombreoriginal[255], nombreeditado[255],codigoeditado[255],marcaeditada[255],descripcioneditada[255],montoeditado[255];
			GetDlgItemText(hWnd, TXT_EDITARPRODUCTO, nombreoriginal, 255);
			GetDlgItemText(hWnd, TXT_NOMBREEDITADO, nombreeditado, 255);
			GetDlgItemText(hWnd, TXT_CODIGOEDITADO, codigoeditado, 255);
			GetDlgItemText(hWnd, TXT_MARCAEDITADA, marcaeditada, 255);
			GetDlgItemText(hWnd, TXT_DESCRIPCIONEDITADA, descripcioneditada, 255);
			GetDlgItemText(hWnd, TXT_MONTOEDITADO, montoeditado, 255);
			int num;
			float num2;
			num = atoi(codigoeditado);
			num2 = atof(montoeditado);
			modificarproducto(nombreeditado, num,marcaeditada, descripcioneditada, num2, nombreoriginal);
			//string nombre, int codigoproducto, string marca, string descripcion, float monto, string nombrecondicion
			break;
		}
		//Ventana mostrar productos
		case ID_PRODUCTOS_MISPRODUCTOS: {
			EndDialog(hWnd, 0);
			HWND MisProductos = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_MISPRODUCTOS), NULL, callback3);
			UpdateWindow(MisProductos);
			ShowWindow(MisProductos, SW_SHOW);
			break;
		}
		case LB_PRODUCTOSDADOSDEALTA: {
			if (HIWORD(wParam) == LBN_SELCHANGE) {
				char nombre[255];
				SendDlgItemMessage(hWnd, LB_PRODUCTOSDADOSDEALTA, LB_GETTEXT, 0, (LPARAM)nombre);
				bool encontrarProducto=buscarProducto(nombre);
				if(buscarProducto(nombre)==true){
			    Productos* indice = origenProductos;
				bool encontrado = false;
				while (indice != NULL) {
					if (strcmp(indice->nombre, nombre) == 0) {
						encontrado = true;
						break;
					}
					indice = indice->siguiente;
				}
				if (encontrado) {
					char cantidadeditada[255], codigoeditado[255], montoeditado[255];
					_itoa_s(indice->cantidad, cantidadeditada, 2);
					_itoa_s(indice->codigoproducto, codigoeditado, 2);
					_gcvt_s(montoeditado, 255, indice->monto, 12);
					SendDlgItemMessage(hWnd, LB_CARACTERISTICAS2, LB_ADDSTRING, 0, (LPARAM)cantidadeditada);
					SendDlgItemMessage(hWnd, LB_CARACTERISTICAS2, LB_ADDSTRING, 0, (LPARAM)codigoeditado);
					SendDlgItemMessage(hWnd, LB_CARACTERISTICAS2, LB_ADDSTRING, 0, (LPARAM)indice->marca);
					SendDlgItemMessage(hWnd, LB_CARACTERISTICAS2, LB_ADDSTRING, 0, (LPARAM)indice->descripcion);
					SendDlgItemMessage(hWnd, LB_CARACTERISTICAS2, LB_ADDSTRING, 0, (LPARAM)montoeditado);
				}
				}
			}
			break; 
		}
		//Ventana comprar productos
		case ID_COMPRARPRODUCTOS: {
			EndDialog(hWnd, 0);
			HWND ComprarProductos = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_COMPRARPRODUCTOS), NULL, callback3);
			UpdateWindow(ComprarProductos);
			ShowWindow(ComprarProductos, SW_SHOW);
			break;
		}
		//Ventana cancelar envio
		case ID_CANCELARUNENVIO: {
			EndDialog(hWnd, 0);
			HWND CancelarEnvio = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_CANCELARENVIO), NULL, callback3);
			UpdateWindow(CancelarEnvio);
			ShowWindow(CancelarEnvio, SW_SHOW);
			break;
		}
        //Editar envio
		case ID_EDITARUNENVIO: {
			EndDialog(hWnd, 0);
			HWND EditarEnvio = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_EDITARENVIO), NULL, callback3);
			UpdateWindow(EditarEnvio);
			ShowWindow(EditarEnvio, SW_SHOW);
			break;
		}
		//Mostrar envios
		case ID_MISENVIOS: {
			EndDialog(hWnd, 0);
			HWND MisEnvios = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_MISENVIOS), NULL, callback3);
			UpdateWindow(MisEnvios);
			ShowWindow(MisEnvios, SW_SHOW);
			break;
		}
		//Ventana salida
		case ID_SALIRMENU: {
			HWND SalirMenu = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_SALIR), NULL, callback3);
			UpdateWindow(SalirMenu);
			ShowWindow(SalirMenu, SW_SHOW);
			break;
		}
		case BTN_SALIR2: {
			PostQuitMessage(0); //Se deben guardar los cambios realizados 
			break;
		}
		case BTN_CANCELAR: {
			EndDialog(hWnd, 0);
			break;
		}
		default:
			break;
		}
		break;
	}
	case WM_DESTROY: { //Se envia cuando una ventana está siendo destruida
		PostQuitMessage(0);
		break;
	}
	default:
		break;
	}
	return false;
}
BOOL CALLBACK cRegistroUsuario(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case BTN_REGRESAR: {
			EndDialog(hWnd, 0);
			ShowWindow(hHome,SW_RESTORE);
			break;
		}
		case BTN_REGISTRAR: {
			GetDlgItemText(hWnd, TXT_USUARIOREGISTRO, usuario->nombreusuario, 255);
			GetDlgItemText(hWnd, TXT_PASSWORDREGISTRO, usuario->contraseña, 255);
			if (string(usuario->nombreusuario) == ("")&& string(usuario->contraseña) == ("")) {
			MessageBox(NULL, "Nombre de usuario y contraseña vacios", "Error", MB_OK | MB_ICONINFORMATION);
			}
			else if (string(usuario->nombreusuario) == ("")) {
				MessageBox(NULL, "Nombre vacio", "Error", MB_OK | MB_ICONINFORMATION);
			}
			else if (string(usuario->contraseña) == ("")) {
				MessageBox(NULL, "Contraseña vacia", "Error", MB_OK | MB_ICONINFORMATION);
			}
			else {
				bool esLetras=soloLetras(usuario->nombreusuario);
				if (soloLetras(usuario->nombreusuario) == true) {
				nuevoUsuario(usuario); //Se guardan los datos del usuario
					MessageBox(NULL, "Se ha generado su cuenta", "Aviso", MB_OK);
				}
				else {
					MessageBox(NULL, "El nombre de usuario solo debe contener letras", "Error", MB_OK | MB_ICONINFORMATION);
				}
			}
			break;
		}
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
	return false;
}
BOOL CALLBACK cHome(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) { //HWND Console Window Handler
	switch (msg) {
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case BTN_REGISTRARTE: {
			EndDialog(hWnd, 0);
			HWND RegistroUsuario = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_REGISTROUSUARIO), NULL, cRegistroUsuario);
			ShowWindow(RegistroUsuario ,SW_SHOW);
			UpdateWindow(RegistroUsuario);
			break;
		}
		case BTN_INICIARSESION: {
			char nombrecondicion[255];
			char contraseñacondicion[255];
			GetDlgItemText(hWnd, TXT_USUARIOINICIAR, nombrecondicion, 255);
			GetDlgItemText(hWnd, TXT_PASSWORDINICIAR, contraseñacondicion, 255);
			bool encontrarUsuario=buscarUsuario(usuario->nombreusuario, usuario->contraseña, nombrecondicion, contraseñacondicion);
			if (buscarUsuario(usuario->nombreusuario, usuario->contraseña, nombrecondicion, contraseñacondicion) == true) {
				EndDialog(hWnd, 0);
				HWND IniciarSesion = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_INFORMACIONVENDEDOR), NULL, callback3);
				ShowWindow(IniciarSesion, SW_SHOW);
				UpdateWindow(IniciarSesion);
			}
			else {
				MessageBox(0, "Datos incorrectos", "Error", MB_OK | MB_ICONINFORMATION);
			}
			break;
		}
		case BTN_SALIR1: {
			HWND Salir = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_SALIR), NULL, callback3);
			ShowWindow(Salir, SW_SHOW);
			UpdateWindow(Salir);
			break;
		}
		default:
			break;
		}
		break;
	}
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}
	default:
		break;
	}
	return false;
} 
//funcion main
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmd, int showCmd) { //Función principal}
	hInstanceGlobal = hInstance;
	hHome = CreateDialog(hInstance, MAKEINTRESOURCE(DLG_INICIARSESION), NULL, cHome);
	//Variable para manejar los mensajes
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	//Mostrar la ventana
	ShowWindow(hHome, showCmd);
	UpdateWindow(hHome);
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
} 

//funciones
Productos* nuevoProducto(Productos*producto) {
	if (origenProductos == NULL) {
		origenProductos =producto;
		producto->anterior = NULL;
	}
	else {
	Productos* indice = origenProductos;
		while (indice->siguiente != NULL)
		indice = indice->siguiente;
		indice->siguiente = producto;
		producto->anterior = indice;
	}
	producto->siguiente = NULL;
	return producto;
}
Envios* nuevoEnvio(string direccion, string fechadeenvio, string estatusdelenvio) {
	Envios* envio = new Envios;
	strcpy_s(envio->direccion, 255, direccion.c_str());
	strcpy_s(envio->fechadeenvio, 255, fechadeenvio.c_str());
	strcpy_s(envio->estatusdelenvio, 255, estatusdelenvio.c_str());
	if (origenEnvios == NULL) {
		origenEnvios = envio;
		envio->anterior = NULL;
	}
	else {
		Envios* indice = origenEnvios;
		while (indice->siguiente != NULL)
			indice = indice->siguiente;
		indice->siguiente = envio;
		envio->anterior = indice;
	}
	envio->siguiente = NULL;
	return envio;
}
Usuarios* nuevoUsuario(Usuarios* usuario) {
	if (origenUsuarios == NULL) {
		origenUsuarios = usuario;
		usuario->anterior = NULL;
	}
	else {
		Usuarios* indice = origenUsuarios;
		while (indice->siguiente != NULL)
			indice = indice->siguiente;
		indice->siguiente = usuario;
		usuario->anterior = indice;
	}
	usuario->siguiente = NULL;
	return usuario;
}
bool buscarUsuario(string nombreusuario, string contraseña,string nombrecondicion, string contraseñacondicion) {
	Usuarios* indice = origenUsuarios;
	while (indice != NULL) {
		if (strcmp(indice->nombreusuario, nombrecondicion.c_str()) == 0 && strcmp(indice->contraseña, contraseñacondicion.c_str()) == 0) {
			return true;
			break;
		}
		indice = indice->siguiente;
	}
	return false;
}
bool buscarProducto(string nombre) {
	if (origenProductos == NULL) {
		return false;
	}
	else{
		Productos* indice = origenProductos;
		while (indice != NULL) {
			if (strcmp(indice->nombre, nombre.c_str()) == 0) {
				return true;
				break;
			}
			indice = indice->siguiente;
		}
		return false;
	}
}
void agregarproductoalinicio(Productos* producto) {
	if (origenProductos == NULL) {
		origenProductos = producto;
	}
	else {
		Productos* indice = origenProductos;
		origenProductos = producto;
		indice->anterior = producto;
		producto->siguiente = indice;
		producto->anterior = NULL;
	}

}
void agregarproductoalmedio(Productos* producto, string nombre) {
	if (origenProductos == NULL) {
		origenProductos = producto;
	}
	else {
		Productos* indice = origenProductos;
		bool encontrado = false;
		while (indice->siguiente != NULL) {
			if (_strcmpi(indice->nombre,nombre.c_str()) == 0) {
				encontrado = true;
				break;
			}
			indice = indice->siguiente;
		}
		if (encontrado) {
			Productos* anterior = indice->anterior;
			if (anterior != NULL) {
				anterior->siguiente = producto;
				producto->siguiente = indice;
				producto->anterior = anterior;
				indice->anterior = producto;
			}
			else {
				agregarproductoalinicio(producto);
			}
		}
	}
}
void borrarproductoalmedio(string nombre) {
	if (origenProductos == NULL) {
		MessageBox(0, "No se encuentra el producto ", "Error", MB_OK | MB_ICONINFORMATION);
		return;
	}
	else {
		Productos* indice = origenProductos;
		bool encontrado = false;
		while (indice != NULL) {
			if (strcmp(indice->nombre,nombre.c_str()) == 0) {
				encontrado = true;
				break;
			}
			indice = indice->siguiente;
		}
		if (encontrado) {
			if (indice->anterior == NULL) {
				delete indice;
				origenProductos = NULL;
			}
			else {
				if (indice->siguiente == NULL) {
					Productos* productoanterior = indice->anterior;
					productoanterior->siguiente = NULL;
					delete indice;
				}
				else {
					Productos* productoanterior = indice->anterior;
					Productos* productosiguiente = indice->siguiente;
					productoanterior->siguiente = productosiguiente;
					productosiguiente->anterior = productoanterior;
					delete indice;
				}
			}
		}
	}
}
void borrarproductoalfinal() {
	if (origenProductos == NULL) {
		return;
	}
	else {
		Productos* indice = origenProductos;
		while (indice->siguiente != NULL) {
			indice = indice->siguiente;
		}
		Productos* anterior = indice->anterior;
		if (anterior == NULL) {
			delete indice;
			origenProductos = NULL;
		}
		else {
			anterior->siguiente = NULL;
			delete indice;
		}
	}
}
void modificarproducto(string nombre, int codigoproducto, string marca, string descripcion, float monto, string nombrecondicion) {
	if (origenProductos == NULL) {
		MessageBox(0, "No se encuentra el producto ", "Error", MB_OK | MB_ICONINFORMATION);
		return;
	}
	else {
		Productos* indice = origenProductos;
		bool encontrado = false;
		while (indice != NULL) {
			if (strcmp(indice->nombre,nombrecondicion.c_str()) == 0) {
				encontrado = true;
				break;
			}
			indice = indice->siguiente;
		}
		if (encontrado) {
			strcpy_s(indice->nombre, nombre.c_str());
			indice->codigoproducto = codigoproducto;
			strcpy_s(indice->marca, marca.c_str());
			strcpy_s(indice->descripcion, descripcion.c_str());
			indice->monto = monto;
			MessageBox(0, "El producto ha sido modificado", "Aviso", MB_OK);
		}
	}

}
void guardararchivoproductos() {
	ofstream variablearchivo;
	variablearchivo.open("C:\\Users\\Usuario\\source\\repos\\Primeravance\\productos.bin", ios::binary | ios::trunc);
	if (variablearchivo.is_open()) {
		Productos* indice = origenProductos;
		while (indice != NULL) {
			variablearchivo.write((char*)indice, sizeof(Productos));
			indice = indice->siguiente;
		}
		variablearchivo.close();
	}
	else {
		cout << "No se pudo abrir el archivo" << endl;
	}
}
void leerarchivoproductos() {
	ifstream variabledearchivo;
	variabledearchivo.open("C:\\Users\\Usuario\\source\\repos\\Primeravance\\productos.bin", ios::binary | ios::in);
	if (variabledearchivo.is_open()) {
		variabledearchivo.seekg(0, variabledearchivo.end);
		int pesoarchivo = variabledearchivo.tellg();
		variabledearchivo.seekg(0, variabledearchivo.beg);
		int bytes = 0;
		while (bytes < pesoarchivo) {
			Productos* lectura = new Productos;
			variabledearchivo.read(reinterpret_cast<char*>(lectura), sizeof(Productos));
			lectura->siguiente = NULL;
			lectura->anterior = NULL;
			nuevoProducto(lectura);
			bytes = bytes + sizeof(Productos);
		}
		variabledearchivo.close();
	}
	else {
		cout << "Error al abrir el archivo" << endl;
	}
}
/*void imprime(Productos* lista) {
	Productos* indice = lista;
	while (indice != NULL) {
		cout << "Nombre del producto: " << indice->nombre << endl;
		cout << "Cantidad: " << indice->cantidad << endl;
		cout << "Codigo del producto: " << indice->codigoproducto << endl;
		cout << "Marca:" << indice->marca << endl;
		cout << "Descripcion:" << indice->descripcion << endl;
		cout << "Monto: " << indice->monto << endl;
		cout << "---------------------------" << endl;
		indice = indice->siguiente;
	}
}*/
bool soloLetras(string str) {
	int tam = str.length();
	for (int i = 0; i < tam; i++) {
		if ((str[i] < 'a' || str[i] > 'z') &&(str[i] < 'A' || str[i] > 'Z') && (str[i] < ' ' || str[i] > ' ') ) {
			return false;
		}
	}
	return true;
}
bool fechaMenorAHoy(SYSTEMTIME hoy, SYSTEMTIME fecha) {
	if (fecha.wYear < hoy.wYear) {
		return true;
	}
	else if (fecha.wYear == hoy.wYear && fecha.wMonth < hoy.wMonth) {
		return true;
	}
	else if (fecha.wYear == hoy.wYear && fecha.wMonth == hoy.wMonth && fecha.wDay < hoy.wDay) {
		return true;
	}

	return false;
}
