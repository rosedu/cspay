/*
 * Cojocar Lucian <cojocar .at. gmail .dot. com>
 * Start: Wed May 16 18:43:21 EEST 2007
 *
 * Fiserul principal al interfetei.
 *
 */

#include <wx/wx.h>
#include <wx/statline.h>

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
	std :: cout << "Aici-1";
	//astea de mai jos vin din alta parte
	arr_str_fac_names.Add(_T("Automatica si Calculatoare"));
	arr_str_fac_names.Add(_T("Inginerie in limbi straine"));
	arr_str_fac_names.Add(_T("I me se te"));
	
	//catedrele
	arr_str_dep_names[0].Add(_T("Calculatoare"));
	arr_str_dep_names[0].Add(_T("TI"));


	arr_str_dep_names[1].Add(_T("Catedra din fac 2"));
	arr_str_dep_names[1].Add(_T("O alta catedra din fac 2"));
			
	arr_str_dep_names[2].Add(_T("Cat din fac 3"));
	arr_str_dep_names[2].Add(_T("O alta cat din fa 3"));
	
	//Initializam zilele
	
	arr_str_days.Add(_T("Luni"));
	arr_str_days.Add(_T("Marti"));
	arr_str_days.Add(_T("Miercuri"));
	arr_str_days.Add(_T("Joi"));
	arr_str_days.Add(_T("Vineri"));
	arr_str_days.Add(_T("Sambata"));
	arr_str_days.Add(_T("Duminica"));
	
	//Initializam titulaturilile
	arr_str_titles.Add(_T("asistent"));
	arr_str_short_titles.Add(_T("a"));
	
	arr_str_titles.Add(_T("profesor"));
	arr_str_short_titles.Add(_T("p"));
	
	arr_str_titles.Add(_T("sef de lucrari"));
	arr_str_short_titles.Add(_T("s"));
	
	arr_str_titles.Add(_T("conferentiar"));
	arr_str_short_titles.Add(_T("c"));
	
	
	std :: cout << "Aici0";
	frame = new MainFrame(arr_str_days, arr_str_fac_names,
							arr_str_titles, arr_str_short_titles,
							arr_str_dep_names);
	frame->Show(TRUE);
	SetTopWindow(frame);
	return TRUE;
}

MainFrame :: MainFrame(
					wxArrayString arr_str_days,
					wxArrayString arr_str_fac_names,
					wxArrayString arr_str_titles,
					wxArrayString arr_str_short_titles,
					wxArrayString arr_str_dep_names[])
: wxFrame((wxFrame *)NULL, -1, _T("CSPayGUI"), wxDefaultPosition, wxDefaultSize)
{
	/*
	 * functia constructor a frame-ului principal
	 * aici voi scrie toata interfata initiala
	 */
	int i;
	
	i = 0;
	while (arr_str_dep_names[i].IsEmpty() == FALSE) {
		p_arr_str_dep_names[i] = (wxArrayString) arr_str_dep_names[i];
		++ i;
	}
	std :: cout << "Aici1";
	p_arr_str_days = arr_str_days;
	p_arr_str_fac_names = arr_str_fac_names;
	p_arr_str_titles = arr_str_titles;
	
	
	//Matricea in care bagam tot
	main_sizer = new wxFlexGridSizer(2, 0, 0);
	
	
	//Numele si prenumele
	main_sizer->Add(new wxStaticText(this, -1, _T("Nume si prenume")),
		0, wxALL | wxALIGN_CENTRE_VERTICAL, 4);
	
	edit_name = new wxTextCtrl(this, -1, _T(""), wxDefaultPosition,
				wxSize(330, 30));
	edit_name->SetMaxLength(40);
	
	main_sizer->Add(edit_name, 0, wxALIGN_LEFT, 4);
	
	
	//Alegerea Facultatii
	main_sizer->Add(new wxStaticText(this, -1, _T("Facultatea")),
		0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTRE_VERTICAL, 4);
		


	choice_fac = new wxChoice(this, ID_on_select_fac, wxDefaultPosition, wxDefaultSize,
			arr_str_fac_names);
	choice_fac->SetSelection(0);
	
	main_sizer->Add(choice_fac);

	
	//Alegerea Catedrei
	main_sizer->Add(new wxStaticText(this, -1, _T("Catedra")),
		0, wxALL | wxALIGN_RIGHT | wxALIGN_CENTRE_VERTICAL, 4);

	choice_dep = new wxChoice(this, -1, wxDefaultPosition, 
					wxDefaultSize, p_arr_str_dep_names[0]);
	choice_dep->SetSelection(0);
	
	main_sizer->Add(choice_dep, 0);



	
	//Prima regula
	this->n = 0;
	labels[this->n] = new wxStaticText(this, -1, _T("1"));
	
	main_sizer->Add(labels[this->n], 0,
		wxALIGN_CENTRE_VERTICAL | wxALIGN_CENTRE_HORIZONTAL, 0);
	
	rules[this->n] = new OptionSizer(this, 
										arr_str_days,
										arr_str_fac_names,
										arr_str_titles);
	
	
	main_sizer->Add(rules[this->n], 0, wxALIGN_LEFT, 0);
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
	int sel = choice_fac->GetCurrentSelection(); 
	
	choice_dep->Clear();
	choice_dep->Append(p_arr_str_dep_names[sel]);
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
	
	main_sizer->Add(
		sl1[this->n] = new wxStaticLine(this,
				-1, wxDefaultPosition, wxDefaultSize,
				wxLI_HORIZONTAL),
		0,
		wxALL | wxEXPAND,
		0);
	
	main_sizer->Add(
		sl2[this->n] = new wxStaticLine(this,
				-1, wxDefaultPosition, wxDefaultSize,
				wxLI_HORIZONTAL),
		0,
		wxALL | wxEXPAND,
		0);
	//creez eticheta din dreapta (aia cu numarul)
	wxString t;
	t.Printf(_("%d "), n + 1);
	labels[this->n] = new wxStaticText(this, -1, t);
	
	main_sizer->Add(labels[this->n], 0,
			wxALIGN_CENTRE_VERTICAL | wxALIGN_CENTRE_HORIZONTAL, 0);

	//creez si noul set de butoane
	rules[this->n] = new OptionSizer(this, 
							p_arr_str_days,
							p_arr_str_fac_names,
							p_arr_str_titles);
	
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
	
	
	main_sizer->Detach(sl1[this->n]);

	main_sizer->Detach(sl2[this->n]);
	sl1[this->n]->Destroy();
	sl2[this->n]->Destroy();

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
	/*
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
		cout << "Ziua \t" << rules[i]->GetIntDay() << endl;
	}
	cout << "========================================" << endl;
	*/

}
void MainFrame :: OnPressQuit(wxCommandEvent& WXUNUSED(event))
{
	std :: cout << "PA" << std :: endl;
	Close(TRUE);
}
