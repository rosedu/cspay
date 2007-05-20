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

#include "opt_gui.h"
#include "cspay_gui.h"	//FIXME: asta nu trebuie inclus

OptionSizer :: OptionSizer(wxWindow *pw)
: wxFlexGridSizer(8, 2)
{
	//Felul postului
	this->Insert(0, new wxStaticText(pw, -1, _T("Fel post:")), 0,
		wxALIGN_RIGHT, 0);
	edit_type = new wxTextCtrl(pw, -1);
	this->Insert(1, edit_type, 0, wxALIGN_LEFT, 0);


	//Grupa
	this->Insert(2, new wxStaticText(pw, -1, _T("Grupa:")), 0,
		wxALIGN_RIGHT, 0);
	edit_group =  new wxTextCtrl(pw, -1);
	this->Insert(3, edit_group, 0, wxALIGN_LEFT, 0);


	//Ziua din saptamana
	this->Insert(4, new wxStaticText(pw, -1, _T("Ziua:")), 0,
		wxALIGN_LEFT, 0);

	array_days.Add(_T("Luni"));
	array_days.Add(_T("Marti"));
	array_days.Add(_T("Miercuri"));
	array_days.Add(_T("Joi"));
	array_days.Add(_T("Vineri"));
	array_days.Add(_T("Sambata"));
	array_days.Add(_T("Duminica"));

	choice_days = new wxChoice(pw, -1, wxDefaultPosition, wxDefaultSize,
			array_days);
	this->Insert(5, choice_days, 0, wxALIGN_LEFT, 0);


	//Butonul more
	button_more = new wxButton(pw, ID_on_more, _T("More"));
	this->Insert(6, button_more, 0, wxEXPAND, 0);

	//Butonul less
	button_less = new wxButton(pw, ID_on_less, _T("Less"));
	this->Insert(7, button_less, 0, wxEXPAND, 0);

	//this->SetSizeHints(pw);
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
