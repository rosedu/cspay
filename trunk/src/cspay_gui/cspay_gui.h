#ifndef __CSPAY_GUI_H__
#define __CSPAY_GUI_H__ 1

#define OMAX	10	//cate reguli putem da, maxim
#define FMAX	4	//cate facultati avem, maxim
class CSpayGUI : public wxApp
{
	public :
		virtual bool OnInit();
	private:
		wxArrayString arr_str_days;				//
		wxArrayString arr_str_fac_names;		//
		wxArrayString arr_str_titles;			//
		wxArrayString arr_str_short_titles;		//
		wxArrayString arr_str_dep_names[FMAX];	//
	
		class MainFrame *frame;
};

class MainFrame :  public wxFrame
{
	public :
		MainFrame(
					wxArrayString arr_str_days,
					wxArrayString arr_str_fac_names,
					wxArrayString arr_str_titles,
					wxArrayString arr_str_short_titles,
					wxArrayString arr_str_dep_names[]);
	
		void OnSelectFac(wxCommandEvent& event);	//selectarea facultatii
		void OnPressMore(wxCommandEvent& event);	//apasarea lui more
		void OnPressLess(wxCommandEvent& event);	//apasarea lui less
		void OnPressSave(wxCommandEvent& event);	//apasarea lui save
		void OnPressQuit(wxCommandEvent& event);	//apasarea lui quit :)

		int n; 		//cate optiuni avem afisate

		class OptionSizer *rules[OMAX];	//regulile

		DECLARE_EVENT_TABLE()
		
	private:
		wxGridSizer *main_sizer;	//gridul principal
		wxTextCtrl *edit_name;		//edit box cu numele
		wxChoice *choice_fac;		//choice cu facultatea
		wxChoice *choice_dep;		//choice pentru catedra
	
		wxStaticText *labels[OMAX];	//etichete cu numar curent
		wxArrayString p_arr_str_dep_names[FMAX];
		wxArrayString p_arr_str_days;
		wxArrayString p_arr_str_fac_names;
		wxArrayString p_arr_str_titles;
		wxArrayString p_arr_str_short_titles;
		wxStaticLine *sl1[OMAX];	//liniile orizontale
		wxStaticLine *sl2[OMAX];
};

enum {
	ID_on_select_fac = 1,	//evenimentul pentru selectarea facultatii
	ID_on_more,				//evenimetntul pentru butonul more, din opt
	ID_on_less,
	ID_on_save,
	ID_on_quit,
};

#endif
