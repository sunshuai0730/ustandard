#include "ustandard/ustandard_sys.h"
#include "ustandard/udevelop.h"
#include "ustandard/ustring.h"
#include "ustandard/ufile.h"

size_t ufile_length(const char* filename)
{
    long file_length;
    FILE* fp = fopen(filename, "r");
    if(NULL == fp)
    {
        ulogerr("%s : fopen <%s>\n", "ufile", filename);
        return 0;
    }

    int ret = fseek(fp, 0, SEEK_END);
    if(0 != ret)
    {
        ulogerr("%s : fseek <%s> .\n", "ufile", filename);
        return 0;
    }

    file_length = ftell(fp);
    fclose(fp);
    
    return file_length;
}


size_t ufile_read(const char* filename, void* ptr, size_t size)
{
    size_t ret = 0;

    FILE* fp = fopen(filename, "r");
    if(NULL == fp)
    {
        ulogerr("%s : fopen <%s>\n", "ufile", filename);
        return 0;
    }

    ret = fread(ptr, 1, size, fp);
    if(ret == 0)
    {
        ulogerr("%s : fread. \n", "ufile");
    }

    fclose(fp);

    return ret;
}


/* use um_free(p) to free the return string value. */
char* ufile_dup(const char* filename, size_t* len)
{
    char* rets = NULL;

    size_t length = ufile_length(filename);
    if(len) {
        *len = length;
    }

    if(length > 0 
            && NULL != (rets= (char*)um_malloc(length + 1))
            ) {
        ufile_read(filename, rets, length);
        rets[length] = '\0';
    }

    return rets;
}


int ufile_insert_content(const char* filename, long index, 
        void *ptr, size_t size, size_t nmemb)
{
    FILE* fp = fopen(filename, "a");
    if(NULL == fp)
    {
        ulogerr("ufile : %s\n", "open file error.");
        return -1;
    }

    int retf;
    int nfwr;

    retf = fseek(fp, 0, SEEK_END);
    if(0 != retf)
    {
        ulogerr("ufile : %s\n", "fseek.");
        return -1;
    }

    nfwr = fwrite(ptr, size, nmemb, fp); 
    if(nfwr != nmemb)
    {
        ulogerr("ufile : %s\n", "fwrite.");
        return -1;
    }

    fclose(fp);

    fp = fopen(filename, "r+");
    if(NULL == fp)
    {
        ulogerr("ufile : %s\n", "open file error.");
        return -1;
    }

    char buf;
    long offset = ufile_length(filename) - 1 ;
    while(offset >= (index + (size*nmemb)))
    {
        retf = fseek(fp, offset-(size*nmemb), SEEK_SET);
        if(0 != retf)
        {
            ulogerr("ufile : %s\n", "fseek.");
            return -1;
        }
        nfwr = fread(&buf, 1, 1, fp);
        if(nfwr != 1)
        {
            ulogerr("ufile : %s\n", "fread.");
            return -1;
        }

        retf = fseek(fp, offset, SEEK_SET);
        if(0 != retf)
        {
            ulogerr("ufile : %s\n", "fseek.");
            return -1;
        }
        nfwr = fwrite(&buf, 1, 1, fp);
        if(nfwr != 1)
        {
            ulogerr("ufile : %s\n", "fwrite.");
            return -1;
        }

        offset --;
    }

    retf = fseek(fp, index, SEEK_SET);
    if(0 != retf)
    {
        ulogerr("ufile : %s\n", "fseek.");
        return -1;
    }
    nfwr = fwrite(ptr, size, nmemb, fp);
    if(nfwr != nmemb)
    {
        ulogerr("ufile : %s\n", "fwrite.");
        return -1;
    }

    fclose(fp);
    return 0;
}


