/*----------------------------------------------------------
OpenKey - The Cross platform Open source Vietnamese Keyboard application.

Copyright (C) 2019 Mai Vu Tuyen
Contact: maivutuyen.91@gmail.com
Github: https://github.com/tuyenvm/OpenKey
Fanpage: https://www.facebook.com/OpenKeyVN

This file is belong to the OpenKey project, Win32 version
which is released under GPL license.
You can fork, modify, improve this program. If you
redistribute your new version, it MUST be open source.
-----------------------------------------------------------*/
#include "MacroDialog.h"
#include "stdafx.h"
#include "AppDelegate.h"
#include <commdlg.h>

#define MAX_MACRO_BUFFER 4096

// ========================== CreateMacroDialog ==========================

static CreateMacroDialog* g_pCreateMacroDialog = nullptr;

INT_PTR CALLBACK CreateMacroDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_INITDIALOG) {
		g_pCreateMacroDialog = (CreateMacroDialog*)lParam;
	}
	if (g_pCreateMacroDialog) {
		return g_pCreateMacroDialog->eventProc(hDlg, uMsg, wParam, lParam);
	}
	return FALSE;
}

CreateMacroDialog::CreateMacroDialog(const HINSTANCE & hInstance, const int & resourceId)
	: hInstance(hInstance), resId(resourceId) {
	currentImagePath = "";
	hMacroName = nullptr;
	hMacroContent = nullptr;
	hSelectImageButton = nullptr;
	hRemoveImageButton = nullptr;
	hImagePreview = nullptr;
	hImagePreview = nullptr;
	hImageName = nullptr;
	initName = "";
	initContent = "";
	savedName = "";
	savedContent = "";
}

CreateMacroDialog::~CreateMacroDialog() {
}

INT_PTR CreateMacroDialog::eventProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
		initDialog(hDlg);
		return TRUE;
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		switch (wmId) {
		case IDC_BUTTON_SELECT_IMAGE:
			onSelectImageButton(hDlg);
			break;
		case IDC_BUTTON_REMOVE_IMAGE:
			onRemoveImageButton();
			break;
		case IDOK: {
			TCHAR buffer[MAX_MACRO_BUFFER];
			GetWindowText(hMacroName, buffer, MAX_MACRO_BUFFER);
			savedName = wideStringToUtf8(buffer);
			GetWindowText(hMacroContent, buffer, MAX_MACRO_BUFFER);
			savedContent = wideStringToUtf8(buffer);
			EndDialog(hDlg, IDOK);
		}
			break;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			break;
		}
		break;
	}
	}
	return FALSE;
}

void CreateMacroDialog::initDialog(HWND hDlg) {
	hMacroName = GetDlgItem(hDlg, IDC_EDIT_MACRO_NAME);
	hMacroContent = GetDlgItem(hDlg, IDC_EDIT_MACRO_CONTENT);
	hSelectImageButton = GetDlgItem(hDlg, IDC_BUTTON_SELECT_IMAGE);
	hRemoveImageButton = GetDlgItem(hDlg, IDC_BUTTON_REMOVE_IMAGE);
	hImagePreview = GetDlgItem(hDlg, IDC_STATIC_IMAGE_PREVIEW);
	hImageName = GetDlgItem(hDlg, IDC_STATIC_IMAGE_NAME);
	
	SetWindowText(hMacroName, utf8ToWideString(initName).c_str());
	SetWindowText(hMacroContent, utf8ToWideString(initContent).c_str());

	updateImagePreview();
	SetDlgItemText(hDlg, IDOK, utf8ToWideString("Lưu").c_str());
	ShowWindow(hImagePreview, SW_HIDE);
}

void CreateMacroDialog::setMacroData(string name, string content, string imagePath) {
	initName = name;
	initContent = content;
	savedName = name;       // Initialize saved data
	savedContent = content; // Initialize saved data
	currentImagePath = imagePath;
}

void CreateMacroDialog::onSelectImageButton(HWND hDlg) {
	OPENFILENAME ofn;
	const int BUFFER_SIZE = 10000;
	TCHAR* szFile = new TCHAR[BUFFER_SIZE]; // Allocate large buffer for multiple files
	if (szFile) memset(szFile, 0, BUFFER_SIZE * sizeof(TCHAR));

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hDlg;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = BUFFER_SIZE;
	ofn.lpstrFilter = _T("Image files (*.png;*.jpg;*.jpeg;*.bmp;*.gif)\0*.png;*.jpg;*.jpeg;*.bmp;*.gif\0All (*.*)\0*.*\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	if (GetOpenFileName(&ofn) == TRUE) {
		wchar_t* p = szFile;
		wstring dir = p;
		p += dir.length() + 1;

		if (*p == 0) {
			// Single file selected
			currentImagePath = wideStringToUtf8(szFile);
		} else {
			// Multiple files selected
			// Ensure dir ends with backslash
			if (!dir.empty() && dir.back() != L'\\') dir += L"\\";

			string multiPaths = "";
			while (*p) {
				wstring filename = p;
				wstring fullPath = dir + filename;
				if (!multiPaths.empty()) multiPaths += "|";
				multiPaths += wideStringToUtf8(fullPath.c_str());
				p += filename.length() + 1;
			}
			currentImagePath = multiPaths;
		}
		
		updateImagePreview();
	}
	delete[] szFile;
}

