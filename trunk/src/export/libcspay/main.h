#ifndef _MAIN_H
#define _MAIN_H 1

extern char *Name;		/** for who we generate? */
extern char *FacultyName;	/** Name's faculty */
extern int Format;		/** zip or ods */
extern char *BaseCourse;	
extern int Month;		/** mask, 1 ->january, 3->jan & feb */
extern MYSQL *Conn;		/** MySQL db connection */

#endif /* _MAIN_H */
