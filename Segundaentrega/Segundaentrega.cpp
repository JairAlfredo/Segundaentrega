#include <Windows.h>
#include <CommCtrl.h>
#include "Segundaentrega.h"

BOOL CALLBACK callback3(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam) {
	switch (mensaje) {
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_INFORMACIONVENDEDOR: {
			ShowWindow(handler, SW_HIDE); //Ocultar ventana anterior
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG3), NULL, (DLGPROC)callback3);
			ShowWindow(handler, SW_SHOW); //Mostrar ventana actual
			break;
		}
		case ID_PRODUCTOS_NUEVOPRODUCTO: {
			ShowWindow(handler, SW_HIDE);
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG8), NULL, (DLGPROC)callback3);
			ShowWindow(handler, SW_SHOW);
			break;
		}

		case ID_PRODUCTOS_ELIMINARPRODUCTO: {
			ShowWindow(handler, SW_HIDE);
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG9), NULL, (DLGPROC)callback3);
			ShowWindow(handler, SW_SHOW);
			break;
		}
		case ID_PRODUCTOS_EDITARPRODUCTO: {
			ShowWindow(handler, SW_HIDE);
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG10), NULL, (DLGPROC)callback3);
			ShowWindow(handler, SW_SHOW);
			break;
		}
		case ID_PRODUCTOS_MISPRODUCTOS: {
			ShowWindow(handler, SW_HIDE);
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG11), NULL, (DLGPROC)callback3);
			ShowWindow(handler, SW_SHOW);
			break;
		}
		case ID_COMPRARPRODUCTOS: {
			ShowWindow(handler, SW_HIDE);
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG7), NULL, (DLGPROC)callback3);
			ShowWindow(handler, SW_SHOW);
			break;
		}
		case ID_CANCELARUNENVIO: {
			ShowWindow(handler, SW_HIDE);
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG5), NULL, (DLGPROC)callback3);
			ShowWindow(handler, SW_SHOW);
			break;
		}
		case ID_EDITARUNENVIO: {
			ShowWindow(handler, SW_HIDE);
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG6), NULL, (DLGPROC)callback3);
			ShowWindow(handler, SW_SHOW);
			break;
		}
		case ID_MISENVIOS: {
			ShowWindow(handler, SW_HIDE);
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG4), NULL, (DLGPROC)callback3);
			ShowWindow(handler, SW_SHOW);
			break;
		}
		case ID_INFORMACI32774: {
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG12), NULL, (DLGPROC)callback3);
			break;
		}
		case IDCANCELAR: {
			ShowWindow(handler, SW_HIDE);
			break;
		}
		case IDSALIR2: {
			PostQuitMessage(0);
			break;
		}
		case IDC_CARGAR1: {
			OPENFILENAME cargarimagen;
			wchar_t direccion[200];
			ZeroMemory(&cargarimagen, sizeof(cargarimagen)); //limpiar
			cargarimagen.lStructSize = sizeof(cargarimagen);
			cargarimagen.hwndOwner = handler;
			cargarimagen.lpstrFile = direccion;
			cargarimagen.lpstrFilter = L"Mapa de Bits\0*.bmp";
			cargarimagen.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
			cargarimagen.lpstrFile[0] = NULL;
			cargarimagen.nMaxFile = sizeof(direccion);
			cargarimagen.nFilterIndex = 0;
			GetOpenFileName(&cargarimagen);
			if (GetOpenFileName(&cargarimagen) == TRUE) {
				HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, direccion, IMAGE_BITMAP, 200, 200, LR_LOADFROMFILE);
				SendDlgItemMessage(handler, IDC_STATIC, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
			}
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
BOOL CALLBACK callback2(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam) {
	switch (mensaje) {
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDREGRESAR: {
			ShowWindow(handler, SW_HIDE);
			break;
		}
		case IDREGISTRAR: {
			//Se guardan los datos del usuario
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
BOOL CALLBACK callback(HWND handler, UINT mensaje, WPARAM wParam, LPARAM lParam) {
	switch (mensaje) {
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDC_REGISTRARTE: {
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG2), NULL, (DLGPROC)callback2);
			ShowWindow(handler, SW_HIDE);
			break;
		}
		case IDC_BUTTON1: {
			ShowWindow(handler, SW_HIDE);
			//inicio* nuevo;
			//SendMessage(GetDlgItem(handler, IDC_EDIT1), WM_GETTEXT, sizeof(nuevo.nombre) / sizeof(nuevo.nombre[0]), (LPARAM)nuevo.nombre);
			//MessageBox(NULL, nuevo.nombre, L"TEXTO", MB_OK);
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG4), NULL, (DLGPROC)callback3);
			ShowWindow(handler, SW_SHOW);
			break;
		}
		case IDSALIR1: {
			ShowWindow(handler, SW_HIDE);
			DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG12), NULL, (DLGPROC)callback3);
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
int WINAPI WinMain(HINSTANCE instancia, HINSTANCE legado, LPSTR comando, int modo) {
	DialogBox(instancia, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)callback);
	return 0;
}