int ufile_insert_content1(const char* filename, long index, 
        void *ptr, size_t size, size_t nmemb)
{
    FILE* fp = fopen(filename, "a");
    if(NULL == fp)
    {
        ulogerr("ufile : %s\n", "open file error.");
        return -1 ;
    }

    int retf;
    int nfwr;

    retf = fseek(fp, 0, SEEK_END);
    if(0 != retf)
    {
        ulogerr("ufile : %s\n", "fseek.");
        return -1 ;
    }

    nfwr = fwrite(ptr, size, nmemb, fp); 
    if(nfwr != nmemb)
    {
        ulogerr("ufile : %s\n", "fwrite.");
        return -1 ;
    }

    fclose(fp);

    fp = fopen(filename, "r+");
    if(NULL == fp)
    {
        ulogerr("ufile : %s\n", "open file error.");
        return -1 ;
    }

    int membsize = size*nmemb;
    char* buf;
    buf = um_malloc(membsize*sizeof(char));
    if(NULL==buf)
    {
        return -1 ;
    }

    long filelength = ufile_length(filename);
    long movelength = filelength-index;
    int n = movelength/membsize ;
    int ndevide = movelength-n*membsize;
    int i = n - 2 ;

    for(;i>=0;i--)
    {
        long offset_read = (i-n)*membsize;
        retf = fseek(fp, offset_read, SEEK_END); 
        if(0 != retf)
        {
            ulogerr("ufile : %s\n", "fseek.");
            goto error;
        }
        nfwr = fread(buf, 1, membsize, fp);
        if(nfwr != nmemb)
        {
            ulogerr("ufile : %s\n", "fread.");
            goto error;
        }

        long offset_write = (i+1-n)*membsize;
        retf = fseek(fp, offset_write, SEEK_END);
        if(0 != retf)
        {
            ulogerr("ufile : %s\n", "fseek.");
            goto error;
        }
        nfwr = fwrite(buf, 1, membsize, fp);
        if(nfwr != membsize)
        {
            ulogerr("ufile : %s\n", "fwrite.");
            goto error;
        }
    }

    for(i=ndevide-1; i>=0; i--)
    {
        retf = fseek(fp, index+i, SEEK_SET); 
        if(0 != retf)
        {
            ulogerr("ufile : %s\n", "fseek.");
            goto error;
        }
        nfwr = fread(buf, 1, 1, fp);
        if(nfwr != 1)
        {
            ulogerr("ufile : %s\n", "fread.");
            goto error;
        }
        retf = fseek(fp, index+i+membsize, SEEK_SET);
        if(0 != retf)
        {
            ulogerr("ufile : %s\n", "fseek.");
            goto error;
        }
        nfwr = fwrite(buf, 1, 1, fp);
        if(nfwr != 1)
        {
            ulogerr("ufile : %s\n", "fwrite.");
            goto error;
        }
    }

    retf = fseek(fp, index, SEEK_SET);
    if(0 != retf)
    {
        ulogerr("ufile : %s\n", "fseek.");
        goto error;
    }

    nfwr = fwrite(ptr, size, nmemb, fp);
    if(nfwr != nmemb)
    {
        ulogerr("ufile : %s\n", "fwrite.");
        goto error;
    }

    fclose(fp);

    um_free(buf);
    return 0 ;

error:
    if(NULL != buf) {
        um_free(buf);
    }
    return -1 ;
}


/*
    向文件中写入buf内容,长度设置在size, 写方式按照mode指示的方式.
    返回值为0表示, 写出错或者buf长度为0.
*/
size_t ufile_write(const char* path, const char* mode, const void* buf, size_t size)
{
    size_t retn;
    retn = 0;

    FILE* fp = fopen(path, mode); 
    if(NULL == fp) {
        ulogerr("fopen error on <%s><%s>.\n", path, mode);
        return retn;
    }

    retn = fwrite(buf, 1, size, fp);
    if(retn != size) {
        ulogerr("fwrite error on <%s><%s>.\n", path, mode);
        return retn;
    }
    fclose(fp);

    return retn;
}


/*
    向文件中写入字符串str内容, 写方式按照mode指示的方式.
    返回值为0表示, 写出错或者buf长度为0.
*/
size_t ufile_write_string(const char* path, const char* mode, const char* str)
{
    size_t retn;
    retn = ufile_write(path, mode, str, strlen(str));
    return retn;
}


/*
    向文件中写入类printf变参内容, 写方式按照mode指示的方式.
    变参组合成的字符串总长度最大1024.
    返回值为0表示, 写出错或者buf长度为0.
*/
size_t ufile_writevs(const char* path, const char* mode, const char* fmt, ...)
{
    size_t retn;

    #define LEN_BUF 1024
    char buf[LEN_BUF] = {0};
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, LEN_BUF, fmt, ap);
    va_end(ap);
    #undef LEN_BUF 

    retn = ufile_write(path, mode, buf, strlen(buf));
    return retn;
}


