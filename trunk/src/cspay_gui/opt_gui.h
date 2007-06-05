#ifndef __OPT_GUI_H__
#define __OPT_GUI_H__	1

class OptionSizer : public wxFlexGridSizer
{
/*
 * clasa cu widgeturile necesare alegerii
 * optiunilor
 */
	public:
		OptionSizer(wxWindow *pw);	//functia consturctor
						//pw parent window
		void HideMore(void);		//ascunde butonul more
		void ShowMore(void);		//arata butonul more
		void HideLess(void);		//ascunde butonul less
		void ShowLess(void);		//arata butonul less
		wxString GetType();		//returneaza tipul cursului
		int GetIntDay();		//returneaza ziua, int
		wxString GetStrDay();		//retunreaza ziua, string
		wxString GetGroup();		//returneaza grupa
	
	private:
		wxTextCtrl *edit_type;		//tipul postului
		wxTextCtrl *edit_group;		//grupa
		wxChoice *choice_days;		//ziua selectata
		wxButton *button_more;		//butonul more
		wxButton *button_less;		//butonul less

		wxArrayString array_days;	//zilele din saptamana

};

#endif
