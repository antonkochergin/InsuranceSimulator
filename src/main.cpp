// Include the main wxWidgets header file
// It contains everything needed for GUI: windows, buttons, events, etc.
#include <wx/wx.h>

// ---------------------------------------------------------------
// 1. Declare the main window class (inherited from wxFrame)
// ---------------------------------------------------------------
class MyFrame : public wxFrame
{
public:
    // Constructor: creates the window and all its elements
    MyFrame();

private:
    // Event handlers — functions called when the user performs actions
    void OnButtonClick(wxCommandEvent& event);      // when the button is clicked
    void OnAbout(wxCommandEvent& event);            // when "About" is selected
    void OnExit(wxCommandEvent& event);             // when exiting the app

    // Pointers to UI elements (so they can be accessed from event handlers)
    wxTextCtrl* m_textCtrl;   // text input field
    wxButton* m_button;     // button

    // Macro required for event handlers to be recognized
    wxDECLARE_EVENT_TABLE();
};

// ---------------------------------------------------------------
// 2. Declare the application class (entry point)
// ---------------------------------------------------------------
class MyApp : public wxApp
{
public:
    virtual bool OnInit(); // called at application startup
};

// ---------------------------------------------------------------
// 3. Implementation of the main window
// ---------------------------------------------------------------

// Event table: "when an event occurs → call a function"
// Without this, buttons and menu items won't work!
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_BUTTON(1001, MyFrame::OnButtonClick)  // button with ID=1001 → OnButtonClick
EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)    // "About" menu item → OnAbout
EVT_MENU(wxID_EXIT, MyFrame::OnExit)     // "Exit" menu item → OnExit
wxEND_EVENT_TABLE()

// Window constructor
MyFrame::MyFrame()
    : wxFrame(nullptr, wxID_ANY, "My wxWidgets Application", wxDefaultPosition, wxSize(400, 200))
{
    // --- Create the menu bar ---
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT, "Exit\tAlt+F4");  // \t — keyboard shortcut

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT, "About..."); // standard ID

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");     // & underlines the letter (Alt+F)
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar); // attach the menu bar to the window

    // --- Create the status bar ---
    CreateStatusBar();
    SetStatusText("Ready");

    // --- Create a text control ---
    // ID = -1 → auto-generated; we won't use it directly
    m_textCtrl = new wxTextCtrl(this, wxID_ANY, "Enter text...", wxPoint(20, 30), wxSize(200, 30));

    // --- Create a button ---
    m_button = new wxButton(this, 1001, "Show Text", wxPoint(230, 30), wxSize(120, 30));
    // ID = 1001 — used in the event table above
}

// Button click handler
void MyFrame::OnButtonClick(wxCommandEvent& WXUNUSED(event))
{
    // Get text from the text field
    wxString text = m_textCtrl->GetValue();

    // Display the text in the status bar
    SetStatusText("You entered: " + text);

    // (optional) show a message box:
    // wxMessageBox("You entered: " + text, "Info", wxOK | wxICON_INFORMATION);
}

// "About" menu handler
void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(
        "This is a sample wxWidgets application.\n\n"
        "Author: Anton\n"
        "Date: 2025",
        "About",
        wxOK | wxICON_INFORMATION,
        this  // specify parent window for modal dialog
    );
}

// Exit handler
void MyFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
    Close(true); // close the application
}

// ---------------------------------------------------------------
// 4. Application entry point implementation
// ---------------------------------------------------------------

// Called once at startup
bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame();
    frame->Show(true); // show the window
    return true;
}

// Macro that registers the application with wxWidgets
// This is the equivalent of main() for wxWidgets apps
wxIMPLEMENT_APP(MyApp);