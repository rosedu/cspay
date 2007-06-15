#ifndef __OPT_GUI_H__
#define __OPT_GUI_H__	1

class OptionSizer : public wxFlexGridSizer
{
/*
 * clasa cu widgeturile necesare alegerii
 * optiunilor
 */
	public:
		OptionSizer(wxWindow *pw,
					wxArrayString arr_str_days,
					wxArrayString arr_str_fac_names,
					wxArrayString arr_str_titles);	
									
		void HideMore(void);		//ascunde butonul more
		void ShowMore(void);		//arata butonul more
		void HideLess(void);		//ascunde butonul less
		void ShowLess(void);		//arata butonul less

		int GetIntDay();			//returneaza ziua, int
	
	protected:
		wxButton *button_more;		//butonul more
		wxButton *button_less;		//butonul less
	
		wxChoice *choice_days;		//ziua selectata
		wxChoice *choice_fac;		//facultatea la care preda
		wxChoice *choice_title;		//titulatura, asitent etc.
	
		wxRadioBox *radio_type;		//tipul postuli
		wxRadioBox *radio_par;		//paritea
		wxRadioBox *radio_start_w;	//saptamana de start

		wxSpinCtrl *spin_start_h;	//ora de inceput
		wxSpinCtrl *spin_stop_h;	//ora de sfarsti
	
		wxTextCtrl *edit_type;		//tipul postului
		wxTextCtrl *edit_matter;	//materia
		wxTextCtrl *edit_post;		//numarul postului
		wxTextCtrl *edit_srs_grp;	//seria si grupa
};

#endif