void CreateMacroDialog::onRemoveImageButton() {
	currentImagePath = "";
	updateImagePreview();
}

void CreateMacroDialog::updateImagePreview() {
	if (currentImagePath.empty()) {
		SetWindowText(hImageName, utf8ToWideString("(Chưa chọn ảnh)").c_str());
	} else {
		if (currentImagePath.find('|') != string::npos) {
			// Multiple images
			int count = 1;
			for (char c : currentImagePath) {
				if (c == '|') count++;
			}
			wstring msg = utf8ToWideString("Đã chọn ") + to_wstring(count) + utf8ToWideString(" ảnh");
			SetWindowText(hImageName, msg.c_str());
		} else {
			// Single image
			wstring wPath = utf8ToWideString(currentImagePath);
			// Extract filename only
			size_t lastSlash = wPath.find_last_of(L"\\/");
			if (lastSlash != wstring::npos) {
				wPath = wPath.substr(lastSlash + 1);
			}
			SetWindowText(hImageName, wPath.c_str());
		}
	}
}

string CreateMacroDialog::getMacroName() const {
	return savedName;
}

string CreateMacroDialog::getMacroContent() const {
	return savedContent;
}

string CreateMacroDialog::getImagePath() const {
	return currentImagePath;
}

// ========================== MacroDialog ==========================

MacroDialog::MacroDialog(const HINSTANCE & hInstance, const int & resourceId)
	: BaseDialog(hInstance, resourceId) {
	createMacroDialog = nullptr;
}

MacroDialog::~MacroDialog() {
	if (createMacroDialog) {
		delete createMacroDialog;
	}
}

INT_PTR MacroDialog::eventProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG:
		this->hDlg = hDlg;
		initDialog();
		return TRUE;
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		switch (wmId) {
		case IDOK:
		case IDBUTTON_OK:
			AppDelegate::getInstance()->closeDialog(this);
			break;
		case IDC_BUTTON_ADD:
			onCreateMacroButton();
			break;
		case IDC_BUTTON_DELETE:
			onDeleteMacroButton();
			break;
		case IDC_BUTTON_IMPORT_MACRO:
			onImportMacroButton();
			break;
		case IDC_BUTTON_EXPORT_MACRO:
			onExportMacrobutton();
			break;
		default:
			if (HIWORD(wParam) == BN_CLICKED) {
				this->onCheckboxClicked((HWND)lParam);
			}
			break;
		}
		break;
	}
	case WM_NOTIFY: {
		switch (((LPNMHDR)lParam)->code) {
		case LVN_ITEMCHANGED: {
			NMLISTVIEW* pData = (NMLISTVIEW*)lParam;
			if (pData->uNewState != 0)
				onSelectItem(pData->iItem);
		}
			break;
		case NM_RCLICK: {
			LPNMITEMACTIVATE pnmitem = (LPNMITEMACTIVATE)lParam;
			onRightClickList(pnmitem);
		}
			break;
		}
		break;
	}
	}
	return FALSE;
}

