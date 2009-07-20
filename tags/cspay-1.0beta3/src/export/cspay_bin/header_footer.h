#ifndef _HEADER_FOOTER_H
#define _HEADER_FOOTER_H 1

/**
 * \struct sum_hours 
 * contains sums from final table
 */
struct sum_hours {
	/** sum of hours as teacher*/
	int prof;

	/** sum of hours as "conferentiar"*/
	int conf;

	/** sum of hours as "sef lucrari"*/
	int sl;

	/** sum of hours as assistant*/
	int as;
};

/**
 * \struct total_hours
 * contains sum hour
 */
struct total_hours {
	/** for course*/
	struct sum_hours course;
	/** for applications*/
	struct sum_hours aplic;
};


int create_header(void);
int create_footer(int last_row, struct total_hours *);

#endif /* _HEADER_FOOTER_H */
