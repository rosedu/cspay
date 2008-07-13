#ifndef _DATA_H
#define _DATA_H 1

#define HOUR	3600		/**< 1 hour in seconds*/
#define DAY	(24 * HOUR)	/**< 1 day in seconds*/
#define WEEK	(7 * DAY)	/**< 1 week in seconds*/

/**
 * role types
 */
#define LCSP_ROLE_ASS	0	/**< assinstant type :-) */
#define LCSP_ROLE_CON	1	/**< "conferentiar" */
#define LCSP_ROLE_SEL	2	/**< "sef lucrari" */
#define LCSP_ROLE_TEA	3	/**< teacher */

/**
 * class types
 */
#define LCSP_CLASS_CRS	0	/**< course */
#define LCSP_CLASS_APP	1	/**< application */
/**
 * \struct class_info
 * contains all the information about a specific class
 */
struct class_info {
	/** name length, see bellow*/
	int name_len;
	/** name of course, usefull in reading information from ini file
	 * eg: "ore/1", "ore/2"
	 */
	char name[8];

	/** day in week of class*/
	int day;

	/** class parity
	 * 1 - every week
	 * 2 - every two weeks
	 * etc.
	 */
	int parity;
	/** first week of class
	 * 1 - the class begin in first *valid* week of the month
	 * 2 - the class begin in second *valid* week of the month
	 * etc.
	 * \todo check if this is implemented corectly
	 */
	int first_week;

	/** role type
	 * 0 - assistant
	 * 1 - "conferentiar"
	 * 2 - "sef lucrari"
	 * 3 - teacher
	 */
	int role_type;
	/** role number/name*/
	char *role_num;

	/** name of class, a.k.a. disciplina*/
	char *class;

	/** short name of faculty*/
	char *faculty;
	
	/** group name of class
	 * eg: 312CC*/
	char *group;

	/** class type
	 * 0 - course
	 * 1 - application
	 */
	int class_type;

	/**
	 * time of class
	 * \remarks interval is used as int
	 */
	struct interval timeline;
	
};


#endif /* _DATA_H */
