#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "debug.h"
#include "cspay.h"
#include "getopt.h"

/*extrage informatiile din arborle extras din fisierul xml*/
void cspay_xml_extract_from_tree(xmlNode *element, struct cspay_config *ret);
	
/*adauga in load informatii depsre restrictiile din nodul node*/
void cspay_xml_parse_rest_element(xmlNode *node, struct cspay_config *load);
	
/*adauga in load informatii despre facultate din nodul node*/
void cspay_xml_parse_fac_element(xmlNode *node, struct cspay_config *load);
	
/*adauga in load informatii depsre universitate*/
void cspay_xml_parse_univ_element(xmlNode *node, struct cspay_config *load);
	
/*adauga in load informatii depsre semestru*/
void cspay_xml_parse_sem_element(xmlNode *node, struct cspay_config *load);
	
struct cspay_config *
read_cspay_xml(char *fname)
{
	/*TODO
	 * validarea fisierului
	 */
	struct cspay_config *cfg;
	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;
	
	LIBXML_TEST_VERSION
	
	doc = xmlReadFile(fname, NULL, 0);
	if (!doc){
		fprintf(stderr, "*.xml file not found, try cspay.xml\n");
		doc = xmlReadFile("cspay.xml", NULL, 0);
		if (!doc) {
			fprintf(stderr, "cspay.xml not found, bye!\n");
			return NULL;
		}
	}
	
	Dprintf("Read *.xml\n");
	
	root_element = xmlDocGetRootElement(doc);
	if (!root_element){
		fprintf(stderr, "root_element\n");
		return NULL;
	}
	if (xmlStrncmp(root_element->name, (xmlChar *)"cspay-config", 12)) {
		fprintf(stderr, "This is not cspay-config xml file\n");
		return NULL;
	}
	cfg = malloc(sizeof (struct cspay_config));
	if (!cfg) {
		fprintf(stderr, "malloc cspay_config");
		return NULL;
	}
	cfg->vac_no = 0;
	cfg->fac_no = 0;
	
	Dprintf("Read root_element\n");
	
	cspay_xml_extract_from_tree(root_element, cfg);
	
	xmlFreeDoc(doc);
	xmlCleanupParser();

	return cfg;
}

/*
 * recursive XML info extraction
 */

void 
cspay_xml_extract_from_tree(xmlNode *element, struct cspay_config *load)
{
	xmlNode *node;

	for (node = element; node; node = node->next){
		if (node->type == XML_ELEMENT_NODE){
			if (!xmlStrcmp(node->name, (xmlChar *)"vacanta")){
				Dprintf("On <vacante>\n");
				cspay_xml_parse_rest_element(node, load);
			}															
			if (!xmlStrcmp(node->name, (xmlChar *)"facultate")){
				Dprintf("On <facultate>\n");
				cspay_xml_parse_fac_element(node, load);
			}
			if (!xmlStrcmp(node->name, (xmlChar *)"universitate")){
				Dprintf("On <universitate>\n");
				cspay_xml_parse_univ_element(node, load);
			}
			if (!xmlStrcmp(node->name, (xmlChar *)"semestru")){
				Dprintf("On <semestru>\n");
				cspay_xml_parse_sem_element(node, load);
			}
		}
		cspay_xml_extract_from_tree(node->children, load);
	}
}

void
cspay_xml_parse_rest_element(xmlNode *node, struct cspay_config *load)
{

	time_t start;
	time_t end;
	struct tm *conv_to;
	
	Dprintf("Begin <vacanta> node\n");

	xmlAttr *prop;
	xmlNode *prop_val;
	
	prop = node->properties;
	
	if (xmlStrncmp(prop->name, (xmlChar *)"data_start", 10))
		return;
	prop_val = prop->children;
	
	conv_to = malloc(sizeof (struct tm));
	memset(conv_to, 0, sizeof (struct tm));
		
	/*FIXME chestia asta parca e periculoasa*/
	
	/*zz.ll.aaaa*/
	sscanf((char *)prop_val->content, "%d.%d.%d", &conv_to->tm_mday, &conv_to->tm_mon, 
			&conv_to->tm_year);
	/*fix data*/
	-- conv_to->tm_mon;	/* january is 0*/
	conv_to->tm_year -= 1900;	/* 1900 is 0*/
	
	start = mktime(conv_to);
	
	if (prop->next) {
		if (xmlStrncmp(prop->next->name, (xmlChar *)"data_end", 8))
			return;
	}
	else
		return;
			
	prop_val = prop->next->children;

	/*zz.ll.aaaa*/
	sscanf((char *)prop_val->content, "%d.%d.%d", &conv_to->tm_mday, &conv_to->tm_mon, 
			&conv_to->tm_year);
	/*fix data*/
	-- conv_to->tm_mon;	/* january is 0*/
	conv_to->tm_year -= 1900;	/* 1900 is 0*/
	
	end = mktime(conv_to);	
	free(conv_to);
	
	load->vac[load->vac_no] = malloc(sizeof (struct interval));
	load->vac[load->vac_no]->start = start;
	load->vac[load->vac_no]->end = end;
	++ load->vac_no;
		
	Dprintf("[%d]: from %ld to %ld\n", load->vac_no, (unsigned long)start, 
		(unsigned long)end);	
	Dprintf("End <vacanta> node\n");
}

