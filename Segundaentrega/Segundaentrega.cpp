//bibliotecas
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

//variables globales
HINSTANCE hInstanceGlobal = 0;
HWND hHome = 0;

//declaracion de funciones
/*Productos* nuevoProducto(string nombre, int cantidad, string foto, string foto2, int codigoproducto, string marca, string descripcion, float monto);
Envios* nuevoEnvio(string direccion, string fechadeenvio, string estatusdelenvio);*/
Usuarios* nuevoUsuario(Usuarios* usuario);
void agregarproductoalinicio(Productos* producto);
void agregarproductoalmedio(Productos* producto, string nombre);
void agregarproductoalfinal(Productos* producto);
void borraralumnoalmedio(string nombre);
void borraralumnoalfinal();
void modificarproducto(string nombre, int cantidad, string foto, string foto2, int codigoproducto, string marca, string descripcion, float monto, string nombrecondicion);
void guardararchivoproductos();
void leerarchivoproductos();
void imprime(Productos* lista);
bool soloLetras(string);
bool fechaMenorAHoy(SYSTEMTIME hoy, SYSTEMTIME fecha);
/*wstring s2ws(const string& s) {
int len;
int slength = (int)s.length() + 1;
len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
wchar_t* buf = new wchar_t[len];
MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
std::wstring r(buf);
delete[] buf;
return r;
}*/
//callbacks
BOOL CALLBACK callback3(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
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
				MessageBox(0, "No eligio archivo", "Aviso", MB_OK | MB_ICONINFORMATION);
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
				MessageBox(0, "No eligio archivo", "Aviso", MB_OK | MB_ICONINFORMATION);
			}
			Productos* producto = new Productos;
			GetDlgItemText(hWnd, TXT_RUTAPRODUCTO2, producto->foto2, 255);
			static HBITMAP bmp = (HBITMAP)LoadImage(NULL, producto->foto2, IMAGE_BITMAP, 150, 200, LR_LOADFROMFILE);
			SendDlgItemMessage(hWnd, PC_PRODUCTO2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
			break;
		}
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
				MessageBox(0, "No eligio archivo ", "Aviso", MB_OK | MB_ICONINFORMATION);
			}
			Usuarios* usuario = new Usuarios;
			GetDlgItemText(hWnd, TXT_RUTAVENDEDOR, usuario->fotovendedor, 255);
			static HBITMAP bmp = (HBITMAP)LoadImage(NULL, usuario->fotovendedor, IMAGE_BITMAP, 150, 200, LR_LOADFROMFILE);
			SendDlgItemMessage(hWnd, PC_INFORMACIONVENDEDOR, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
			break;
		}
		case ID_PRODUCTOS_NUEVOPRODUCTO: {
			EndDialog(hWnd, 0);
			HWND NuevoProducto = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_NUEVOPRODUCTO), NULL, callback3);
			UpdateWindow(NuevoProducto);
			ShowWindow(NuevoProducto, SW_SHOW);
			break;
		}
		case ID_PRODUCTOS_ELIMINARPRODUCTO: {
			EndDialog(hWnd, 0);
			HWND EliminarProducto = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_ELIMINARPRODUCTO), NULL, callback3);
			UpdateWindow(EliminarProducto);
			ShowWindow(EliminarProducto, SW_SHOW);
			break;
		}
		case ID_PRODUCTOS_EDITARPRODUCTO: {
			EndDialog(hWnd, 0);
			HWND EditarProducto = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_EDITARPRODUCTO), NULL, callback3);
			UpdateWindow(EditarProducto);
			ShowWindow(EditarProducto, SW_SHOW);
			break;
		}
		case ID_PRODUCTOS_MISPRODUCTOS: {
			EndDialog(hWnd, 0);
			HWND MisProductos = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_MISPRODUCTOS), NULL, callback3);
			UpdateWindow(MisProductos);
			ShowWindow(MisProductos, SW_SHOW);
			break;
		}
		case ID_COMPRARPRODUCTOS: {
			EndDialog(hWnd, 0);
			HWND ComprarProductos = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_COMPRARPRODUCTOS), NULL, callback3);
			UpdateWindow(ComprarProductos);
			ShowWindow(ComprarProductos, SW_SHOW);
			break;
		}
		case ID_CANCELARUNENVIO: {
			EndDialog(hWnd, 0);
			HWND CancelarEnvio = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_CANCELARENVIO), NULL, callback3);
			UpdateWindow(CancelarEnvio);
			ShowWindow(CancelarEnvio, SW_SHOW);
			break;
		}
		case ID_EDITARUNENVIO: {
			EndDialog(hWnd, 0);
			HWND EditarEnvio = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_EDITARENVIO), NULL, callback3);
			UpdateWindow(EditarEnvio);
			ShowWindow(EditarEnvio, SW_SHOW);
			break;
		}
		case ID_MISENVIOS: {
			EndDialog(hWnd, 0);
			HWND MisEnvios = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_MISENVIOS), NULL, callback3);
			UpdateWindow(MisEnvios);
			ShowWindow(MisEnvios, SW_SHOW);
			break;
		}
		case ID_SALIRMENU: {
			HWND SalirMenu = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_SALIR), NULL, callback3);
			UpdateWindow(SalirMenu);
			ShowWindow(SalirMenu, SW_SHOW);
			break;
		}
		case BTN_SALIR2: {
			PostQuitMessage(0); //Salir
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
		case IDREGRESAR: {
			EndDialog(hWnd, 0);
			ShowWindow(hHome,SW_RESTORE);
			break;
		}
		case IDREGISTRAR: {
			Usuarios* usuario = new Usuarios;
			GetDlgItemText(hWnd, TXT_USUARIOREGISTRO, usuario->nombreusuario, 255);
			GetDlgItemText(hWnd, TXT_PASSWORDREGISTRO, usuario->contraseña, 255);
			if (string(usuario->nombreusuario) == ("")) {
				MessageBox(NULL, "Nombre vacio", "Error", MB_OK);
			}
			else if (string(usuario->contraseña) == ("")) {
				MessageBox(NULL, "Contraseña vacia", "Error", MB_OK);
			}
			else {
				bool esLetras=soloLetras(usuario->nombreusuario);
				if (soloLetras(usuario->nombreusuario) == true) {
					Usuarios* nuevoUsuario(Usuarios * usuario); //Se guardan los datos del usuario
					MessageBox(NULL, "Ya puedes empezar a comprar o vender", "Registro completado", MB_OK);
				}
				else {
					MessageBox(NULL, "El nombre de usuario solo debe contener letras", "Error", MB_OK);
				}
			}
			//MessageBox(hWnd, usuario->nombreusuario, "Nombre de usuario", 0);
	     	//MessageBox(hWnd, usuario->contraseña, "Contraseña", 0);
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
			EndDialog(hWnd, 0);
			//GetWindowText(hWnd, LPTSTR lpString, 255);
			HWND IniciarSesion = CreateDialog(hInstanceGlobal, MAKEINTRESOURCE(DLG_INFORMACIONVENDEDOR), NULL, callback3);
			ShowWindow(IniciarSesion, SW_SHOW);
			UpdateWindow(IniciarSesion);
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
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
} 

//funciones
/*Productos* nuevoProducto(string nombre, int cantidad, string foto, string foto2, int codigoproducto, string marca, string descripcion, float monto) {
	Productos* producto = new Productos;
	strcpy_s(producto->nombre, 255, nombre.c_str());
	strcpy_s(producto->foto, 255, foto.c_str());
	strcpy_s(producto->foto2, 255, foto2.c_str());
	producto->cantidad = cantidad;
	producto->codigoproducto = codigoproducto;
	strcpy_s(producto->marca, 255, marca.c_str());
	strcpy_s(producto->descripcion, 255, descripcion.c_str());
	producto->monto = monto;
	producto->anterior = NULL;
	producto->siguiente = NULL;
	return producto;
}
Envios* nuevoEnvio(string direccion, string fechadeenvio, string estatusdelenvio) {
	Envios* envio = new Envios;
	strcpy_s(envio->direccion, 255, direccion.c_str());
	strcpy_s(envio->fechadeenvio, 255, fechadeenvio.c_str());
	strcpy_s(envio->estatusdelenvio, 255, estatusdelenvio.c_str());
	envio->anterior = NULL;
	envio->siguiente = NULL;
	return envio;
}*/
Usuarios* nuevoUsuario(Usuarios*usuario) {
	/*strcpy_s(usuario->nombreusuario, 255, nombreusuario.c_str());
	strcpy_s(usuario->contraseña, 255, contraseña.c_str());
	strcpy_s(usuario->nombrevendedor, 255, nombrevendedor.c_str());
	strcpy_s(usuario->nombrevendedor, 255, nombrevendedor.c_str());
	strcpy_s(usuario->aliasempresa, 255, aliasempresa.c_str());
	strcpy_s(usuario->fotovendedor, 255, fotovendedor.c_str());*/
	if (origenUsuarios == NULL) {
		origenUsuarios = usuario;
	}
	else {
		Usuarios* indice = origenUsuarios;
		while (indice->siguiente != NULL)
			indice = indice->siguiente;
		indice->siguiente = usuario;
		usuario->anterior = indice;
	}
	usuario->anterior = NULL;
	usuario->siguiente = NULL;
	return usuario;
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
void agregarproductoalfinal(Productos* producto) {
	if (origenProductos == NULL) {
		origenProductos = producto;
	}
	else {
		Productos* indice = origenProductos;
		while (indice->siguiente != NULL)
			indice = indice->siguiente;
		indice->siguiente = producto;
		producto->anterior = indice;
	}
}
void borraralumnoalmedio(string nombre) {
	if (origenProductos == NULL) {
		return;
	}
	else {
		Productos* indice = origenProductos;
		bool encontrado = false;
		while (indice != NULL) {
			if (strcpy_s(indice->nombre,nombre.c_str()) == 0) {
				encontrado = true;
				break;
			}
			indice = indice->siguiente;
		}
		if (encontrado) {
			if (indice->anterior == NULL) {
				origenProductos = indice->siguiente;
				origenProductos->anterior = NULL;
				delete indice;
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
void borraralumnoalfinal() {
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
void modificarproducto(string nombre, int cantidad, string foto, string foto2, int codigoproducto, string marca, string descripcion, float monto, string nombrecondicion) {
	if (origenProductos == NULL) {
		return;
	}
	else {
		Productos* indice = origenProductos;
		bool encontrado = false;
		while (indice != NULL) {
			if (strcpy_s(indice->nombre,nombrecondicion.c_str()) == 0) {
				encontrado = true;
				break;
			}
			indice = indice->siguiente;
		}
		if (encontrado) {
			strcpy_s(indice->nombre, nombre.c_str());
			indice->cantidad = cantidad;
			strcpy_s(indice->foto, foto.c_str());
			strcpy_s(indice->foto2, foto2.c_str());
			indice->codigoproducto = codigoproducto;
			strcpy_s(indice->marca, marca.c_str());
			strcpy_s(indice->descripcion, descripcion.c_str());
			indice->monto = monto;
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
			agregarproductoalfinal(lectura);
			bytes = bytes + sizeof(Productos);
		}
		variabledearchivo.close();
	}
	else {
		cout << "Error al abrir el archivo" << endl;
	}
}
void imprime(Productos* lista) {
	Productos* indice = lista;
	while (indice != NULL) {
		cout << "Nombre del producto: " << indice->nombre << endl;
		cout << "Cantidad: " << indice->cantidad << endl;
		cout << "Foto: " << indice->foto << endl;
		cout << "Foto2: " << indice->foto2 << endl;
		cout << "Codigo del producto: " << indice->codigoproducto << endl;
		cout << "Marca:" << indice->marca << endl;
		cout << "Descripcion:" << indice->descripcion << endl;
		cout << "Monto: " << indice->monto << endl;
		cout << "---------------------------" << endl;
		indice = indice->siguiente;
	}
}
bool soloLetras(string str) {
	int tam = str.length();
	for (int i = 0; i < tam; i++) {
		if ((str[i] < 'a' || str[i] > 'z') &&
			(str[i] < 'A' || str[i] > 'Z')) {
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