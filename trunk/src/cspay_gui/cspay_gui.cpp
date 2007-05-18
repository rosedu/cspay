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
	 * functia constructor a framului principal
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
	catedre_names.Add(_T("Calculatoare"));
	catedre_names.Add(_T("TI"));
	choice_cat = new wxChoice(this, -1, wxDefaultPosition, wxDefaultSize, catedre_names);
	choice_cat->SetSelection(0);
	main_sizer->Insert(5, choice_cat, 0);


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

	main_sizer->SetSizeHints(this);

	SetSizer(main_sizer);
}
void MainFrame :: OnSelectFac(wxCommandEvent& WXUNUSED(event))
{
	//std::cout << choice_fac->GetCurrentSelection() << std::endl;
	switch (choice_fac->GetCurrentSelection()) {
		case 0 : 
			//catedrele din prima facultate
			//asta nu prea au valoare acum
			catedre_names.Empty();
			catedre_names.Add(_T("Calculatoare"));
			catedre_names.Add(_T("TI"));
			break;
		case 1 :
			//o alta faculate
			catedre_names.Empty();
			catedre_names.Add(_T("Catedra din fac 2"));
			catedre_names.Add(_T("O alta catedra din fac 2"));
			break;
		case 2 :
			//alta facultate
			catedre_names.Empty();
			catedre_names.Add(_T("Cat din fac 3"));
			catedre_names.Add(_T("O alta cat din fa 3"));
			break;
	}

	choice_cat->Clear();
	choice_cat->Append(catedre_names);
	choice_cat->SetSelection(0);

}
void MainFrame :: OnPressMore(wxCommandEvent& WXUNUSED(event))
{
	//testez daca se mai pot insera reguli
	if (this->n + 1 >= OMAX) {
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
	
	//FIXME SetSizeHints, trebuie apelat o singur data
	//cred.
	main_sizer->SetSizeHints(this);
	main_sizer->Layout();
	main_sizer->SetSizeHints(this);
	//testez daca nu cumva, am inserat ultima regula
	if ((this->n - 1) == OMAX) {
		//nu se mai pot insera
		rules[this->n]->HideMore();
	}
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