void MacroDialog::initDialog() {
	SET_DIALOG_ICON(IDI_APP_ICON);

	listMacro = GetDlgItem(hDlg, IDC_LIST_MACRO_DATA);
	hCreateButton = GetDlgItem(hDlg, IDC_BUTTON_ADD);
	hDeleteButton = GetDlgItem(hDlg, IDC_BUTTON_DELETE);
	hAutoCaps = GetDlgItem(hDlg, IDC_CHECK_AUTO_CAPS);
	hEmptyListLabel = GetDlgItem(hDlg, IDC_STATIC_EMPTY_LIST);

	LVCOLUMN LvCol;
	memset(&LvCol, 0, sizeof(LvCol));
	LvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	
	wstring col1 = utf8ToWideString("Phím tắt");
	LvCol.pszText = (LPWSTR)col1.c_str();
	LvCol.cx = 150;
	SendMessage(listMacro, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	SendMessage(listMacro, LVM_INSERTCOLUMN, 0, (LPARAM)&LvCol);

	wstring col2 = utf8ToWideString("Nội dung tin nhắn");
	LvCol.pszText = (LPWSTR)col2.c_str();
	LvCol.cx = 250;
	SendMessage(listMacro, LVM_INSERTCOLUMN, 1, (LPARAM)&LvCol);

	// Image Column (Text-based status)
	wstring col3 = utf8ToWideString("Ảnh");
	LvCol.pszText = (LPWSTR)col3.c_str();
	LvCol.cx = 80; 
	SendMessage(listMacro, LVM_INSERTCOLUMN, 2, (LPARAM)&LvCol);

	fillData();
}

void MacroDialog::fillData() {
	SendMessage(hAutoCaps, BM_SETCHECK, vAutoCapsMacro ? 1 : 0, 0);
	SendMessage(listMacro, LVM_DELETEALLITEMS, 0, 0);

	getAllMacro(keys, macroText, macroContent);
	
	macroImagePath.clear();
	macroType.clear();
	
	for (size_t i = 0; i < macroText.size(); i++) {
		string imagePath;
		int type = 0;
		getMacroData(macroText[i], imagePath, type);
		macroImagePath.push_back(imagePath);
		macroType.push_back(type);
	}
	
	for (size_t i = 0; i < macroText.size(); i++) {
		string imgPath = macroImagePath[i]; 
		string imgInfo = "";

		if (!imgPath.empty()) {
			int count = 1;
			for(char c : imgPath) if(c=='|') count++;
			imgInfo = "[" + to_string(count) + " ảnh]";
		}

		// Insert Item (Column 0: Shortcut)
		LV_ITEM data;
		memset(&data, 0, sizeof(data));
		data.mask = LVIF_TEXT;
		data.cchTextMax = 256;
		data.iItem = (int)i; 
		data.iSubItem = 0;
		
		wstring wName = utf8ToWideString(macroText[i]);
		data.pszText = (LPWSTR)wName.c_str();

		// Insert item
		int newIndex = (int)SendMessage(listMacro, LVM_INSERTITEM, 0, (LPARAM)&data); 

		// Set SubItem 1 (Content)
		if (newIndex != -1) {
			data.iItem = newIndex;
			data.iSubItem = 1;
			wstring wContent = utf8ToWideString(macroContent[i]);
			data.pszText = (LPWSTR)wContent.c_str();
			SendMessage(listMacro, LVM_SETITEM, 0, (LPARAM)&data);

			// Set SubItem 2 (Image Info)
			if (!imgInfo.empty()) {
				data.iSubItem = 2;
				wstring wInfo = utf8ToWideString(imgInfo);
				data.pszText = (LPWSTR)wInfo.c_str();
				SendMessage(listMacro, LVM_SETITEM, 0, (LPARAM)&data);
			}
		}
	}
	
	updateEmptyListLabel();
}

void MacroDialog::insertItem(const int& index, LPTSTR macroName, LPTSTR macroContent) {
	// Not used anymore as fillData handles insertion
}

void MacroDialog::updateEmptyListLabel() {
	int itemCount = SendMessage(listMacro, LVM_GETITEMCOUNT, 0, 0);
	if (itemCount == 0) {
		ShowWindow(hEmptyListLabel, SW_SHOW);
		wstring emptyMsg = utf8ToWideString("Chưa có tin nhắn nhanh nào.\nNhấn \"+ Tạo mới\" để thêm.");
		SetWindowText(hEmptyListLabel, emptyMsg.c_str());
	} else {
		ShowWindow(hEmptyListLabel, SW_HIDE);
	}
}

void MacroDialog::saveAndReload() {
	vector<Byte> macroData;
	getMacroSaveData(macroData);
	OpenKeyHelper::setRegBinary(_T("macroData"), macroData.data(), (int)macroData.size());
	fillData();
}

void MacroDialog::onSelectItem(const int & index) {
	// Can be used later for editing macro
}

void MacroDialog::onCreateMacroButton() {
	if (!createMacroDialog) {
		createMacroDialog = new CreateMacroDialog(hInstance, IDD_DIALOG_CREATE_MACRO);
	}
	
	// Reset dialog state before showing
	createMacroDialog->setMacroData("", "", "");
	
	INT_PTR result = DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG_CREATE_MACRO), hDlg, 
		CreateMacroDialogProc, (LPARAM)createMacroDialog);
	
	if (result == IDOK) {
		string macroName = createMacroDialog->getMacroName();
		string macroContent = createMacroDialog->getMacroContent();
		string imagePath = createMacroDialog->getImagePath();

		if (macroName.empty() || macroContent.empty()) {
			MessageBox(hDlg, utf8ToWideString("Bạn hãy nhập phím tắt và nội dung tin nhắn!").c_str(), _T("OpenKey"), MB_OK);
			return;
		}

		int macroTypeToSave = imagePath.empty() ? 0 : 1;
		addMacro(macroName, macroContent, macroTypeToSave, imagePath);
		saveAndReload();
	}
}