void
cspay_xml_parse_fac_element(xmlNode *node, struct cspay_config *load)
{
	xmlAttr *prop;
	xmlNode *dept;
	struct faculty *fac;
 	struct department *dep;
	fac = malloc(sizeof (struct faculty));

	Dprintf("Begin facultate\n");

	for (prop = node->properties; prop; prop=prop->next){
		if (prop->type == XML_ATTRIBUTE_NODE) {/* nu trebuie*/
			if (!xmlStrcmp(prop->name, (xmlChar *)"decan"))
				fac->dean = strdup((char *)prop->children->content);
			if (!xmlStrcmp(prop->name, (xmlChar *)"nume"))
				fac->name = strdup((char *)prop->children->content);
			if (!xmlStrcmp(prop->name, (xmlChar *)"nume_scurt"))
				fac->short_name = strdup((char *)prop->children->content);
		}
		
	}

	fac->dept_no = 0;
	for (dept = node->children; dept; dept=dept->next)
		if (dept->type == XML_ELEMENT_NODE)
			if (!xmlStrcmp(dept->name, (xmlChar *)"catedra")) {
				dep = malloc(sizeof (struct department));
				for (prop = dept->properties; prop; prop=prop->next)
					if (prop->type == XML_ATTRIBUTE_NODE) {
						if (!xmlStrcmp(prop->name, (xmlChar *)"nume")) {
							/*FIXME malloc, assert prop->children*/
							dep->name = strdup((char *)prop->children->content);
						}
						if (!xmlStrcmp(prop->name, (xmlChar *)"sef")) {
							dep->chief = strdup((char *)prop->children->content);
						}

					}
				fac->depts[fac->dept_no] = dep;
				++ fac->dept_no;
			}
	load->fac[load->fac_no] = fac;
	++ load->fac_no;

	{
		int i;
		Dprintf("%s\t%s\t%s\n", fac->name, fac->dean, fac->short_name);
		for (i = 0; i < fac->dept_no; ++ i)
			Dprintf("\t%s\t%s\n", fac->depts[i]->name, fac->depts[i]->chief);
		Dprintf("End facultate\n");
	}
}

void 
cspay_xml_parse_univ_element(xmlNode *node, struct cspay_config *load)
{
	xmlAttr *prop;
	Dprintf("Begin universitate\n");
	for (prop=node->properties; prop; prop=prop->next)
		if (prop->type == XML_ATTRIBUTE_NODE)
			if (!xmlStrcmp(prop->name, (xmlChar *)"nume")){
				load->univ_name = strdup((char *)prop->children->content);
			}
	Dprintf("End universitate :%s\n", load->univ_name);
}

void 
cspay_xml_parse_sem_element(xmlNode *node, struct cspay_config *load)
{
	xmlAttr *prop;
	Dprintf("Begin semester\n");
	struct tm *conv_to;
	conv_to = calloc(1, sizeof (struct tm));
	load->sem = malloc(sizeof (struct interval));
		
	for (prop=node->properties; prop; prop=prop->next)
		if (prop->type == XML_ATTRIBUTE_NODE) {
			if (!xmlStrcmp(prop->name, (xmlChar *)"data_start")){
				sscanf((char *)prop->children->content, "%d.%d.%d",
					&conv_to->tm_mday, &conv_to->tm_mon, &conv_to->tm_year);
				-- conv_to->tm_mon;	/* january is 0*/
				conv_to->tm_year -= 1900;	/* 1900 is 0*/
				load->sem->start = mktime(conv_to);
			}
			if (!xmlStrcmp(prop->name, (xmlChar *)"data_sfarsit")){
				sscanf((char *)prop->children->content, "%d.%d.%d",
					&conv_to->tm_mday, &conv_to->tm_mon, &conv_to->tm_year);
				-- conv_to->tm_mon;	/* january is 0*/
				conv_to->tm_year -= 1900;	/* 1900 is 0*/
				load->sem->end = mktime(conv_to);
			}
		}
	free(conv_to);
	Dprintf("End semester\n");
	Dprintf("Semester begin at %ld and ends at %ld\n",
		load->sem->start, load->sem->end);
}
