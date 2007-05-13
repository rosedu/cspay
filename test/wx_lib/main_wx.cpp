#include "cspay.h"

#include <wx/wx.h>

class cspay : public wxApp
{
	virtual bool OnInit();
};

class my_frame: public wxFrame
{
public:
	my_frame(const wxString &title, const wxPoint &pos, const wxSize &size);
	void OnBtnPress(wxCommandEvent& event);
	DECLARE_EVENT_TABLE()
};

enum 
{
	ID_BTN_OK,
};

BEGIN_EVENT_TABLE(my_frame, wxFrame)
	EVT_BUTTON(ID_BTN_OK, my_frame::OnBtnPress)
END_EVENT_TABLE()

IMPLEMENT_APP(cspay)

bool cspay::OnInit()
{
	my_frame *frame = new my_frame(_T("Salutare"), wxPoint(50, 50), wxSize(450, 450));
	frame->Show(TRUE);
	SetTopWindow(frame);
	return TRUE;
}
my_frame::my_frame(const wxString &title, const wxPoint &pos, const wxSize &size) 
: wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
	//wxBoxSizer *bs = new wxBoxSizer(wxVERTICAL);

	//bs->Add(new wxButton(this, ID_POP_DLG, _T("Buton")), 0, 0, 0);
	//bs->Add(new wxButton(this, ID_POP_DLG, _T("BUTOOOOOOOOOOOON")), 0, 0, 0);
	new wxButton(this, ID_BTN_OK, _T("OK"));
	//SetSizer(bs);
}
void my_frame::OnBtnPress(wxCommandEvent& event)
{
	wxString ws((const char *)mesaj(), wxConvUTF8);
	//std::cout << mesaj() << std::endl;
	
	wxMessageDialog *md = new wxMessageDialog(this, ws);
	md->ShowModal();
}