/*
    文件不存在时,创建文件.
*/
int ufile_create(const char* path)
{
    int ret = 0;

    if(0 != access(path, F_OK)) {
        FILE* fp = fopen(path, "w");
        if(NULL != fp) {
            fclose(fp);
        }
        else {
            usloge_perror("fopen \"w\"");
            errno = EACCES;
            ret = -1;
        }
    }
    else {
        usloge_perror("exist");
        errno = EEXIST;
        ret = -1;
    }

    return ret;
}


int ufile_replaces(const char* filename, const char* filename_to, 
        const char* needle, const char* to)
{
    uslog_check_arg(filename != NULL    , -1);
    uslog_check_arg(filename_to != NULL , -1);
    uslog_check_arg(needle != NULL      , -1);
    uslog_check_arg(to != NULL          , -1);

    int ret = 0;

    size_t size = 0;
    char* content = ufile_dup(filename, &size);
    if(!content) {
        ret = -1;
        return ret;
    }

    if(NULL == strstr(content, needle)) {
        um_free(content);
        ret = 0;
        return ret;
    }

    ustr_t str;
    ret = ustr_replaces(content, needle, to, &str);
    if(0 != ret) {
        um_free(content);
        return ret;
    }

    size_t len = strlen(str.s);
    size_t size_write = ufile_write(filename_to, "w", str.s, len);
    if(size_write != len) {
        ret = -1;
    }
    
    um_free(content);
    um_free(str.s);

    return ret;
}











































































































































































































































#define LEN_MIN_PATH        128
#define UFILE_GLOBAL_MAX    128
struct fstream_open {
    FILE* fp;
    char* path;
};
static struct fstream_open kfo[UFILE_GLOBAL_MAX] = {{0}};
static int knum_fo = 0;

static FILE* ufile_global_open(const char* path)
{
    FILE* fp_ret = NULL;
    if(NULL == path || strlen(path) > LEN_MIN_PATH-1) {
        fp_ret = NULL;
        return fp_ret;
    }

    if(knum_fo >= UFILE_GLOBAL_MAX) {
        ulogerr("ufile_global_open. full.\n");
        fp_ret = NULL;
        return fp_ret;
    }

    int i;
    for(i=0; i<knum_fo; i++) {
        if(0 == strcmp(path, kfo[i].path)) {
            fp_ret = kfo[i].fp;
            return fp_ret;
        }
    }

    FILE* fp = fopen(path, "w");
    char* dup_name = ustrdup(path);
    if(NULL == fp || NULL == dup_name) {
        if(NULL != fp) {
            fclose(fp);
            fp = NULL;
        }

        if(NULL != dup_name) {
            um_free(dup_name);
            dup_name = NULL;
        }
    }
    else {
        struct fstream_open* fo = &kfo[knum_fo];
        fo->fp = fp;    
        fo->path = dup_name;
        knum_fo ++;

        fp_ret = fp;
    }

    return fp_ret;
}


int ufile_global_write(const char* path, void* buf, size_t size)
{
    int ret = 0;

    FILE* fp = ufile_global_open(path);
    if(NULL == fp) {
        ulogerr("path open error.\n");
        ret = -1;
        return ret;
    }

    size_t nwrite = fwrite(buf, 1, size, fp);
    if(nwrite != size) {
        ulogerr("fwrite error.\n");
        ret = -1;
        return ret;
    }
    fflush(fp);

    return ret;
}


int ufile_global_writevs(const char* path, const char* fmt, ...)
{
    int ret = 0;

    #define LEN_BUF 1024
    char buf[LEN_BUF] = {0};
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, LEN_BUF, fmt, ap);
    va_end(ap);
    #undef LEN_BUF 

    ret = ufile_global_write(path, buf, strlen(buf));

    return ret;
}


int ufile_global_close(void)
{
    int ret = 0;
    int i;
    for(i=0; i<knum_fo; i++) {
        struct fstream_open* fo = &kfo[i];
        if(NULL != fo->fp) {
            fclose(fo->fp);
            fo->fp = 0;
        }
    }
    knum_fo = 0;

    return ret;
}
