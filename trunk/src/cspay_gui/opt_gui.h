#ifndef __CEVA_H__
#define __CEVA_H__

class OptionSizer : public wxFlexGridSizer
{
/*
 * clasa cu widgeturile necesare alegerii
 * optiunilor
 */
	public:
		OptionSizer(wxWindow *pw);
		void HideMore(void);		//ascunde butonul more
		void ShowMore(void);		//arata butonul more
	private:
		wxTextCtrl *edit_type;		//tipul postului
		wxTextCtrl *edit_group;		//grupa
		wxChoice *choice_days;		//ziua selectata
		wxButton *button_more;		//butonul more

		wxArrayString array_days;	//zilele din saptamana

};

#endif
