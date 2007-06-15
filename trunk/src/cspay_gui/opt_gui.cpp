/*
 * Cojocar Lucian <cojocar .at. gmail .dot. com>
 *
 * Fisierul acesta contine clasele pentru manunchiul
 * de widgeturi afisat cand se da more.
 *
 * Adica widget-urile cu toate butoanele pentru optiuni
 * 
 * Este derivata din wxFlexGridSizer
 *
 */

#include <wx/wx.h>
#include <wx/statline.h>
#include <wx/spinctrl.h>

#include "opt_gui.h"
#include "cspay_gui.h"	//FIXME: asta nu trebuie inclus

OptionSizer :: OptionSizer(wxWindow *pw,
							wxArrayString arr_str_days, 
							wxArrayString arr_str_fac_names,
							wxArrayString arr_str_titles)
: wxFlexGridSizer(12, 0, 0)
{

	int left_flag = (wxALL & ~wxRIGHT) | wxALIGN_CENTRE_VERTICAL;
	int right_flag = (wxALL & ~wxLEFT) | wxALIGN_CENTRE_VERTICAL;
	//Ziua din saptamana
	this->Add(
		new wxStaticText(pw, -1, _T("Ziua: ")),
		0,
		left_flag,
		4);
	
	choice_days = new wxChoice(pw, -1, wxDefaultPosition, 
							wxDefaultSize,
							arr_str_days);
	choice_days->SetSelection(0);
	
	this->Add(
		choice_days, 
		0,
		right_flag | wxEXPAND,
		4);
	
	
	this->Add(
		new wxStaticLine(pw,
				-1, wxDefaultPosition, wxDefaultSize,
				wxLI_VERTICAL),
		0,
		wxALL | wxEXPAND,
		0);		
		
	//Facultatea la care preda
	this->Add(
		new wxStaticText(pw, -1, _T("Facultatea: ")),
		0,
		left_flag,
		4);
		
	choice_fac = new wxChoice(pw, -1, wxDefaultPosition,
									wxDefaultSize,
									arr_str_fac_names);
	choice_fac->SetSelection(0);
	
	this->Add(
		choice_fac,
		0,
		right_flag,
		4);
		
	this->Add(
		new wxStaticLine(pw,
				-1, wxDefaultPosition, wxDefaultSize,
				wxLI_VERTICAL),
		0,
		wxALL | wxEXPAND,
		0);	
		
	//Materia
	this->Add(
		new wxStaticText(pw, -1, _T("Materia: ")),
		0,
		left_flag,
		4);
		
	edit_matter = new wxTextCtrl(pw, -1, _T(""), wxDefaultPosition,
						wxSize(60, 25), 0, wxDefaultValidator);
	edit_matter->SetMaxLength(6);
	this->Add(
		edit_matter,
		0,
		right_flag | wxEXPAND,
		4);
	this->Add(
		new wxStaticLine(pw,
				-1, wxDefaultPosition, wxDefaultSize,
				wxLI_VERTICAL),
		0,
		wxALL | wxEXPAND,
		0);	
		
	//Numar post
	this->Add(
		new wxStaticText(pw, -1, _T("Numar post: ")),
		0,
		left_flag,
		4);
	edit_post =  new wxTextCtrl(pw, -1, _T(""), wxDefaultPosition,
						wxSize(60, 25), 0, wxDefaultValidator);
	edit_post->SetMaxLength(6);
	this->Add(
		edit_post,
		0,
		right_flag | wxEXPAND,
		4);
	this->Add(
		new wxStaticLine(pw,
				-1, wxDefaultPosition, wxDefaultSize,
				wxLI_VERTICAL),
		0,
		wxALL | wxEXPAND,
		0);

	//Aplicatie sau curs
	this->Add(
		new wxStaticText(pw, -1, _T("Tip: ")),
		0,
		left_flag,
		4);
	wxArrayString radio_strings;
	radio_strings.Add(_T("aplicatie"));
	radio_strings.Add(_T("curs"));
 
	radio_type = new wxRadioBox(pw, -1, _T("Tip"), wxDefaultPosition,
			wxDefaultSize, radio_strings, 2, wxRA_SPECIFY_COLS);
	this->Add(
		radio_type,
		0,
		right_flag,
		4);
	this->Add(
		new wxStaticLine(pw,
				-1, wxDefaultPosition, wxDefaultSize,
				wxLI_VERTICAL),
		0,
		wxALL | wxEXPAND,
		0);
		
	//Seria si grupa
	this->Add(
		new wxStaticText(pw, -1, _T("Seria si grupa: ")),
		0,
		left_flag,
		4);
	edit_srs_grp = new wxTextCtrl(pw, -1, _T(""), wxDefaultPosition,
				wxSize(60, 25));
	edit_srs_grp->SetMaxLength(6);
	this->Add(
		edit_srs_grp,
		0,
		right_flag,
		4);
	this->Add(
		new wxStaticLine(pw,
				-1, wxDefaultPosition, wxDefaultSize,
				wxLI_VERTICAL),
		0,
		wxALL | wxEXPAND,
		0);	
	//Ora de start
	this->Add(
		new wxStaticText(pw, -1, _T("De la ora: ")),
		0,
		left_flag,
		4);
	spin_start_h = new wxSpinCtrl(pw, -1, _T(""), wxDefaultPosition,
				wxSize(40, 25), wxSP_ARROW_KEYS, 8, 20);
	
	this->Add(
		spin_start_h,
		0,
		right_flag,
		4);
	this->Add(
		new wxStaticLine(pw,
				-1, wxDefaultPosition, wxDefaultSize,
				wxLI_VERTICAL),
		0,
		wxALL | wxEXPAND,
		0);
		
	//Ora de sfarsit
	this->Add(
		new wxStaticText(pw, -1, _T("Pana la ora: ")),
		0,
		left_flag,
		4);
	spin_stop_h = new wxSpinCtrl(pw, -1, _T(""), wxDefaultPosition,
				wxSize(40, 25), wxSP_ARROW_KEYS, 8, 20);
	
	this->Add(
		spin_stop_h,
		0,
		right_flag,
		4);
	this->Add(
		new wxStaticLine(pw,
				-1, wxDefaultPosition, wxDefaultSize,
				wxLI_VERTICAL),
		0,
		wxALL | wxEXPAND,
		0);
		
	//Paritatea
	this->Add(
		new wxStaticText(pw, -1, _T("O data la : ")),
		0,
		left_flag,
		4);

	wxArrayString radio_par_str;
	radio_par_str.Add(_T("o"));
	radio_par_str.Add(_T("doua"));
	radio_par_str.Add(_T("trei"));
	radio_par_str.Add(_T("patru"));
	
	radio_par = new wxRadioBox(pw, -1, _T("saptamani"), wxDefaultPosition,
			wxDefaultSize, radio_par_str, 4, wxRA_SPECIFY_COLS);
	this->Add(
		radio_par,
		0,
		right_flag,
		4);
	this->Add(
		new wxStaticLine(pw,
				-1, wxDefaultPosition, wxDefaultSize,
				wxLI_VERTICAL),
		0,
		wxALL | wxEXPAND,
		0);
		
	//Selectarea saptamanii de start
	this->Add(
		new wxStaticText(pw, -1, _T("Ore incepand cu : ")),
		0,
		left_flag,
		4);

	wxArrayString radio_start_w_str;
	radio_start_w_str.Add(_T("prima"));
	radio_start_w_str.Add(_T("a doua"));
	radio_start_w_str.Add(_T("a treia"));
	radio_start_w_str.Add(_T("a patra"));
	
	radio_start_w = new wxRadioBox(pw, -1, _T("saptamana"), wxDefaultPosition,
			wxDefaultSize, radio_start_w_str, 4, wxRA_SPECIFY_COLS);
	this->Add(
		radio_start_w,
		0,
		right_flag,
		4);
	this->Add(
		new wxStaticLine(pw,
				-1, wxDefaultPosition, wxDefaultSize,
				wxLI_VERTICAL),
		0,
		wxALL | wxEXPAND,
		0);

	//Titulatura
	
	this->Add(
		new wxStaticText(pw, -1, _T("In calitate de: ")),
		0,
		left_flag,
		4);
	
	choice_title = new wxChoice(pw, -1, wxDefaultPosition, 
							wxDefaultSize,
							arr_str_titles);
	choice_title->SetSelection(0);
	
	this->Add(
		choice_title, 
		0,
		right_flag ,
		4);
	
	
	this->Add(
		new wxStaticLine(pw,
				-1, wxDefaultPosition, wxDefaultSize,
				wxLI_VERTICAL),
		0,
		wxALL | wxEXPAND,
		0);
		
	
	//Butonul more
	button_more = new wxButton(pw, ID_on_more, _T("More"), wxDefaultPosition,
						wxSize(60, 25));
	this->Add(
		button_more,
		0,
		wxEXPAND,
		0);

	//Butonul less
	button_less = new wxButton(pw, ID_on_less, _T("Less"), wxDefaultPosition,
					wxSize(60, 25));
	this->Add(
		button_less,
		0,
		wxEXPAND,
		0);
	
	this->Layout();
	
	
}
void OptionSizer :: HideMore(void)
{
	button_more->Hide();
}
void OptionSizer :: ShowMore(void)
{
	button_more->Show();
}
void OptionSizer :: HideLess(void)
{
	button_less->Hide();
}
void OptionSizer :: ShowLess(void)
{
	button_less->Show();
}

int OptionSizer :: GetIntDay()
{
	return choice_days->GetSelection();
}
