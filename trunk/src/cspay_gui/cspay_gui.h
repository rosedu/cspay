#ifndef __CSPAY_GUI_H__
#define __CSPAY_GUI_H__ 1

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
		void OnSelectFac(wxCommandEvent& event);
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
		wxChoice *choice_cat;		//choice pentru catedra
		wxTextCtrl *edit_course;	//edit box cu numele cursului

		wxArrayString faculty_names;	//stringuri cu toate facultatile
		wxArrayString catedre_names;	//stringuri cu catedrele (din fac selecteata)
};

enum {
	ID_on_select_fac = 1,			//evenimentul pentru selectarea facultatii
};

#endif