void MacroDialog::onDeleteMacroButton() {
	int selectedIndex = SendMessage(listMacro, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
	if (selectedIndex < 0) {
		MessageBox(hDlg, utf8ToWideString("Bạn hãy chọn tin nhắn cần xóa!").c_str(), _T("OpenKey"), MB_OK);
		return;
	}

	if (selectedIndex < macroText.size()) {
		// Because list is inverted (see insertItem)
		string macroNameToDelete = macroText[macroText.size() - 1 - selectedIndex];
		if (deleteMacro(macroNameToDelete)) {
			saveAndReload();
		}
	}
}

void MacroDialog::onEditMacroButton() {
	int selectedIndex = SendMessage(listMacro, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
	if (selectedIndex < 0) return;

	int realIndex = (int)macroText.size() - 1 - selectedIndex;
	if (realIndex < 0 || realIndex >= macroText.size()) return;

	string oldKey = macroText[realIndex];
	string oldContent = macroContent[realIndex];
	string oldImage = macroImagePath[realIndex];

	if (!createMacroDialog) {
		createMacroDialog = new CreateMacroDialog(hInstance, IDD_DIALOG_CREATE_MACRO);
	}

	createMacroDialog->setMacroData(oldKey, oldContent, oldImage);

	INT_PTR result = DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG_CREATE_MACRO), hDlg,
		CreateMacroDialogProc, (LPARAM)createMacroDialog);

	if (result == IDOK) {
		string macroName = createMacroDialog->getMacroName();
		string macroContent = createMacroDialog->getMacroContent();
		string imagePath = createMacroDialog->getImagePath();

		if (macroName.empty() || macroContent.empty()) {
			MessageBox(hDlg, utf8ToWideString("Bạn hãy nhập phím tắt và nội dung tin nhắn!").c_str(), _T("OpenKey"), MB_OK);
			return;
		}

		if (macroName != oldKey) {
			deleteMacro(oldKey);
		}

		int macroTypeToSave = imagePath.empty() ? 0 : 1;
		addMacro(macroName, macroContent, macroTypeToSave, imagePath);
		saveAndReload();
	}
}

void MacroDialog::onRightClickList(LPNMITEMACTIVATE pnmitem) {
	if (pnmitem->iItem == -1) return;

	HMENU hPopupMenu = CreatePopupMenu();
	InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, 1001, utf8ToWideString("Sửa").c_str());
	InsertMenu(hPopupMenu, 1, MF_BYPOSITION | MF_STRING, 1002, utf8ToWideString("Xóa").c_str());

	POINT pt;
	GetCursorPos(&pt);

	// TrackPopupMenu blocks, returning the selected command ID because of TPM_RETURNCMD
	int selected = TrackPopupMenu(hPopupMenu, TPM_RETURNCMD | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hDlg, NULL);

	if (selected == 1001) { // Edit
		onEditMacroButton();
	} else if (selected == 1002) { // Delete
		onDeleteMacroButton();
	}

	DestroyMenu(hPopupMenu);
}

void MacroDialog::onImportMacroButton() {
	OPENFILENAME ofn;
	TCHAR szFile[MAX_PATH] = { 0 };
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hDlg;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = _T("Text file (*.txt)\0*.txt\0All (*.*)\0*.*\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE) {
		int msgboxID = MessageBox(
			hDlg,
			utf8ToWideString("Bạn có muốn giữ lại dữ liệu hiện tại không?").c_str(),
			utf8ToWideString("Dữ liệu gõ tắt").c_str(),
			MB_ICONEXCLAMATION | MB_YESNO
		);
		wstring path = ofn.lpstrFile;
		readFromFile(wideStringToUtf8(path), msgboxID == IDYES);
		saveAndReload();
	}
}

void MacroDialog::onExportMacrobutton() {
	OPENFILENAME ofn;
	TCHAR szFile[MAX_PATH] = { 'O', 'p', 'e', 'n', 'K', 'e', 'y', 'M', 'a', 'c', 'r', 'o' };
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hDlg;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = _T("Text file (*.txt)\0*.txt\0");
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrDefExt = (LPCWSTR)L"txt";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetSaveFileName(&ofn) == TRUE) {
		wstring path = ofn.lpstrFile;
		saveToFile(wideStringToUtf8(path));
	}
}

void MacroDialog::onCheckboxClicked(const HWND& hWnd) {
	if (hWnd == hAutoCaps) {
		int val = (int)SendMessage(hWnd, BM_GETCHECK, 0, 0);
		APP_SET_DATA(vAutoCapsMacro, val ? 1 : 0);
	}
}
