#ifndef __CSPAY_GUI_H__
#define __CSPAY_GUI_H__ 1

#define OMAX	10	//cate reguli putem da, maxim

class CSpayGUI : public wxApp
{
	public :
		virtual bool OnInit();
	private:
		class MainFrame *frame;
};

class MainFrame :  public wxFrame
{
	public :
		MainFrame(const wxString &title, const wxPoint& pos, const wxSize& size);
		void OnSelectFac(wxCommandEvent& event);	//selectarea facultatii
		void OnPressMore(wxCommandEvent& event);	//apasarea lui more
		void OnPressLess(wxCommandEvent& event);	//apasarea lui less
		void OnPressSave(wxCommandEvent& event);
		void OnPressQuit(wxCommandEvent& event);

		int n; 				//cate de optiuni avem afisate

		class OptionSizer *rules[OMAX];	//butonase pentru reguli
	/*
	 * Framul principal ale tabela de evenimente
	 */
	DECLARE_EVENT_TABLE()

	/*
	 * aici sunt variabilele care vor fi accesibile
	 * doar din interiorul clasei.
	 * Variabilele astea ne sunt utile pentru
	 * aflarea informatiilor completate de utilizator
	 */
	private:
		wxGridSizer *main_sizer;	//gridul principal
		wxTextCtrl *edit_name;		//edit box cu numele
		wxChoice *choice_fac;		//choice cu facultatea
		wxChoice *choice_dep;		//choice pentru catedra
		wxTextCtrl *edit_course;	//edit box cu numele cursului

		wxArrayString faculty_names;	//stringuri cu toate facultatile
		wxArrayString dep_names;	//stringuri cu catedrele (din fac selecteata)

		wxStaticText *labels[OMAX];	//etichete cu numar curent, adaugare reguli
						//FIXME trebuia gandit altfel
};

enum {
	ID_on_select_fac = 1,			//evenimentul pentru selectarea facultatii
	ID_on_more,				//evenimetntul pentru butonul more, din opt
	ID_on_less,
	ID_on_save,
	ID_on_quit,
};

#endif
