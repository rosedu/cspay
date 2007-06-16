/*
 * Cojocar Lucian <cojocar .at. gmail .dot. com>
 * Start: Wed May 16 18:43:21 EEST 2007
 *
 * Fiserul principal al interfetei.
 *
 */

#include <wx/wx.h>
#include <wx/statline.h>
#include <wx/file.h>

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

	p_arr_str_days = arr_str_days;
	p_arr_str_fac_names = arr_str_fac_names;
	p_arr_str_titles = arr_str_titles;
	p_arr_str_short_titles = arr_str_short_titles;
	
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

	wxFile *fout;
	fout = new wxFile(_T("personal.ini"), wxFile::write);
	if (!fout) {
		return;
	}
	wxString str_edit_name = edit_name->GetValue();
	str_edit_name.Trim(TRUE);
	str_edit_name.Trim(FALSE);
	if (str_edit_name.IsEmpty()) {
		ShowErr(_T("nume"));
		return;
	}
	fout->Write(_T("[antet]\n"));
	
	fout->Write(_T("nume="));
	fout->Write(str_edit_name);
	fout->Write(_T("\n"));
	
	fout->Write(_T("universitate=Universitatea Politehnica"));
	fout->Write(_T("\n"));
	
	fout->Write(_T("facultate="));
	fout->Write(choice_fac->GetStringSelection());
	fout->Write(_T("\n"));
	
	fout->Write(_T("catedra="));
	fout->Write(choice_dep->GetStringSelection());
	fout->Write(_T("\n"));
	
	fout->Write(_T("tip_fisier=odt"));
	fout->Write(_T("\n"));
	
	fout->Write(_T("\n"));
	
	fout->Write(_T("[ore]\n"));
	
	
	wxString *str_tmp;
	wxString str_edit_mat;
	wxString str_edit_course_no;
	wxString str_sr_grp;
	
	int sel_start, sel_stop;
	int sel_par;
	
	int i;
	for (i = 0; i < this->n; ++ i) {
		str_tmp = new wxString;
		str_tmp->Printf(_T("%d"), i + 1);
		
		str_tmp->Append(_T("="));
		str_tmp->Append(rules[i]->GetSelDay());
		str_tmp->Append(_T(":"));
		
		str_tmp->Append(rules[i]->GetSelFac());
		str_tmp->Append(_T(":"));
		
		str_edit_mat = rules[i]->GetEditMat();
		str_edit_mat.Trim(TRUE);
		str_edit_mat.Trim(FALSE);
		if (str_edit_mat.IsEmpty()) {
			ShowErr(_T("materia"));
			return;
		}
		str_tmp->Append(str_edit_mat);
		str_tmp->Append(_T(":"));
		
		str_tmp->Append(p_arr_str_short_titles[rules[i]->GetTitle()]);
		str_tmp->Append(_T(":"));
		
		str_edit_course_no = rules[i]->GetCourseNo();
		str_edit_course_no.Trim(TRUE);
		str_edit_course_no.Trim(FALSE);
		if (!str_edit_course_no.IsNumber()){
			ShowErr(_T("numar curs, corect"));
			return;
		}
		str_tmp->Append(str_edit_course_no);
		str_tmp->Append(_T(":"));
		
		if (rules[i]->GetType() == 0) {
			str_tmp->Append(_T("a"));
		} else
			str_tmp->Append(_T("c"));
		str_tmp->Append(_T(":"));
		
		str_sr_grp = rules[i]->GetSrGrp();
		str_sr_grp.Trim(TRUE);
		str_sr_grp.Trim(FALSE);
		if (str_sr_grp.IsEmpty()) {
			ShowErr(_T("seria, grupa"));
			return;
		}
		str_tmp->Append(str_sr_grp);
		str_tmp->Append(_T(":"));
		
		sel_start = rules[i]->GetStartH();
		str_tmp->Append(wxString :: Format(_T("%d"), sel_start));
		str_tmp->Append(_T("-"));
		sel_stop = rules[i]->GetStopH();
		if (sel_stop <= sel_start) {
			ShowErr(_T("Calatoria in timp nu este posibila !"));
			return;
		}
		str_tmp->Append(wxString :: Format(_T("%d"), sel_stop));
		str_tmp->Append(_T(":"));		
		
		sel_par = rules[i]->GetPar();
		str_tmp->Append(wxString :: Format(_T("%d"), sel_par));
		str_tmp->Append(_T(":"));
		
		sel_par = rules[i]->GetStartW();
		str_tmp->Append(wxString :: Format(_T("%d"), sel_par));
		str_tmp->Append(_T("\n"));
		
		fout->Write(*str_tmp);
		delete str_tmp;
	}
	
	fout->Close();
}
void MainFrame :: OnPressQuit(wxCommandEvent& WXUNUSED(event))
{
	std :: cout << "PA" << std :: endl;
	Close(TRUE);
}
void MainFrame :: ShowErr(wxString field)
{
	wxString msg = _T("Va rugam completati campul\n");
	msg.Append(field);
	(new wxMessageDialog(this, 
		msg, 
		_T("Camp lipsa"),
		wxICON_ERROR | wxOK))
	->ShowModal();
}
