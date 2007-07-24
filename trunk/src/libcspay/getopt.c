#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "cspay.h"
#include "getopt.h"

/*extrage informatiile din arborle extras din fisierul xml*/
void xml_extract_from_tree(xmlNode *element, struct cspay_config *ret);
	
/*adauga in load informatii depsre restrictiile din nodul node*/
void xml_parse_rest_element(xmlNode *node, struct cspay_config *load);
	
/*adauga in load informatii despre facultate din nodul node*/
void xml_parse_fac_element(xmlNode *node, struct cspay_config *load);
	
/*adauga in load informatii depsre universitate*/
void xml_parse_univ_element(xmlNode *node, struct cspay_config *load);
	
struct cspay_config *
read_cspay_xml(char *fname)
{
	/*TODO
	 * validarea fisierului
	 */
	struct cspay_config *ret;
	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;
	
	LIBXML_TEST_VERSION
	
	doc = xmlReadFile(fname, NULL, 0);
	if (!doc){
		fprintf(stderr, "Nu am putut citi fisierul indicat\n");
		doc = xmlReadFile("cspay.xml", NULL, 0);
		if (!doc) {
			fprintf(stderr, "Nu am putut citi nici cspay.xml\n");
			return NULL;
		}
	}
	
	#ifdef __DEBUG__
	printf("Am citit fisierul *.xml\n");
	#endif /* __DEBUG__ */
	
	root_element = xmlDocGetRootElement(doc);
	if (!root_element){
		fprintf(stderr, "root_element\n");
		return NULL;
	}
	if (xmlStrncmp(root_element->name, (xmlChar *)"cspay-config", 12)) {
		fprintf(stderr, "Nu este fisierul pe care-l cautam\n");
		return NULL;
	}
	ret = malloc(sizeof (struct cspay_config));
	if (!ret) {
		fprintf(stderr, "Nu am putut aloca struct cspay_config");
		return NULL;
	}
	ret->rest_no = 0;
	ret->fac_no = 0;
	
	#ifdef __DEBUG__
	printf("Am citi root_element\n");
	#endif /* __DEBUG__ */
	
	xml_extract_from_tree(root_element, ret);
	
	xmlFreeDoc(doc);
	xmlCleanupParser();
	return ret;
}
void 
xml_extract_from_tree(xmlNode *element, struct cspay_config *load)
{
	xmlNode *cur_node;
	for (cur_node=element; cur_node; cur_node=cur_node->next){
		if (cur_node->type == XML_ELEMENT_NODE){
			if (!xmlStrcmp(cur_node->name, (xmlChar *)"vacanta")){
				#ifdef __DEBUG__
				printf("Sunt pe <vacante>\n");
				#endif /* __DEBUG__ */
				
				xml_parse_rest_element(cur_node, load);
			}															
			if (!xmlStrcmp(cur_node->name, (xmlChar *)"facultate")){
				
				#ifdef __DEBUG__
				printf("Sunt pe <facultate>\n");
				#endif /* __DEBUG__ */
				
				xml_parse_fac_element(cur_node, load);
			}
			if (!xmlStrcmp(cur_node->name, (xmlChar *)"universitate")){
				
				#ifdef __DEBUG__
				printf("Sunt pe <universitate>\n");
				#endif /* __DEBUG__ */
				
				xml_parse_univ_element(cur_node, load);
			}
		}
		xml_extract_from_tree(cur_node->children, load);
	}
}

void
xml_parse_rest_element(xmlNode *node, struct cspay_config *load)
{
	#ifdef __DEBUG__
	printf("Tratez un nod pentru restrictii\n");
	#endif /* __DEBUG__ */

	time_t start;
	time_t end;
	struct tm *conv_to;
	
	xmlAttr *prop;
	xmlNode *prop_val;
	
	prop = node->properties;
	
	if (xmlStrncmp(prop->name, (xmlChar *)"data_start", 10))
		return;
	prop_val=prop->children;
	
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
		else return;
			
	prop_val=prop->next->children;
		
	/*zz.ll.aaaa*/
	sscanf((char *)prop_val->content, "%d.%d.%d", &conv_to->tm_mday, &conv_to->tm_mon, 
			&conv_to->tm_year);
	/*fix data*/
	-- conv_to->tm_mon;	/* january is 0*/
	conv_to->tm_year -= 1900;	/* 1900 is 0*/
	
	end = mktime(conv_to) - 1;	/*interval deschis*/
	free(conv_to);
	
	load->start[load->rest_no] = start;
	load->end[load->rest_no] = end;
	++ load->rest_no;
		
		
	#ifdef __DEBUG__
	printf("[%d]:\tSTART: %ld\tEND: %ld\n", load->rest_no, (unsigned long)start, 
		(unsigned long)end);	
	printf("Am terminat cu nodul pentru restrictii\n");
	#endif /* __DEBUG__ */
}
void
xml_parse_fac_element(xmlNode *node, struct cspay_config *load)
{
	#ifdef __DEBUG__
	printf("Tratez un nod cu facultate\n");
	#endif /* __DEBUG__ */
	
	xmlAttr *prop;
	xmlNode *dept;
	struct faculty *fac;
 	struct department *dep;
	fac = malloc(sizeof (struct faculty));
		
	for (prop = node->properties; prop; prop=prop->next){
		if (prop->type == XML_ATTRIBUTE_NODE) {/* nu trebuie*/
			if (!xmlStrcmp(prop->name, (xmlChar *)"decan"))
				fac->decan = strdup((char *)prop->children->content);
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
			
	#ifdef __DEBUG__
	printf("%s\t%s\t%s\n", fac->name, fac->decan, fac->short_name);
	int i;
	for (i = 0; i < fac->dept_no; ++ i)
		printf("\t%s\t%s\n", fac->depts[i]->name, fac->depts[i]->chief);
	printf("Am terminat cu nodul cu facultate\n");
	#endif /* __DEBUG__ */
}
void 
xml_parse_univ_element(xmlNode *node, struct cspay_config *load)
{
	xmlAttr *prop;
	
	for (prop=node->properties; prop; prop=prop->next)
		if (prop->type == XML_ATTRIBUTE_NODE)
			if (!xmlStrcmp(prop->name, (xmlChar *)"nume")){
				load->univ_name = strdup((char *)prop->children->content);
			}
	#ifdef __DEBUG__
	printf("Am extras numele universitatii :%s\n", load->univ_name);
	#endif
}
