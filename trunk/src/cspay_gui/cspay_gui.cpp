/*
 * Cojocar Lucian <cojocar .at. gmail .dot. com>
 * Start: Wed May 16 18:43:21 EEST 2007
 *
 * Fiserul principal al interfetei.
 *
 */

#include <wx/wx.h>

#include "cspay_gui.h"
#include "opt_gui.h"

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_CHOICE(ID_on_select_fac, MainFrame :: OnSelectFac)
	EVT_BUTTON(ID_on_more, MainFrame :: OnPressMore)
	EVT_BUTTON(ID_on_less, MainFrame :: OnPressLess)
	EVT_TOOL(ID_on_save, MainFrame :: OnPressSave)
	EVT_TOOL(ID_on_quit, MainFrame :: OnPressQuit)
END_EVENT_TABLE()

IMPLEMENT_APP(CSpayGUI)

bool
CSpayGUI :: OnInit()
{
	frame = new MainFrame(_T("CSpayGUI"), wxPoint(50, 50), wxSize(500, 500));
	frame->Show(TRUE);
	SetTopWindow(frame);
	return TRUE;
}

MainFrame :: MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
	/*
	 * functia constructor a frame-ului principal
	 * aici voi scrie toata interfata initiala
	 */
	//Matricea in care bagam tot
	main_sizer = new wxFlexGridSizer(OMAX + 10, 2, 0, 0);

	
	//Numele si prenumele
	main_sizer->Insert(0, new wxStaticText(this, -1, _T("Nume si prenume")),
		0, wxALIGN_RIGHT, 0);
	edit_name = new wxTextCtrl(this, -1);
	main_sizer->Insert(1, edit_name, 0, wxALIGN_LEFT, 0);
	//primul numar din ->Insert este pozitia in care inseram,
	//astfel, 1 corespunde primei linii si coloanei doi.
	
	
	//Alegerea Facultatii
	main_sizer->Insert(2, new wxStaticText(this, -1, _T("Facultatea")),
		0, wxALIGN_RIGHT, 0);
	//astea de mai jos vin din alta parte
	faculty_names.Add(_T("Automatica si Calculatoare"));
	faculty_names.Add(_T("Inginerie in limbi straine"));
	faculty_names.Add(_T("I me se te"));
	choice_fac = new wxChoice(this, ID_on_select_fac, wxDefaultPosition, wxDefaultSize,
			faculty_names);
	choice_fac->SetSelection(0);
	main_sizer->Insert(3, choice_fac);

	
	//Alegerea Catedrei
	main_sizer->Insert(4, new wxStaticText(this, -1, _T("Catedra")),
		0, wxALIGN_RIGHT, 0);
	//Si asta vin din alta parte, ele vor fi pentru prima selctie doar
	dep_names.Add(_T("Calculatoare"));
	dep_names.Add(_T("TI"));
	choice_dep = new wxChoice(this, -1, wxDefaultPosition, wxDefaultSize, dep_names);
	choice_dep->SetSelection(0);
	main_sizer->Insert(5, choice_dep, 0);


	//Nume curs
	main_sizer->Insert(6, new wxStaticText(this, -1, _T("Nume curs")),
		0, wxALIGN_RIGHT, 0);
	edit_course = new wxTextCtrl(this, -1);
	main_sizer->Insert(7, edit_course);


	//Prima regula
	this->n = 0;		//avem o regula inserta
	labels[this->n] = new wxStaticText(this, -1, _T("0 "));
	main_sizer->Insert(8, labels[this->n],	0, wxALIGN_RIGHT, 0);
	rules[this->n] = new OptionSizer(this);
	main_sizer->Insert(9, rules[this->n], 0, wxALIGN_LEFT, 0);
	rules[this->n]->HideLess();
	++ this->n;


	/*
	 * toolbarul
	 */

	this->CreateToolBar(wxNO_BORDER|wxHORIZONTAL|wxTB_FLAT, 0);
	wxToolBar *tb = this->GetToolBar();

	#include "document-save-48.xpm"
	#include "process-stop-48.xpm"

	wxBitmap *wb = new wxBitmap(document_save_48_xpm);
	wb->SetWidth(2);
	tb->AddTool(ID_on_save, _T("Save"), *wb, _T("Save"));
	delete wb;
	wb = new wxBitmap(process_stop_48_xpm);
	tb->AddTool(ID_on_quit, _T("Quit"), *wb, _T("Quit"));
	delete wb;
	tb->Realize();

	main_sizer->Layout();
	main_sizer->SetSizeHints(this);

	SetSizer(main_sizer);
	main_sizer->Layout();
}
void MainFrame :: OnSelectFac(wxCommandEvent& WXUNUSED(event))
{
	//std::cout << choice_fac->GetCurrentSelection() << std::endl;
	switch (choice_fac->GetCurrentSelection()) {
		case 0 : 
			//catedrele din prima facultate
			//asta nu prea au valoare acum
			dep_names.Empty();
			dep_names.Add(_T("Calculatoare"));
			dep_names.Add(_T("TI"));
			break;
		case 1 :
			//o alta faculate
			dep_names.Empty();
			dep_names.Add(_T("Catedra din fac 2"));
			dep_names.Add(_T("O alta catedra din fac 2"));
			break;
		case 2 :
			//alta facultate
			dep_names.Empty();
			dep_names.Add(_T("Cat din fac 3"));
			dep_names.Add(_T("O alta cat din fa 3"));
			break;
	}

	choice_dep->Clear();
	choice_dep->Append(dep_names);
	choice_dep->SetSelection(0);
	main_sizer->Layout();

}
void MainFrame :: OnPressMore(wxCommandEvent& WXUNUSED(event))
{
	//testez daca se mai pot insera reguli
	if (this->n  >= OMAX) {
		//std::cout << "Prea multe" << std :: endl;
		return ;
	}

	//creez eticheta din dreapta (aia cu numarul)
	wxString t;
	t.Printf(_("%d "), n);
	labels[this->n] = new wxStaticText(this, -1, t);
	main_sizer->Add(labels[this->n], 0, wxALIGN_RIGHT, 0);

	//creez si noul set de butoane
	rules[this->n] = new OptionSizer(this);
	main_sizer->Add(rules[this->n]);
	rules[this->n - 1]->HideMore();
	rules[this->n - 1]->HideLess();


	//crestem numarul de butoane de reguli, inserate
	++ this->n;
	
	//testez daca nu cumva, am inserat ultima regula
	if ((this->n) == OMAX) {
		//nu se mai pot insera
		rules[this->n - 1]->HideMore();
	}
	//FIXME SetSizeHints, trebuie apelat o singur data
	//cred.
	//main_sizer->SetSizeHints(this);
	main_sizer->Layout();
	main_sizer->SetSizeHints(this);
}
void MainFrame :: OnPressLess(wxCommandEvent& WXUNUSED(event))
{
	if (this->n <= 0) {
		//std::cout << "Prea putine" << std :: endl;
		return;
	}
	-- this->n;

	//detasam eticheta si sizerul care trebuie
	main_sizer->Detach(rules[this->n]);
	main_sizer->Detach(labels[this->n]);

	//le "stergem"
	rules[this->n]->Clear(true);
	labels[this->n]->Destroy();

	//afisam buton more
	rules[this->n - 1]->ShowMore();
	if (this->n - 1)	//afisam buton less
		rules[this->n - 1]->ShowLess();
	
	//redesanam tot
	//FIXME: SetSizeHints, trebuie apelat o singura data,
	//cred.
	main_sizer->SetSizeHints(this);
	main_sizer->Layout();
	main_sizer->SetSizeHints(this);
}
void MainFrame :: OnPressSave(wxCommandEvent& WXUNUSED(event))
{
	using namespace std;
	cout << "========================================" << endl;
	cout << "Avem " << this->n  << " optiuni completate" << endl;
	wxString edn = edit_name->GetValue();
	cout << "Nume\t" << edn.ToAscii() << endl;
	cout << "Facu\t" << faculty_names[choice_fac->GetSelection()].ToAscii() << endl;
	cout << "Cate\t" << dep_names[choice_dep->GetSelection()].ToAscii() << endl;
	cout << "Curs\t" << edit_course->GetValue().ToAscii() << endl;

	int i;
	for (i = 0; i < this->n; ++ i) {
		cout << "OP   \t" << i << endl;
		cout << "Tipul\t" << rules[i]->GetType().ToAscii() << endl;
		cout << "Grupa\t" << rules[i]->GetGroup().ToAscii() << endl;
		cout << "Ziua \t" << rules[i]->GetStrDay().ToAscii() << endl;
	}
	cout << "========================================" << endl;

}
void MainFrame :: OnPressQuit(wxCommandEvent& WXUNUSED(event))
{
	std :: cout << "PA" << std :: endl;
	Close(TRUE);
}
