#ifndef __ULINKLIST_H__
#define __ULINKLIST_H__
__BEGIN_DECLS



struct ulinklist* ulinklist_create(void);
struct ulinklist* ulinklist_create_detail(uf_pointer_free pointer_free, uf_pointer_description  pointer_description);
int ulinklist_destroy(struct ulinklist* list);


struct ulinklist_node;
int ulinklist_length(struct ulinklist* list);
void* ulinklist_header_value(struct ulinklist* list, struct ulinklist_node** ppnode);
void* ulinklist_tailer_value(struct ulinklist* list, struct ulinklist_node** ppnode);


int ulinklist_add_header(struct ulinklist* list, void* p);
int ulinklist_add_tailer(struct ulinklist* list, void* p);

void* ulinklist_pop(struct ulinklist* list);
int ulinklist_description(struct ulinklist* list, char* s, size_t size);



#if 0
void* ulinklist_next(struct ulinklist* list);
void* ulinklist_prev(struct ulinklist* list);




int ulinklist_remove(struct ulinklist* list, void* remove, bool clear);
#endif







#if uvdata
struct ulinklist* ulinklist_uv_create_detail(uvdata_free_func data_free);
struct uvdata* ulinklist_header(struct ulinklist* list);
struct uvdata* ulinklist_tailer(struct ulinklist* list);
int ulinklist_add_header_by_copy(struct ulinklist* list, void* p, size_t size);
int ulinklist_add_tailer_by_copy(struct ulinklist* list, void* p, size_t size);
int ulinklist_add_after_by_copy(struct ulinklist* list, struct uvdata* data, void* p, size_t size);
int ulinklist_add_before_by_copy(struct ulinklist* list, struct uvdata* data, void* p, size_t size);
struct uvdata* ulinklist_next(struct ulinklist* list);
struct uvdata* ulinklist_prev(struct ulinklist* list);
struct uvdata* ulinklist_at(struct ulinklist* list, int idx);
#endif

__END_DECLS
#endif /* ulinklist.h */
