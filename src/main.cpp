#define wxUSE_WARNING_FOR_COMMON_CONTROLS 0
#include <wx/wx.h>

class MyFrame : public wxFrame {
public:
    MyFrame();

private:
    void OnButtonClick(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

enum {
    ID_BUTTON = 1000,
    ID_EXIT
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_BUTTON(ID_BUTTON, MyFrame::OnButtonClick)
EVT_MENU(ID_EXIT, MyFrame::OnExit)
wxEND_EVENT_TABLE()

bool MyApp::OnInit() {
    MyFrame* frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(nullptr, wxID_ANY, "Моё первое wxWidgets-приложение", wxDefaultPosition, wxSize(400, 200))
{
    // Панель (обязательна в wxWidgets для размещения контролов)
    wxPanel* panel = new wxPanel(this);

    // Кнопка
    wxButton* button = new wxButton(panel, ID_BUTTON, "Нажми !", wxPoint(140, 50));

    // Меню
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_EXIT, "Выход\tAlt+F4");

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&Файл");
    SetMenuBar(menuBar);

    // Статусная строка
    CreateStatusBar();
    SetStatusText("Готово");
}

void MyFrame::OnButtonClick(wxCommandEvent& WXUNUSED(event)) {
    wxMessageBox("Привет Сделал !", "Сообщение", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

wxIMPLEMENT_APP(MyApp);