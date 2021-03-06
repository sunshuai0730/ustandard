#ifndef __USLOG_H__
#define __USLOG_H__
__BEGIN_DECLS


/* item define. */
#define USLOG_ITEM_ENABLE_FILEINFO       (1<<1)
#define USLOG_ITEM_ENABLE_PTHREAD_ID     (1<<2)
#define USLOG_ITEM_ENABLE_PID            (1<<3)
#define USLOG_ITEM_ENABLE_TIMEVAL        (1<<4)
#define USLOG_ITEM_ENABLE_DATATIME       (1<<5)
#define USLOG_ITEM_ENABLE_MODULE         (1<<6)
#define USLOG_ITEM_ENABLE_ALL            (0xFFFFFFFF)


/* setopt tag. */
#define USLOG_CONFIG_OUTSTREAM          1
#define USLOG_CONFIG_ITEM               2
#define USLOG_CONFIG_PREFIX             3
#define USLOG_CONFIG_SUFFIX             4


struct uslog_data;



int uslog_set_config_filename(const char* name);

/* it's better to call it in the beginning of the process. */
struct uslog_data* uslog_init(void);

/* call before process exit. */
int uslog_deinit(struct uslog_data* logctx);


/* int uslog_config_set(int level, struct uslog_config* config); */
int uslog_config_setopt(struct uslog_data** logctx, int level, int tag, ...);


int uslog_is_enabled(struct uslog_data** logctx, int level);
int uslog_fprintf(struct uslog_data** logctx, int level, 
        const char* file, const char* function, unsigned int line,const char* s);


/* uslog implement. */
#define USLOG_LEN_STRINGP   (10240)
#define __uslog(logctx, level, fmt...)                                         \
do {                                                                           \
    if(!uslog_is_enabled(&(logctx), level)) {                                  \
        break;                                                                 \
    }                                                                          \
    char str_qwasxz[USLOG_LEN_STRINGP];                                        \
    snprintf(str_qwasxz, USLOG_LEN_STRINGP, fmt);                              \
    uslog_fprintf(&(logctx), level,                                            \
            __FILE__, __FUNCTION__, __LINE__, str_qwasxz);                     \
}while(0)


#if 1
/* define level if caller want to use __v0, __vt, __vi, __vd, __ve .
   change ustandard level log. level should be 0<=level<10. */
#define USLOG_USTANDARD_TEST    1
#define USLOG_USTANDARD_INFO    2
#define USLOG_USTANDARD_DEBUG   3
#define USLOG_USTANDARD_ERROR   4
#define USLOG_MODULE 0
#define __v0     ((USLOG_MODULE)*8 + 0)
#define __vt     ((USLOG_MODULE)*8 + (USLOG_USTANDARD_TEST))
#define __vi     ((USLOG_MODULE)*8 + (USLOG_USTANDARD_INFO))
#define __vd     ((USLOG_MODULE)*8 + (USLOG_USTANDARD_DEBUG))
#define __ve     ((USLOG_MODULE)*8 + (USLOG_USTANDARD_ERROR))
#define USLOG_MODULE_LEVEL(module, level) ((module)*8 + (level)%8)
#endif


#define uslog(level, fmt...)
    

#define usloge_perror(c) \
            ulogerr("%s error : %s(%s).\n", __FUNCTION__, c, strerror(errno))

#define usloge_malloc(size) \
            ulogerr("%s malloc failed(size%zd).\n", __FUNCTION__, size)

#define usloge_realloc(size) \
            ulogerr("%s realloc failed(size%zd).\n", __FUNCTION__, size)

#define uslog_check_arg(cond, retval) if(!(cond)) {ulogerr("%s error : invalid arg.\n", __FUNCTION__); errno = EINVAL; return retval;}




const char* uslog_level_blank(int level);
int uslog_function_enter(const char* file, const char* function, int line);
int uslog_function_return(const char* file, const char* function, int line);

extern __thread int tp_function_level ;
extern __thread char* tp_function[1024];




#define ENTER__ {\
    uloginf("%s[%d]%s\n", uslog_level_blank(tp_function_level), tp_function_level, __FUNCTION__); \
    uslog_function_enter(__FILE__, __FUNCTION__, __LINE__);\
}


#define RETURN__ do{\
    uloginf("%s\e[34;1m[%d]\e[0m%s\n", uslog_level_blank(tp_function_level-1), tp_function_level-1, __FUNCTION__);\
    uslog_function_return(__FILE__, __FUNCTION__, __LINE__); \
    return 


#define RETURN_END__ }while(0);


#define RETURN_VOID_ADD__ do{\
    uloginf("%s\e[34;1m[%d]\e[0m%s\n", uslog_level_blank(tp_function_level-1), tp_function_level-1, __FUNCTION__);\
    uslog_function_return(__FILE__, __FUNCTION__, __LINE__); \
    return;\
}while(0);



#define RETURN_TYPE__(type, value) do { \
    type retvalqwe = value; \
    uloginf("%s\e[34;1m[%d]\e[0m%s\n", uslog_level_blank(tp_function_level-1), tp_function_level-1, __FUNCTION__);\
    uslog_function_return(__FILE__, __FUNCTION__, __LINE__); \
    return retvalqwe;}while(0)

#define RETURN_TYPE_END__
































__END_DECLS
#endif /* uslog.h */
