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
#pragma once
#include "BaseDialog.h"

// Modal dialog for creating/editing macros
class CreateMacroDialog {
public:
	HWND hMacroName, hMacroContent;
	HWND hSelectImageButton, hRemoveImageButton;
	HWND hImagePreview, hImageName;
	string currentImagePath;
	string initName;    // New variable
	string initContent; // New variable
	string savedName;    // New variable to save result
	string savedContent; // New variable to save result
	HINSTANCE hInstance;
	int resId;

	CreateMacroDialog(const HINSTANCE & hInstance, const int & resourceId);
	~CreateMacroDialog();
	string getMacroName() const;
	string getMacroContent() const;
	string getImagePath() const;
	void initDialog(HWND hDlg);
	void onSelectImageButton(HWND hDlg);
	void onRemoveImageButton();
	void updateImagePreview();
	void setMacroData(string name, string content, string imagePath); // New method
	INT_PTR eventProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

	friend INT_PTR CALLBACK CreateMacroDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

class MacroDialog : public BaseDialog {
private:
	HWND listMacro;
	HWND hAutoCaps;
	HWND hCreateButton, hDeleteButton;
	HWND hEmptyListLabel;
	vector<vector<Uint32>> keys;
	vector<string> macroText;
	vector<string> macroContent;
	vector<string> macroImagePath;
	vector<int> macroType;
	CreateMacroDialog* createMacroDialog;
	
public:
	MacroDialog(const HINSTANCE & hInstance, const int & resourceId);
	~MacroDialog();
	virtual void fillData() override;
protected:
	INT_PTR eventProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void initDialog();
private:
	void saveAndReload();
	void insertItem(const int& index, LPTSTR macroName, LPTSTR macroContent);
	void updateEmptyListLabel();
	void onSelectItem(const int& index);
	void onCreateMacroButton();
	void onDeleteMacroButton();
	void onEditMacroButton(); // New method
	void onRightClickList(LPNMITEMACTIVATE pnmitem); // New method
	void onImportMacroButton();
	void onExportMacrobutton();
	void onCheckboxClicked(const HWND& hWnd);
};

