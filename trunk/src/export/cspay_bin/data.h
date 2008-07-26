#ifndef _DATA_H
#define _DATA_H 1

#define HOUR	3600		/**< 1 hour in seconds*/
#define DAY	(24 * HOUR)	/**< 1 day in seconds*/
#define WEEK	(7 * DAY)	/**< 1 week in seconds*/

/**
 * role types
 */
#define LCSP_ROLE_ASS	3	/**< assinstant type :-) */
#define LCSP_ROLE_CON	1	/**< "conferentiar" */
#define LCSP_ROLE_SEL	2	/**< "sef lucrari" */
#define LCSP_ROLE_TEA	0	/**< teacher */

/**
 * class types
 */
#define LCSP_CLASS_CRS	0	/**< course */
#define LCSP_CLASS_APP	1	/**< application */

#endif /* _DATA_H */
