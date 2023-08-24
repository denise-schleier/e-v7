#include <stdlib.h>
#include <string.h>

#include "v7.h"

struct mg_str
{
    const char *p; /* Memory chunk pointer */
    size_t len;    /* Memory chunk length */
};

struct mg_str mg_mk_str(const char *s) { return (struct mg_str){NULL, 0}; }
struct mg_str mg_mk_str_n(const char *s, size_t len) { return (struct mg_str){NULL, 0}; }

const char *mg_strchr(const struct mg_str s, int c) { return (char *)0; }

long timezone;

typedef uint64_t val_t;

static void show_usage(char *argv[])
{
    fprintf(stderr, "eV7 version %s (c) Cesanta Software, built on %s\n",
            V7_VERSION, __DATE__);
    fprintf(stderr, "Usage: %s [OPTIONS] ...\n", argv[0]);
    fprintf(stderr, "%s\n", "OPTIONS:");
    fprintf(stderr, "%s\n", "  -e <expr>            execute expression");
    fprintf(stderr, "%s\n", "  -vo <n>              object arena size");
    fprintf(stderr, "%s\n", "  -vf <n>              function arena size");
    fprintf(stderr, "%s\n", "  -vp <n>              property arena size");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int exit_rcode = EXIT_SUCCESS;
    struct v7 *v7;
    struct v7_create_opts opts;
    int as_json = 0;
    int i, j;
    val_t res;
    int nexprs = 0;
    const char *exprs[16];

    memset(&opts, 0, sizeof(opts));

    /* Execute inline code */
    for (i = 1; i < argc && argv[i][0] == '-'; i++)
    {
        if (strcmp(argv[i], "-e") == 0 && i + 1 < argc)
        {
            exprs[nexprs++] = argv[i + 1];
            i++;
        }
        else if (strcmp(argv[i], "-h") == 0)
        {
            show_usage(argv);
        }
        else if (strcmp(argv[i], "-j") == 0)
        {
            as_json = 1;
        }
        else if (strcmp(argv[i], "-vo") == 0 && i + 1 < argc)
        {
            opts.object_arena_size = atoi(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "-vf") == 0 && i + 1 < argc)
        {
            opts.function_arena_size = atoi(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "-vp") == 0 && i + 1 < argc)
        {
            opts.property_arena_size = atoi(argv[i + 1]);
            i++;
        }
    }

    v7 = v7_create_opt(opts);
    res = V7_UNDEFINED;

    /* Execute runtime file (load) */
    if (v7_exec_file(v7, "ev7_runtime.js", &res) != V7_OK)
    {
        v7_print_error(stderr, v7, argv[i], res);
        res = V7_UNDEFINED;
    }

    /* Execute inline expressions */
    for (j = 0; j < nexprs; j++)
    {
        if (v7_exec(v7, exprs[j], &res) != V7_OK)
        {
            v7_print_error(stderr, v7, exprs[j], res);
            exit_rcode = EXIT_FAILURE;
            res = V7_UNDEFINED;
        }
    }

    {
        char buf[2000];
        char *s = v7_stringify(v7, res, buf, sizeof(buf),
                               as_json ? V7_STRINGIFY_JSON : V7_STRINGIFY_DEBUG);
        printf("%s\n", s);
        if (s != buf)
        {
            free(s);
        }
    }

    v7_destroy(v7);
    return exit_rcode;
}
